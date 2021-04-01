/*
  LegalMoveGen, an incomplete UCI chess engine prototype derived from Stockfish 2.2.2 and Kaissa 1.00.
  Copyright (C) 2012 Dr. Dmitri A. Gusev

  Stockfish 2.2.2 is a free, open-source UCI chess engine derived from Glaurung 2.1.
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2012 Marco Costalba, Joona Kiiski, Tord Romstad

  Kaissa 1.00 is a free, open-source chess program derived from the original Kaissa.
  Copyright (C) 1972-1980 G.M. Adelson-Velsky, V.L. Arlazarov, and M.V. Donskoy (Kaissa authors); 
  Other contributors: A.V. Uskov, A.R. Bitman, A. Barayev, A. Leman, and M. Rosenfeld
  Copyright (C) 1990-1991 JV ParaGraph, intellectual property rights transferred to DISCo in 1994;
  Authors of Kaissa 1.00: M.V. Donskoy, A.V. Dubetz, M.Yu. Karaev, V.A. Kokin, 
  D.V. Posvjansky, I.R. Shabalin, A.G. Sidorovitch, E.A. Sokolinsky. 
  Sources used by written permission from DISCo. 

  LegalMoveGen is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  LegalMoveGen is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <fstream>
#include <iostream>
#include <vector>

//#include "misc.h"
//#include "position.h"
//#include "search.h"
//#include "thread.h"
/// D.A.G., 03/07/2012. Commented out #include "ucioption.h":
/// We're not ready to work with the UCI interface yet.
//#include "ucioption.h"
#include "timeman.h"
#include "movegen.h"

using namespace std;

static const char* Defaults[] = {
  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
  "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 10",
  "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 11",
  "4rrk1/pp1n3p/3q2pQ/2p1pb2/2PP4/2P3N1/P2B2PP/4RRK1 b - - 7 19",
  "rq3rk1/ppp2ppp/1bnpb3/3N2B1/3NP3/7P/PPPQ1PP1/2KR3R w - - 7 14",
  "r1bq1r1k/1pp1n1pp/1p1p4/4p2Q/4Pp2/1BNP4/PPP2PPP/3R1RK1 w - - 2 14",
  "r3r1k1/2p2ppp/p1p1bn2/8/1q2P3/2NPQN2/PPP3PP/R4RK1 b - - 2 15",
  "r1bbk1nr/pp3p1p/2n5/1N4p1/2Np1B2/8/PPP2PPP/2KR1B1R w kq - 0 13",
  "r1bq1rk1/ppp1nppp/4n3/3p3Q/3P4/1BP1B3/PP1N2PP/R4RK1 w - - 1 16",
  "4r1k1/r1q2ppp/ppp2n2/4P3/5Rb1/1N1BQ3/PPP3PP/R5K1 w - - 1 17",
  "2rqkb1r/ppp2p2/2npb1p1/1N1Nn2p/2P1PP2/8/PP2B1PP/R1BQK2R b KQ - 0 11",
  "r1bq1r1k/b1p1npp1/p2p3p/1p6/3PP3/1B2NN2/PP3PPP/R2Q1RK1 w - - 1 16",
  "3r1rk1/p5pp/bpp1pp2/8/q1PP1P2/b3P3/P2NQRPP/1R2B1K1 b - - 6 22",
  "r1q2rk1/2p1bppp/2Pp4/p6b/Q1PNp3/4B3/PP1R1PPP/2K4R w - - 2 18",
  "4k2r/1pb2ppp/1p2p3/1R1p4/3P4/2r1PN2/P4PPP/1R4K1 b - - 3 22",
  "3q2k1/pb3p1p/4pbp1/2r5/PpN2N2/1P2P2P/5PP1/Q2R2K1 b - - 4 26"
};


/// benchmark() runs a simple benchmark by letting the program analyze a set
/// of positions for a given limit each. There are five parameters; the
/// transposition table size, the number of search threads that should
/// be used, the limit value spent for each position (optional, default is
/// depth 12), an optional file name where to look for positions in fen
/// format (defaults are the positions defined above) and the type of the
/// limit value: depth (default), time in secs or number of nodes.

void benchmark(int argc, char* argv[]) 
{
	vector<string> fens; // A vector of chess position descriptions in the FEN format
	LimitsType limits;
	int time;
	int64_t nodes = 0;
	
	// Declare and initialize move_none:
	Move move_none;
	move_none.from=A1;
	move_none.to=A1;
	move_none.promotionPieceType=NO_PIECE_TYPE;
	move_none.moveFlags[M_EN_PASSANT]=0;
	move_none.moveFlags[M_CASTLE_SHORT]=0;
	move_none.moveFlags[M_CASTLE_LONG]=0;
	move_none.moveFlags[M_NULL_MOVE]=0;
	move_none.moveFlags[M_MOVE_NONE]=1;
	move_none.moveFlags[M_NO_BOOK_MOVE]=0;
	move_none.moveFlags[M_CAPTURE]=0;
	move_none.moveFlags[M_ISCHECK]=0;
	move_none.moveFlags[M_KILLER]=0;
	move_none.moveFlags[M_PASSAGE]=0;
	move_none.moveFlags[M_CHECK_DEFENSE]=0;
	move_none.moveFlags[M_EVASION]=0;
	move_none.moveFlags[M_QUIET]=0;
	move_none.moveFlags[M_DRAW]=0;
	move_none.moveFlags[M_STALEMATE]=0;
	move_none.moveFlags[M_CHECKMATE]=0;
	move_none.score=0;
	
	// Assign default values to missing arguments
	string ttSize  = argc > 2 ? argv[2] : "128";
	string threads = argc > 3 ? argv[3] : "1";
	string valStr  = argc > 4 ? argv[4] : "12";
	string fenFile = argc > 5 ? argv[5] : "default";
	string valType = argc > 6 ? argv[6] : "depth";
	
	// D.A.G., 03/08/2012. Commented out setting of UCI options.
	// We're not using the UCI interface yet.
	/*
    Options["Hash"]    = ttSize;
    Options["Threads"] = threads;
    Options["OwnBook"] = false;
    */
	
	if (valType == "time")
		limits.maxTime = 1000 * atoi(valStr.c_str()); // maxTime is in ms
	else if (valType == "nodes")
		limits.maxNodes = atoi(valStr.c_str());
	else
		limits.maxDepth = atoi(valStr.c_str());
	
	if (fenFile != "default")
	{
		string fen;
		ifstream file(fenFile.c_str());
		
		if (!file.is_open())
		{
			cerr << "Unable to open file " << fenFile << endl;
			exit(EXIT_FAILURE);
		}
		
		while (getline(file, fen))
			if (!fen.empty())
				fens.push_back(fen);
		
		file.close();
	}
	else
		fens.assign(Defaults, Defaults + 16);
	
	time = system_time();
	
	for (size_t i = 0; i < fens.size(); i++)
	{
		Position pos(fens[i], false, 0);
		cerr << "\nPosition: " << i + 1 << '/' << fens.size() << endl;
		// D.A.G., 03/08/2012: Inserted a debugging output statement
		// to test reading/writing of positions from and to FEN.
		pos.print(move_none);
		RootMoveList rootMoveList; // Create an empty root move list
		rootMoveList.generate_from_position(pos); // Fill the root move list: TO DO!
		rootMoveList.print(); // Print the root move list: TO DO!
		// D.A.G., 03/08/2012: Commented out both forms of search.
		// We're not ready for the search yet.
		/*
		if (valType == "perft")
		{
		    int64_t cnt = Search::perft(pos, limits.maxDepth * ONE_PLY);
            cerr << "\nPerft " << limits.maxDepth  << " leaf nodes: " << cnt << endl;
            nodes += cnt;
        }
        else
        {
            Threads.start_thinking(pos, limits);
            nodes += Search::RootPosition.nodes_searched();
        }
	    */
    }

    time = system_time() - time;

    cerr << "\n==========================="
         << "\nTotal time (ms) : " << time
         << "\nNodes searched  : " << nodes
         << "\nNodes/second    : " << int(nodes / (time / 1000.0)) << endl;
}
