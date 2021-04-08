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
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "timeman.h"

using namespace std;

extern void benchmark(int argc, char* argv[]); // in benchmark.cpp

static const string Version = "1.0";
static const string Tag = "";

/// engine_info() returns the full name of the current LegalMoveGen version.
/// This will be either "LegalMoveGen YYMMDD" (where YYMMDD is the date when
/// the program was compiled) or "Laissa <version number>", depending
/// on whether Version is empty.

const string engine_info(bool to_uci) 
{
  const string months("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
  // D.A.G., 03/07/2012: Commented out to simplify engine_info() reporting.
  /*
  const string cpu64(Is64Bit ? " 64bit" : "");
  const string popcnt(HasPopCnt ? " SSE4.2" : "");
  */
  string month, day, year;
  stringstream s, date(__DATE__); // From compiler, format is "Sep 21 2008"

  if (Version.empty())
  {
      date >> month >> day >> year;

      s << "LegalMoveGen " << Tag
        << setfill('0') << " " << year.substr(2)
        << setw(2) << (1 + months.find(month) / 4)
        << setw(2) << day; // << cpu64 << popcnt;
  }
  else
      s << "LegalMoveGen " << Version; // << cpu64 << popcnt;
  s << (to_uci ? "\nid author ": " by ") // D.A.G., 03/07/2012. Let's print author's name no matter what!
    << "Your Name"; // <- TYPE YOUR NAME HERE!! D.A.G., 03/07/2012

  return s.str();
}

int main(int argc, char* argv[]) 
{
	// initialize numerous useful bitboards:
	bitboards_init();
	// Print several bitboards for debugging purposes:
    /*
	cout << "print_bitboard(SquareBB[C3]);" << endl;
	print_bitboard(SquareBB[C3]);
	cout << "print_bitboard(FileBB[FILE_G]);" << endl;
	print_bitboard(FileBB[FILE_G]);
	cout << "print_bitboard(RankBB[RANK_2]);" << endl;
	print_bitboard(RankBB[RANK_2]);
	cout << "print_bitboard(AdjacentFilesBB[FILE_A]);" << endl;
	print_bitboard(AdjacentFilesBB[FILE_A]);
	cout << "print_bitboard(AdjacentFilesBB[FILE_B]);" << endl;
	print_bitboard(AdjacentFilesBB[FILE_B]);
	cout << "print_bitboard(ThisAndAdjacentFilesBB[FILE_F]);" << endl;
	print_bitboard(ThisAndAdjacentFilesBB[FILE_F]);
	cout << "print_bitboard(InFrontBB[WHITE][RANK_3]);" << endl;
	print_bitboard(InFrontBB[WHITE][RANK_3]);
	cout << "print_bitboard(InFrontBB[BLACK][RANK_7]);" << endl;
	print_bitboard(InFrontBB[BLACK][RANK_7]);
	cout << "print_bitboard(SquaresInFrontMask[WHITE][C5]);" << endl;
	print_bitboard(SquaresInFrontMask[WHITE][C5]);
	cout << "print_bitboard(SquaresInFrontMask[BLACK][C5]);" << endl;
	print_bitboard(SquaresInFrontMask[BLACK][C5]);
	cout << "print_bitboard(PassedPawnMask[WHITE][D4]);" << endl;
	print_bitboard(PassedPawnMask[WHITE][D4]);
	cout << "print_bitboard(PassedPawnMask[BLACK][H7]);" << endl;
	print_bitboard(PassedPawnMask[BLACK][H7]);
	cout << "print_bitboard(AttackSpanMask[WHITE][F4]);" << endl;
	print_bitboard(AttackSpanMask[WHITE][F4]);
	cout << "print_bitboard(AttackSpanMask[BLACK][G5]);" << endl;
	print_bitboard(AttackSpanMask[BLACK][G5]);
	*/
	// init() is a static member function of the Position class.
	// It initializes at startup the arrays used to compute hash 
	// keys and the piece square tables. [D.A.G. disabled computation
	// of the piece square tables.] You may access a static 
	// member function of a class without using an object of the class:
	Position::init();
	
	// D.A.G., 03/07/2012. Commented out initialization of the bitbase, search, and threads:
	// We're not going to use the bitbase, we're not going to search yet, and 
	// we won't mess with threads, either.
	/*
	kpk_bitbase_init();
	Search::init();
	Threads.init();
	*/

	
	// D.A.G., 03/07/2012. I pass true to engine_info() to print author's name no matter what:
	cout << engine_info(true) << endl; 
	// D.A.G., 03/07/2012. Commented out the invocation of uci_loop(); 
	// We're not ready to use the UCI iterface yet.
	/*
	if (argc == 1)
		uci_loop();
	else*/ if ((argc>1)&&(string(argv[1]) == "bench"))
		benchmark(argc, argv);
	else
		cerr << "\nUsage: LegalMoveGen bench [hash size = 128] [threads = 1] "
		     << "[limit = 12] [fen positions file = default] "
             << "[limited by depth, time, nodes or perft = depth]" << endl;
	
	/// D.A.G., 03/08/2012. Commented out the exit call related to threads:
    //Threads.exit();  
}