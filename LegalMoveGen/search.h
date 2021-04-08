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
// It comes fourth in a series of headers: chess.h -> bitboard.h ->
// position.h -> search.h -> timeman.h -> (thread.h)...
#if !defined(SEARCH_H_INCLUDED)
#define SEARCH_H_INCLUDED

#include <cstring>
#include <vector>

#include "position.h"

class Position;
struct SplitPoint;
const int MAX_KILLERS=2; // D.A.G.: Allow up to 2 killer moves?

// D.A.G., 03/08/2012: Commented out namespace Search:
//namespace Search {

/// The Stack struct keeps track of the information we need to remember from
/// nodes shallower and deeper in the tree during the search. Each search thread
/// has its own array of Stack objects, indexed by the current ply.

struct Stack {
  SplitPoint* sp;
  int ply;
  Move currentMove;
  Move excludedMove;
  Move bestMove;
  Move killers[MAX_KILLERS];
  int reduction;  // D.A.G., 03/08/2012. Changed type from Depth
  int eval;       // D.A.G., 03/08/2012. Changed type from Value
  int evalMargin; // D.A.G., 03/08/2012. Changed type from Value
  int skipNullMove;
};

const int VALUE_INFINITE=30001; // Added by D.A.G. to replace Value with int
/// RootMove struct is used for moves at the root of the tree. For each root
/// move we store a score, a node count, and a PV (Preferred Variation;
/// really a refutation in the case of moves which fail low). 
/// Score is normally set at -VALUE_INFINITE for all non-pv moves.
struct RootMove 
{
  RootMove(){} // Needed by sort()
  RootMove(Move m) : score(-VALUE_INFINITE), prevScore(-VALUE_INFINITE) 
  {
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
      
	  pv.push_back(m); pv.push_back(move_none);
  }

  // D.A.G., 03/08/2012: Compare scores of two moves
  bool operator<(const RootMove& m) const { return score < m.score; }
  // D.A.G., 03/08/2012: Compare two moves for equality
  bool operator==(const Move& m) const 
  { 
	  return (pv[0].from == m.from && pv[0].to == m.to 
		  && pv[0].promotionPieceType == m.promotionPieceType); 
  }
  bool operator!=(const Move& m) const 
  { 
	  return (pv[0].from != m.from || pv[0].to != m.to 
		  || pv[0].promotionPieceType != m.promotionPieceType); 
  }
  // D.A.G., 03/19/2012: Needed to work with threads.
  // Commented out: We're not working with threads.
  /*
  void extract_pv_from_tt(Position& pos);
  void insert_pv_in_tt(Position& pos);
  */
  
  int score;     // D.A.G., 03/08/2012. Changed type from Value
  int prevScore; // D.A.G., 03/08/2012. Changed type from Value
  std::vector<Move> pv; // D.A.G., 03/08/2012: Preferred Variation (a vector of moves) 
                        // for the RootMove is stored in this vector 
                        // Our first Move is pv[0], and we're not ready for longer variations!
};


/// The LimitsType struct stores information sent by GUI about available time
/// to search the current move, maximum depth/time, if we are in analysis mode
/// or if we have to ponder while is our opponent's side to move.

struct LimitsType 
{
  LimitsType() { memset(this, 0, sizeof(LimitsType)); }
  bool use_time_management() const { return !(maxTime | maxDepth | maxNodes | infinite); }

  int time, increment, movesToGo, maxTime, maxDepth, maxNodes, infinite, ponder;
};


/// The SignalsType struct stores volatile flags updated during the search
/// typically in an async fashion, for instance to stop the search by the GUI.

struct SignalsType 
{
  bool stopOnPonderhit, firstRootMove, stop, failedLowAtRoot;
};

extern volatile SignalsType Signals;
extern LimitsType Limits;
extern std::vector<RootMove> RootMoves;
extern Position RootPosition;

extern void init();
extern int64_t perft(Position& pos, int depth); // D.A.G., 03/08/2012: Changed from Depth to int
extern void think();

//} // namespace Search commented out by D.A.G., 03/08/2012

#endif // !defined(SEARCH_H_INCLUDED)
