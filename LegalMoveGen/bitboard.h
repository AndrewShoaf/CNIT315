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
// bitboard.h provides our chess program with the definitions essential to
// implementation of bitboards. It is derived from bitboard.h in Stockfish 2.2.2
// It comes second in a series of headers: chess.h -> bitboard.h ->
// position.h -> search.h -> timeman.h -> (thread.h)...

//#if !defined(BITBOARD_H_INCLUDED)
//#define BITBOARD_H_INCLUDED

#include "chess.h"

using namespace std;

typedef   signed __int64  int64_t;
typedef unsigned __int64 uint64_t;

typedef bitset<NUM_SQUARES> Bitboard;
typedef Bitboard Key; // For hash keys!

const Bitboard FileABB = 0x0101010101010101ULL;
const Bitboard FileBBB = FileABB << 1;
const Bitboard FileCBB = FileABB << 2;
const Bitboard FileDBB = FileABB << 3;
const Bitboard FileEBB = FileABB << 4;
const Bitboard FileFBB = FileABB << 5;
const Bitboard FileGBB = FileABB << 6;
const Bitboard FileHBB = FileABB << 7;

const Bitboard Rank1BB = 0xFF;
const Bitboard Rank2BB = Rank1BB << (8 * 1);
const Bitboard Rank3BB = Rank1BB << (8 * 2);
const Bitboard Rank4BB = Rank1BB << (8 * 3);
const Bitboard Rank5BB = Rank1BB << (8 * 4);
const Bitboard Rank6BB = Rank1BB << (8 * 5);
const Bitboard Rank7BB = Rank1BB << (8 * 6);
const Bitboard Rank8BB = Rank1BB << (8 * 7);

// Everything variable (non-const) has to be extern.
// The following variables are declared in bitboard.cpp:

// Commented out by D.A.G.:
/*
extern Bitboard RMasks[NUM_SQUARES];
extern Bitboard RMagics[NUM_SQUARES];
extern Bitboard* RAttacks[NUM_SQUARES];
extern int RShifts[NUM_SQUARES];

extern Bitboard BMasks[NUM_SQUARES];
extern Bitboard BMagics[NUM_SQUARES];
extern Bitboard* BAttacks[NUM_SQUARES];
extern int BShifts[NUM_SQUARES];
*/

extern Bitboard SquareBB[NUM_SQUARES];
extern Bitboard FileBB[NUM_FILES];
extern Bitboard RankBB[NUM_RANKS];
extern Bitboard AdjacentFilesBB[NUM_FILES];
extern Bitboard ThisAndAdjacentFilesBB[NUM_FILES];
extern Bitboard InFrontBB[NUM_COLORS][NUM_RANKS];
extern Bitboard StepAttacksBB[NUM_PIECES][NUM_SQUARES];
extern Bitboard BetweenBB[NUM_SQUARES][NUM_SQUARES];
extern Bitboard SquaresInFrontMask[NUM_COLORS][NUM_SQUARES];
extern Bitboard PassedPawnMask[NUM_COLORS][NUM_SQUARES];
extern Bitboard AttackSpanMask[NUM_COLORS][NUM_SQUARES];
// PseudoAttacks commented out by D.A.G.:
//extern Bitboard PseudoAttacks[NUM_PIECE_TYPES][NUM_SQUARES];

extern int SquareDistance[NUM_SQUARES][NUM_SQUARES];

/// Overloads of bitwise operators between a Bitboard and a Square for testing
/// whether a given bit is set in a bitboard, and for setting and clearing bits.

inline Bitboard operator&(Bitboard b, Square s) 
{
  return b & SquareBB[s];
}

inline Bitboard& operator|=(Bitboard& b, Square s) 
{
  return b |= SquareBB[s], b;
}

inline Bitboard& operator^=(Bitboard& b, Square s) 
{
  return b ^= SquareBB[s], b;
}

/// rank_bb() and file_bb() take a file or a square as input and return
/// a bitboard representing all squares on the given file or rank.

inline Bitboard rank_bb(Rank r) 
{
  return RankBB[r];
}

inline Bitboard rank_bb(Square s) 
{
  return RankBB[rank_of(s)];
}

inline Bitboard file_bb(File f) 
{
  return FileBB[f];
}

inline Bitboard file_bb(Square s) 
{
  return FileBB[file_of(s)];
}

/// adjacent_files_bb takes a file as input and returns a bitboard representing
/// all squares on the adjacent files.

inline Bitboard adjacent_files_bb(File f) 
{
  return AdjacentFilesBB[f];
}

/// this_and_adjacent_files_bb takes a file as input and returns a bitboard
/// representing all squares on the given and adjacent files.

inline Bitboard this_and_adjacent_files_bb(File f) 
{
  return ThisAndAdjacentFilesBB[f];
}

/// in_front_bb() takes a color and a rank or square as input, and returns a
/// bitboard representing all the squares on all ranks in front of the rank
/// (or square), from the given color's point of view.  For instance,
/// in_front_bb(WHITE, RANK_5) will give all squares on ranks 6, 7 and 8, while
/// in_front_bb(BLACK, SQ_D3) will give all squares on ranks 1 and 2.

inline Bitboard in_front_bb(Color c, Rank r) 
{
  return InFrontBB[c][r];
}

inline Bitboard in_front_bb(Color c, Square s) 
{
  return InFrontBB[c][rank_of(s)];
}

/// passed_pawn_mask takes a color and a square as input, and returns a
/// bitboard mask which can be used to test if a pawn of the given color on
/// the given square is a passed pawn. Definition of the table is:
/// PassedPawnMask[c][s] = in_front_bb(c, s) & this_and_adjacent_files_bb(s)
inline Bitboard passed_pawn_mask(Color c, Square s) 
{
  return PassedPawnMask[c][s];
}

/// print_bitboard() prints a bitboard in an easily readable format to the
/// standard output. This is sometimes useful for debugging. 
/// The function's decalaration is provided below:
void print_bitboard(Bitboard b);
/// bitboards_init() initializes various bitboard arrays. It is called during
/// program initialization. This is its declaration:
void bitboards_init();

//#endif // !defined(BITBOARD_H_INCLUDED)