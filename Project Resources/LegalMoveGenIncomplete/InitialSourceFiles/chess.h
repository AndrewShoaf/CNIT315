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
// chess.h provides our chess program with the most basic definitions
// It is modeled after CHESS.H in Kaissa 1.00 and types.h in Stockfish 2.2.2
// It comes first in a series of headers: chess.h -> bitboard.h ->
// position.h -> search.h -> timeman.h -> (thread.h)...
// chess.h contains the array of proto-moves created by ProtoMoveGen
#if !defined(CHESS_H_INCLUDED)
#define CHESS_H_INCLUDED
#include <bitset>
using namespace std;

const int NUM_COLORS=2;
enum Color {NO_COLOR=-1, WHITE=0, BLACK=1}; 
// Color is similar to OLD_PIECE_COLOR in Kaissa 1.00 (CHESS.H)
// and Color in Stockfish 2.2.2 (types.h)
const int NUM_PIECE_TYPES=7;
enum PieceType {NO_PIECE_TYPE, PAWN, KNIGHT, KING, QUEEN, ROOK, BISHOP};
// PieceType is similar to OLD_PIECE_NAME in Kaissa 1.00 (CHESS.H)
// and PieceType in Stockfish 2.2.2 (types.h)

enum PieceCategory {NO_CATEGORY=-1, PAWN_KNIGHT_OR_KING, SLIDING_PIECE};
// Only sliding pieces (bishops, rooks, and queens) can pin other pieces.
const int NUM_PIECES=14; 
enum Piece {NO_PIECE, EN_PASSANT, WHITE_PAWN, BLACK_PAWN, WHITE_KNIGHT, 
	BLACK_KNIGHT, WHITE_KING, BLACK_KING, WHITE_QUEEN, BLACK_QUEEN, 
	WHITE_ROOK, BLACK_ROOK, WHITE_BISHOP, BLACK_BISHOP}; 
// Piece is a cross of OLD_PIECE_NAME in Kaissa 1.00 (CHESS.H) and
// Piece in Stockfish 2.2.2 (types.h)
const int NUM_SQUARES=64; // not counting the NO square
extern int SquareDistance[NUM_SQUARES][NUM_SQUARES]; // sits in bitboard.cpp
enum Square {A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8, NO=-1};
// Square is similar to OLD_SQUARE_NUM in Kaissa 1.00 (CHESS.H) and
// Square in Stockfish 2.2.2 (types.h)

// A piece can move in at most 8 directions:
const int MAX_DIRECTIONS = 8; 
// A piece can have up to 7 options of where to move to in a given 
// direction. We add a pseudo-option for a sentinel (NO):
const int MAX_OPTIONS = 8;
// File and Rank below are from Stockfish 2.2.2 (types.h):
const int NUM_FILES = 8;
enum File {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};
const int NUM_RANKS = 8;
enum Rank {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
const File filesOfSquares[NUM_SQUARES] = 
   {FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H,
	FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H};
const Rank ranksOfSquares[NUM_SQUARES] = 
   {RANK_1,RANK_1,RANK_1,RANK_1,RANK_1,RANK_1,RANK_1,RANK_1,
	RANK_2,RANK_2,RANK_2,RANK_2,RANK_2,RANK_2,RANK_2,RANK_2,
	RANK_3,RANK_3,RANK_3,RANK_3,RANK_3,RANK_3,RANK_3,RANK_3,
	RANK_4,RANK_4,RANK_4,RANK_4,RANK_4,RANK_4,RANK_4,RANK_4,
	RANK_5,RANK_5,RANK_5,RANK_5,RANK_5,RANK_5,RANK_5,RANK_5,
	RANK_6,RANK_6,RANK_6,RANK_6,RANK_6,RANK_6,RANK_6,RANK_6,
	RANK_7,RANK_7,RANK_7,RANK_7,RANK_7,RANK_7,RANK_7,RANK_7,
	RANK_8,RANK_8,RANK_8,RANK_8,RANK_8,RANK_8,RANK_8,RANK_8};

// knightOffsets for a knight to jump around the chessboard
const int knightOffsets[MAX_DIRECTIONS]={-17,-15,-10,-6,6,10,15,17};

// Two halves of the board (for ProtoMoveGen):
const int NUM_HALVES=2;
enum Half {LEFT_HALF, RIGHT_HALF}; 

// Proto-move priorities vary from left to right half of the board
// knightPriorities were used in ProtoMoveGen. We won't need them here.
// They provided an index into knightOffsets.
const int knightPriorities[NUM_COLORS][NUM_HALVES][MAX_DIRECTIONS]=
{{{7,5,6,4,3,1,2,0},{6,4,7,5,2,0,3,1}},
 {{1,3,0,2,5,7,4,6},{0,2,1,3,4,6,5,7}}}; 

// CastleRight is from Stockfish 2.2.2.
const int NUM_CASTLE_RIGHTS=16; // to cover all combinations
enum CastleRight 
{
  CASTLES_NONE = 0,
  WHITE_OO     = 1,
  BLACK_OO     = 2,
  WHITE_OOO    = 4,
  BLACK_OOO    = 8,
  ALL_CASTLES  = 15
};

// colorOf computes Color from Piece
inline Color colorOf(Piece p) 
{
	return Color(p>>1?p&1:-1);
}

// pieceTypeOf computes PieceType from Piece
inline PieceType pieceTypeOf(Piece p)
{
	return PieceType(p >> 1);
}

// pieceCategoryOf computes PieceCategory from Piece
inline PieceCategory pieceCategoryOf(Piece p)
{
	return PieceCategory(p>>1?(p>>3):-1);
}

inline Piece make_piece(Color c, PieceType pt) 
{
  return Piece((pt << 1) | c);
}

// make_square computes Square from File and Rank
inline Square make_square(File f, Rank r) 
{
  return Square((r << 3) | f);
}

// needed to compute StepAttacksBB
inline bool square_is_ok(Square s) 
{
  return s >= A1 && s <= H8;
}

// file_of computes file of Square
inline File file_of(Square s) 
{
  return File(s & 7);
}

// rank_of computes rank of Square
inline Rank rank_of(Square s) 
{
  return Rank(s >> 3);
}

// file_distance computes file distance between two squares
inline int file_distance(Square s1, Square s2) 
{
  return abs(file_of(s1) - file_of(s2));
}

// rank_distance computes rank distance between two squares
inline int rank_distance(Square s1, Square s2) 
{
  return abs(rank_of(s1) - rank_of(s2));
}

// return a char corresponding to a given piece type
inline char piece_type_to_char(PieceType pt) 
{
  return " PNKRQB"[pt];
}

// return a char corresponding to a given chessboard file
inline char file_to_char(File f) 
{
  return char(f - FILE_A + int('a'));
}

// return a char corresponding to a given chessboard rank 
inline char rank_to_char(Rank r) 
{
  return char(r - RANK_1 + int('1'));
}

// along with square_is_ok, also needed to compute StepAttacksBB
inline int square_distance(Square s1, Square s2) 
{
  return SquareDistance[s1][s2];
}

inline Square relative_square(Color c, Square s) 
{
  return Square(s ^ (c * 56));
}

inline bool opposite_colors(Square s1, Square s2) 
{
  int s = s1 ^ s2;
  return ((s >> 3) ^ s) & 1;
}

inline Rank relative_rank(Color c, Rank r) 
{
  return Rank(r ^ (c * 7));
}

inline Rank relative_rank(Color c, Square s) 
{
  return relative_rank(c, rank_of(s));
}

const int NUM_MOVE_FLAGS=16;
// Our move flags are modeled after Kaissa 1.00
// M_PROMOTION is excluded: Can be deduced from promotionPieceType in Move
// M_NO_BOOK_MOVE added to deal with the opening book eventually
// M_MOVE_NONE, M_EVASION, M_QUIET, M_DRAW, M_STALEMATE, M_CHECKMATE added
enum MoveFlagNames {M_EN_PASSANT, M_CASTLE_SHORT, M_CASTLE_LONG, M_NULL_MOVE, M_MOVE_NONE,
	M_NO_BOOK_MOVE, M_CAPTURE, M_ISCHECK, M_KILLER, M_PASSAGE, M_CHECK_DEFENSE, M_EVASION, 
    M_QUIET, M_DRAW, M_STALEMATE, M_CHECKMATE};
typedef bitset<NUM_MOVE_FLAGS> MoveFlags;

const int MAX_MOVES      = 256; // D.A.G., 02/04/2012: No position is known with more than 218 moves to choose from 
const int MAX_PLY        = 100; // D.A.G., 02/06/2012: 2 plys = 1 move (made by both sides)
const int MAX_PLY_PLUS_2 = MAX_PLY + 2;
// struct Move is modeled after Kaissa 1.00
struct Move
{
  Square from;
  Square to;
  PieceType promotionPieceType;
  MoveFlags moveFlags;
  int score; // D.A.G., 02/26/2012: score comes from Stockfish 2.2.2. 
             // Kaissa 1.00 had "profit" (capture profit). 
};

// An operator to compare two move scores:
inline bool operator<(const Move& f, const Move& s) 
{
  return f.score < s.score;
}

// An operator to invert a color:
inline Color operator~(Color c) 
{
  return Color(c ^ 1);
}

// Returns a move's square of origin:
inline Square from_sq(Move m) 
{
  return m.from;
}

// Returns a move's destination:
inline Square to_sq(Move m) 
{
  return m.to;
}

// A useful check: For example, our move_none 
// from search.h (RootMove) is not ok.
inline bool is_ok(Move m) 
{
  return from_sq(m) != to_sq(m); 
}

// is_promotion returns true if a move is a promotion
inline bool is_promotion(Move m) 
{
  return m.promotionPieceType != NO_PIECE_TYPE;
}

// is_enpassant returns true if a move is en passant
inline int is_enpassant(Move m) {
  return m.moveFlags[M_EN_PASSANT];
}

// is_castle returns true if a move is castling
inline int is_castle(Move m) {
  return m.moveFlags[M_CASTLE_SHORT]|m.moveFlags[M_CASTLE_LONG];
}

// is_special returns true if a move is either promotion,
// or en passant, or castle
inline bool is_special(Move m) 
{
  return is_enpassant(m) || is_castle(m) || is_promotion(m);
}

// D.A.G., 03/19/2012. Piece values of Kaissa 1.00 multiplied by 120 and adjusted:
const int PawnValueMidgame   =  240; /* Stockfish 2.2.2: 0x0C6 (198)  */ /* Kaissa 1.00: 2 */
const int PawnValueEndgame   =  312; /* Stockfish 2.2.2: 0x102 (258)  */
const int KnightValueMidgame =  821; /* Stockfish 2.2.2: 0x331 (817)  */ /* Kaissa 1.00: 7 */
const int KnightValueEndgame =  829; /* Stockfish 2.2.2: 0x34E (846)  */
const int BishopValueMidgame =  840; /* Stockfish 2.2.2: 0x344 (836)  */ /* Kaissa 1.00: 7 */
const int BishopValueEndgame =  840; /* Stockfish 2.2.2: 0x359 (857)  */
const int RookValueMidgame   = 1200; /* Stockfish 2.2.2: 0x4F6 (1270) */ /* Kaissa 1.00: 10 */
const int RookValueEndgame   = 1200; /* Stockfish 2.2.2: 0x4FE (1278) */
const int QueenValueMidgame  = 2280; /* Stockfish 2.2.2: 0x9D9 (2521) */ /* Kaissa 1.00: 19 */
const int QueenValueEndgame  = 2314; /* Stockfish 2.2.2: 0x9FE (2558) */

// The array of proto-moves created by ProtoMoveGen:
const Square protoMoves[NUM_PIECES][NUM_SQUARES][MAX_DIRECTIONS][MAX_OPTIONS]={
/* begin NO_PIECE */
{/* NO_PIECE A1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE A8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE B8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE C8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE D8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE E8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE F8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE G8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* NO_PIECE H8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end NO_PIECE */
/* begin EN_PASSANT */
{/* EN_PASSANT A1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H2 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H3 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H4 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H5 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H6 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H7 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT A8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT B8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT C8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT D8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT E8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT F8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT G8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* EN_PASSANT H8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end EN_PASSANT */
/* begin WHITE_PAWN */
{/* WHITE_PAWN A1 */
{{A2,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B1 */
{{B2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C1 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D1 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E1 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F1 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G1 */
{{G2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H1 */
{{H2,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A2 */
{{A3,A4,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B2 */
{{B3,B4,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C2 */
{{C3,C4,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D2 */
{{D3,D4,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E2 */
{{E3,E4,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F2 */
{{F3,F4,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G2 */
{{G3,G4,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H2 */
{{H3,H4,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A3 */
{{A4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B3 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C3 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D3 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E3 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F3 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G3 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H3 */
{{H4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A4 */
{{A5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B4 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C4 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D4 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E4 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F4 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G4 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H4 */
{{H5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A5 */
{{A6,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B5 */
{{B6,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C5 */
{{C6,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D5 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E5 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F5 */
{{F6,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G5 */
{{G6,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H5 */
{{H6,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A6 */
{{A7,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B6 */
{{B7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C6 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D6 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E6 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F6 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G6 */
{{G7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H6 */
{{H7,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A7 */
{{A8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B7 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G7 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H7 */
{{H8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN A8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN B8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN C8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN D8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN E8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN F8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN G8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_PAWN H8 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_PAWN */
/* begin BLACK_PAWN */
{/* BLACK_PAWN A1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H1 */
{{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A2 */
{{A1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B2 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G2 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H2 */
{{H1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A3 */
{{A2,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B3 */
{{B2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C3 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D3 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E3 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F3 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G3 */
{{G2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H3 */
{{H2,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A4 */
{{A3,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B4 */
{{B3,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C4 */
{{C3,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D4 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E4 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F4 */
{{F3,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G4 */
{{G3,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H4 */
{{H3,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A5 */
{{A4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B5 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C5 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D5 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E5 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F5 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G5 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H5 */
{{H4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A6 */
{{A5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B6 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C6 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D6 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E6 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F6 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G6 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H6 */
{{H5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A7 */
{{A6,A5,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B7 */
{{B6,B5,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C7 */
{{C6,C5,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D7 */
{{D6,D5,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E7 */
{{E6,E5,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F7 */
{{F6,F5,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G7 */
{{G6,G5,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H7 */
{{H6,H5,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN A8 */
{{A7,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN B8 */
{{B7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN C8 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN D8 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN E8 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN F8 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN G8 */
{{G7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_PAWN H8 */
{{H7,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end BLACK_PAWN */
/* begin WHITE_KNIGHT */
{/* WHITE_KNIGHT A1 */
{{B3,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B1 */
{{C3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C1 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D1 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E1 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F1 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G1 */
{{F3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H1 */
{{G3,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A2 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B2 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C2 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{E1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D2 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E2 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F2 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{D1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G2 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H2 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A3 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B3 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C3 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D3 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E3 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F3 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G3 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H3 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A4 */
{{B6,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B4 */
{{C6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C4 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D4 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E4 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F4 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G4 */
{{F6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H4 */
{{G6,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A5 */
{{B7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B5 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C5 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D5 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E5 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F5 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G5 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H5 */
{{G7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A6 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B6 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C6 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D6 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E6 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F6 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G6 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H6 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT A8 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT B8 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT C8 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT D8 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT E8 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT F8 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT G8 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KNIGHT H8 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_KNIGHT */
/* begin BLACK_KNIGHT */
{/* BLACK_KNIGHT A1 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B1 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C1 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D1 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E1 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F1 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G1 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H1 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A3 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B3 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C3 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D3 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E3 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F3 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G3 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H3 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A4 */
{{B2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B4 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C4 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D4 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E4 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F4 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G4 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H4 */
{{G2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A5 */
{{B3,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B5 */
{{C3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C5 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D5 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E5 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F5 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G5 */
{{F3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H5 */
{{G3,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A6 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B6 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C6 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D6 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E6 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F6 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G6 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H6 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A7 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B7 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C7 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{E8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D7 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E7 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F7 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{D8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G7 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H7 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT A8 */
{{B6,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT B8 */
{{C6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT C8 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT D8 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT E8 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT F8 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT G8 */
{{F6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KNIGHT H8 */
{{G6,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end BLACK_KNIGHT */
/* begin WHITE_KING */
{/* WHITE_KING A1 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B1 */
{{A1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C1 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D1 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E1 */
{{F1,G1,NO,NO,NO,NO,NO,NO},{D1,C1,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F1 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G1 */
{{H1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H1 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A2 */
{{B3,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B2 */
{{C3,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C2 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D2 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{D1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E2 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{E1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F2 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G2 */
{{F3,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H2 */
{{G3,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A3 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B3 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C3 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D3 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E3 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F3 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G3 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H3 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A4 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B4 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C4 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D4 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E4 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F4 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G4 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H4 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A5 */
{{B6,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B5 */
{{C6,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C5 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D5 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E5 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F5 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G5 */
{{F6,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H5 */
{{G6,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A6 */
{{B7,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B6 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C6 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D6 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E6 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F6 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G6 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H6 */
{{G7,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A7 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H7 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING A8 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING B8 */
{{C7,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING C8 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING D8 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING E8 */
{{D7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING F8 */
{{E7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING G8 */
{{F7,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_KING H8 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_KING */
/* begin BLACK_KING */
{/* BLACK_KING A1 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B1 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C1 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D1 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E1 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F1 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G1 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H1 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A2 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{D2,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{E2,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H2 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A3 */
{{B2,NO,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B3 */
{{C2,NO,NO,NO,NO,NO,NO,NO},{B2,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C3 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{C2,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{B2,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D3 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{D2,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{C2,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E3 */
{{D2,NO,NO,NO,NO,NO,NO,NO},{E2,NO,NO,NO,NO,NO,NO,NO},
{D3,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{F2,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F3 */
{{E2,NO,NO,NO,NO,NO,NO,NO},{F2,NO,NO,NO,NO,NO,NO,NO},
{E3,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{G2,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G3 */
{{F2,NO,NO,NO,NO,NO,NO,NO},{G2,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H3 */
{{G2,NO,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A4 */
{{B3,NO,NO,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B4 */
{{C3,NO,NO,NO,NO,NO,NO,NO},{B3,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C4 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{C3,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{B3,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D4 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{D3,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{C3,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E4 */
{{D3,NO,NO,NO,NO,NO,NO,NO},{E3,NO,NO,NO,NO,NO,NO,NO},
{D4,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{F3,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F4 */
{{E3,NO,NO,NO,NO,NO,NO,NO},{F3,NO,NO,NO,NO,NO,NO,NO},
{E4,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{G3,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G4 */
{{F3,NO,NO,NO,NO,NO,NO,NO},{G3,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H4 */
{{G3,NO,NO,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A5 */
{{B4,NO,NO,NO,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B5 */
{{C4,NO,NO,NO,NO,NO,NO,NO},{B4,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C5 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{C4,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{B4,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D5 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{D4,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{C4,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E5 */
{{D4,NO,NO,NO,NO,NO,NO,NO},{E4,NO,NO,NO,NO,NO,NO,NO},
{D5,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{F4,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F5 */
{{E4,NO,NO,NO,NO,NO,NO,NO},{F4,NO,NO,NO,NO,NO,NO,NO},
{E5,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{G4,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G5 */
{{F4,NO,NO,NO,NO,NO,NO,NO},{G4,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H5 */
{{G4,NO,NO,NO,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A6 */
{{B5,NO,NO,NO,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B6 */
{{C5,NO,NO,NO,NO,NO,NO,NO},{B5,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C6 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{C5,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{B5,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D6 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{D5,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{C5,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E6 */
{{D5,NO,NO,NO,NO,NO,NO,NO},{E5,NO,NO,NO,NO,NO,NO,NO},
{D6,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{F5,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F6 */
{{E5,NO,NO,NO,NO,NO,NO,NO},{F5,NO,NO,NO,NO,NO,NO,NO},
{E6,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{G5,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G6 */
{{F5,NO,NO,NO,NO,NO,NO,NO},{G5,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H6 */
{{G5,NO,NO,NO,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A7 */
{{B6,NO,NO,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B7 */
{{C6,NO,NO,NO,NO,NO,NO,NO},{B6,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C7 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{C6,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{B6,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D7 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{D6,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{C6,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{D8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E7 */
{{D6,NO,NO,NO,NO,NO,NO,NO},{E6,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{F6,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{E8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F7 */
{{E6,NO,NO,NO,NO,NO,NO,NO},{F6,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{G6,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G7 */
{{F6,NO,NO,NO,NO,NO,NO,NO},{G6,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H7 */
{{G6,NO,NO,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING A8 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{B7,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING B8 */
{{A8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C7,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING C8 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{B7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING D8 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{C7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{D7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING E8 */
{{F8,G8,NO,NO,NO,NO,NO,NO},{D8,C8,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{D7,NO,NO,NO,NO,NO,NO,NO},
{E7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING F8 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{E7,NO,NO,NO,NO,NO,NO,NO},
{F7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING G8 */
{{H8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F7,NO,NO,NO,NO,NO,NO,NO},
{G7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_KING H8 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{G7,NO,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end BLACK_KING */
/* begin WHITE_QUEEN */
{/* WHITE_QUEEN A1 */
{{B2,C3,D4,E5,F6,G7,H8,NO},{A2,A3,A4,A5,A6,A7,A8,NO},
{B1,C1,D1,E1,F1,G1,H1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B1 */
{{C2,D3,E4,F5,G6,H7,NO,NO},{B2,B3,B4,B5,B6,B7,B8,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C1,D1,E1,F1,G1,H1,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C1 */
{{D2,E3,F4,G5,H6,NO,NO,NO},{C2,C3,C4,C5,C6,C7,C8,NO},
{B2,A3,NO,NO,NO,NO,NO,NO},{D1,E1,F1,G1,H1,NO,NO,NO},
{B1,A1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D1 */
{{E2,F3,G4,H5,NO,NO,NO,NO},{D2,D3,D4,D5,D6,D7,D8,NO},
{C2,B3,A4,NO,NO,NO,NO,NO},{E1,F1,G1,H1,NO,NO,NO,NO},
{C1,B1,A1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E1 */
{{D2,C3,B4,A5,NO,NO,NO,NO},{E2,E3,E4,E5,E6,E7,E8,NO},
{F2,G3,H4,NO,NO,NO,NO,NO},{D1,C1,B1,A1,NO,NO,NO,NO},
{F1,G1,H1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F1 */
{{E2,D3,C4,B5,A6,NO,NO,NO},{F2,F3,F4,F5,F6,F7,F8,NO},
{G2,H3,NO,NO,NO,NO,NO,NO},{E1,D1,C1,B1,A1,NO,NO,NO},
{G1,H1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G1 */
{{F2,E3,D4,C5,B6,A7,NO,NO},{G2,G3,G4,G5,G6,G7,G8,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F1,E1,D1,C1,B1,A1,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H1 */
{{G2,F3,E4,D5,C6,B7,A8,NO},{H2,H3,H4,H5,H6,H7,H8,NO},
{G1,F1,E1,D1,C1,B1,A1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A2 */
{{B3,C4,D5,E6,F7,G8,NO,NO},{A3,A4,A5,A6,A7,A8,NO,NO},
{B2,C2,D2,E2,F2,G2,H2,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B2 */
{{C3,D4,E5,F6,G7,H8,NO,NO},{B3,B4,B5,B6,B7,B8,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{C2,D2,E2,F2,G2,H2,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C2 */
{{D3,E4,F5,G6,H7,NO,NO,NO},{C3,C4,C5,C6,C7,C8,NO,NO},
{B3,A4,NO,NO,NO,NO,NO,NO},{D2,E2,F2,G2,H2,NO,NO,NO},
{B2,A2,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D2 */
{{E3,F4,G5,H6,NO,NO,NO,NO},{D3,D4,D5,D6,D7,D8,NO,NO},
{C3,B4,A5,NO,NO,NO,NO,NO},{E2,F2,G2,H2,NO,NO,NO,NO},
{C2,B2,A2,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{D1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E2 */
{{D3,C4,B5,A6,NO,NO,NO,NO},{E3,E4,E5,E6,E7,E8,NO,NO},
{F3,G4,H5,NO,NO,NO,NO,NO},{D2,C2,B2,A2,NO,NO,NO,NO},
{F2,G2,H2,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{E1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F2 */
{{E3,D4,C5,B6,A7,NO,NO,NO},{F3,F4,F5,F6,F7,F8,NO,NO},
{G3,H4,NO,NO,NO,NO,NO,NO},{E2,D2,C2,B2,A2,NO,NO,NO},
{G2,H2,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G2 */
{{F3,E4,D5,C6,B7,A8,NO,NO},{G3,G4,G5,G6,G7,G8,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{F2,E2,D2,C2,B2,A2,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H2 */
{{G3,F4,E5,D6,C7,B8,NO,NO},{H3,H4,H5,H6,H7,H8,NO,NO},
{G2,F2,E2,D2,C2,B2,A2,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A3 */
{{B4,C5,D6,E7,F8,NO,NO,NO},{A4,A5,A6,A7,A8,NO,NO,NO},
{B3,C3,D3,E3,F3,G3,H3,NO},{B2,C1,NO,NO,NO,NO,NO,NO},
{A2,A1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B3 */
{{C4,D5,E6,F7,G8,NO,NO,NO},{B4,B5,B6,B7,B8,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{C3,D3,E3,F3,G3,H3,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{C2,D1,NO,NO,NO,NO,NO,NO},
{B2,B1,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C3 */
{{D4,E5,F6,G7,H8,NO,NO,NO},{C4,C5,C6,C7,C8,NO,NO,NO},
{B4,A5,NO,NO,NO,NO,NO,NO},{D3,E3,F3,G3,H3,NO,NO,NO},
{B3,A3,NO,NO,NO,NO,NO,NO},{D2,E1,NO,NO,NO,NO,NO,NO},
{C2,C1,NO,NO,NO,NO,NO,NO},{B2,A1,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D3 */
{{E4,F5,G6,H7,NO,NO,NO,NO},{D4,D5,D6,D7,D8,NO,NO,NO},
{C4,B5,A6,NO,NO,NO,NO,NO},{E3,F3,G3,H3,NO,NO,NO,NO},
{C3,B3,A3,NO,NO,NO,NO,NO},{E2,F1,NO,NO,NO,NO,NO,NO},
{D2,D1,NO,NO,NO,NO,NO,NO},{C2,B1,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E3 */
{{D4,C5,B6,A7,NO,NO,NO,NO},{E4,E5,E6,E7,E8,NO,NO,NO},
{F4,G5,H6,NO,NO,NO,NO,NO},{D3,C3,B3,A3,NO,NO,NO,NO},
{F3,G3,H3,NO,NO,NO,NO,NO},{D2,C1,NO,NO,NO,NO,NO,NO},
{E2,E1,NO,NO,NO,NO,NO,NO},{F2,G1,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F3 */
{{E4,D5,C6,B7,A8,NO,NO,NO},{F4,F5,F6,F7,F8,NO,NO,NO},
{G4,H5,NO,NO,NO,NO,NO,NO},{E3,D3,C3,B3,A3,NO,NO,NO},
{G3,H3,NO,NO,NO,NO,NO,NO},{E2,D1,NO,NO,NO,NO,NO,NO},
{F2,F1,NO,NO,NO,NO,NO,NO},{G2,H1,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G3 */
{{F4,E5,D6,C7,B8,NO,NO,NO},{G4,G5,G6,G7,G8,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{F3,E3,D3,C3,B3,A3,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{F2,E1,NO,NO,NO,NO,NO,NO},
{G2,G1,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H3 */
{{G4,F5,E6,D7,C8,NO,NO,NO},{H4,H5,H6,H7,H8,NO,NO,NO},
{G3,F3,E3,D3,C3,B3,A3,NO},{G2,F1,NO,NO,NO,NO,NO,NO},
{H2,H1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A4 */
{{B5,C6,D7,E8,NO,NO,NO,NO},{A5,A6,A7,A8,NO,NO,NO,NO},
{B4,C4,D4,E4,F4,G4,H4,NO},{B3,C2,D1,NO,NO,NO,NO,NO},
{A3,A2,A1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B4 */
{{C5,D6,E7,F8,NO,NO,NO,NO},{B5,B6,B7,B8,NO,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{C4,D4,E4,F4,G4,H4,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{C3,D2,E1,NO,NO,NO,NO,NO},
{B3,B2,B1,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C4 */
{{D5,E6,F7,G8,NO,NO,NO,NO},{C5,C6,C7,C8,NO,NO,NO,NO},
{B5,A6,NO,NO,NO,NO,NO,NO},{D4,E4,F4,G4,H4,NO,NO,NO},
{B4,A4,NO,NO,NO,NO,NO,NO},{D3,E2,F1,NO,NO,NO,NO,NO},
{C3,C2,C1,NO,NO,NO,NO,NO},{B3,A2,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D4 */
{{E5,F6,G7,H8,NO,NO,NO,NO},{D5,D6,D7,D8,NO,NO,NO,NO},
{C5,B6,A7,NO,NO,NO,NO,NO},{E4,F4,G4,H4,NO,NO,NO,NO},
{C4,B4,A4,NO,NO,NO,NO,NO},{E3,F2,G1,NO,NO,NO,NO,NO},
{D3,D2,D1,NO,NO,NO,NO,NO},{C3,B2,A1,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E4 */
{{D5,C6,B7,A8,NO,NO,NO,NO},{E5,E6,E7,E8,NO,NO,NO,NO},
{F5,G6,H7,NO,NO,NO,NO,NO},{D4,C4,B4,A4,NO,NO,NO,NO},
{F4,G4,H4,NO,NO,NO,NO,NO},{D3,C2,B1,NO,NO,NO,NO,NO},
{E3,E2,E1,NO,NO,NO,NO,NO},{F3,G2,H1,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F4 */
{{E5,D6,C7,B8,NO,NO,NO,NO},{F5,F6,F7,F8,NO,NO,NO,NO},
{G5,H6,NO,NO,NO,NO,NO,NO},{E4,D4,C4,B4,A4,NO,NO,NO},
{G4,H4,NO,NO,NO,NO,NO,NO},{E3,D2,C1,NO,NO,NO,NO,NO},
{F3,F2,F1,NO,NO,NO,NO,NO},{G3,H2,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G4 */
{{F5,E6,D7,C8,NO,NO,NO,NO},{G5,G6,G7,G8,NO,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{F4,E4,D4,C4,B4,A4,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{F3,E2,D1,NO,NO,NO,NO,NO},
{G3,G2,G1,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H4 */
{{G5,F6,E7,D8,NO,NO,NO,NO},{H5,H6,H7,H8,NO,NO,NO,NO},
{G4,F4,E4,D4,C4,B4,A4,NO},{G3,F2,E1,NO,NO,NO,NO,NO},
{H3,H2,H1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A5 */
{{B6,C7,D8,NO,NO,NO,NO,NO},{A6,A7,A8,NO,NO,NO,NO,NO},
{B5,C5,D5,E5,F5,G5,H5,NO},{B4,C3,D2,E1,NO,NO,NO,NO},
{A4,A3,A2,A1,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B5 */
{{C6,D7,E8,NO,NO,NO,NO,NO},{B6,B7,B8,NO,NO,NO,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{C5,D5,E5,F5,G5,H5,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{C4,D3,E2,F1,NO,NO,NO,NO},
{B4,B3,B2,B1,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C5 */
{{D6,E7,F8,NO,NO,NO,NO,NO},{C6,C7,C8,NO,NO,NO,NO,NO},
{B6,A7,NO,NO,NO,NO,NO,NO},{D5,E5,F5,G5,H5,NO,NO,NO},
{B5,A5,NO,NO,NO,NO,NO,NO},{D4,E3,F2,G1,NO,NO,NO,NO},
{C4,C3,C2,C1,NO,NO,NO,NO},{B4,A3,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D5 */
{{E6,F7,G8,NO,NO,NO,NO,NO},{D6,D7,D8,NO,NO,NO,NO,NO},
{C6,B7,A8,NO,NO,NO,NO,NO},{E5,F5,G5,H5,NO,NO,NO,NO},
{C5,B5,A5,NO,NO,NO,NO,NO},{E4,F3,G2,H1,NO,NO,NO,NO},
{D4,D3,D2,D1,NO,NO,NO,NO},{C4,B3,A2,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E5 */
{{D6,C7,B8,NO,NO,NO,NO,NO},{E6,E7,E8,NO,NO,NO,NO,NO},
{F6,G7,H8,NO,NO,NO,NO,NO},{D5,C5,B5,A5,NO,NO,NO,NO},
{F5,G5,H5,NO,NO,NO,NO,NO},{D4,C3,B2,A1,NO,NO,NO,NO},
{E4,E3,E2,E1,NO,NO,NO,NO},{F4,G3,H2,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F5 */
{{E6,D7,C8,NO,NO,NO,NO,NO},{F6,F7,F8,NO,NO,NO,NO,NO},
{G6,H7,NO,NO,NO,NO,NO,NO},{E5,D5,C5,B5,A5,NO,NO,NO},
{G5,H5,NO,NO,NO,NO,NO,NO},{E4,D3,C2,B1,NO,NO,NO,NO},
{F4,F3,F2,F1,NO,NO,NO,NO},{G4,H3,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G5 */
{{F6,E7,D8,NO,NO,NO,NO,NO},{G6,G7,G8,NO,NO,NO,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{F5,E5,D5,C5,B5,A5,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{F4,E3,D2,C1,NO,NO,NO,NO},
{G4,G3,G2,G1,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H5 */
{{G6,F7,E8,NO,NO,NO,NO,NO},{H6,H7,H8,NO,NO,NO,NO,NO},
{G5,F5,E5,D5,C5,B5,A5,NO},{G4,F3,E2,D1,NO,NO,NO,NO},
{H4,H3,H2,H1,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A6 */
{{B7,C8,NO,NO,NO,NO,NO,NO},{A7,A8,NO,NO,NO,NO,NO,NO},
{B6,C6,D6,E6,F6,G6,H6,NO},{B5,C4,D3,E2,F1,NO,NO,NO},
{A5,A4,A3,A2,A1,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B6 */
{{C7,D8,NO,NO,NO,NO,NO,NO},{B7,B8,NO,NO,NO,NO,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C6,D6,E6,F6,G6,H6,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{C5,D4,E3,F2,G1,NO,NO,NO},
{B5,B4,B3,B2,B1,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C6 */
{{D7,E8,NO,NO,NO,NO,NO,NO},{C7,C8,NO,NO,NO,NO,NO,NO},
{B7,A8,NO,NO,NO,NO,NO,NO},{D6,E6,F6,G6,H6,NO,NO,NO},
{B6,A6,NO,NO,NO,NO,NO,NO},{D5,E4,F3,G2,H1,NO,NO,NO},
{C5,C4,C3,C2,C1,NO,NO,NO},{B5,A4,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D6 */
{{E7,F8,NO,NO,NO,NO,NO,NO},{D7,D8,NO,NO,NO,NO,NO,NO},
{C7,B8,NO,NO,NO,NO,NO,NO},{E6,F6,G6,H6,NO,NO,NO,NO},
{C6,B6,A6,NO,NO,NO,NO,NO},{E5,F4,G3,H2,NO,NO,NO,NO},
{D5,D4,D3,D2,D1,NO,NO,NO},{C5,B4,A3,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E6 */
{{D7,C8,NO,NO,NO,NO,NO,NO},{E7,E8,NO,NO,NO,NO,NO,NO},
{F7,G8,NO,NO,NO,NO,NO,NO},{D6,C6,B6,A6,NO,NO,NO,NO},
{F6,G6,H6,NO,NO,NO,NO,NO},{D5,C4,B3,A2,NO,NO,NO,NO},
{E5,E4,E3,E2,E1,NO,NO,NO},{F5,G4,H3,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F6 */
{{E7,D8,NO,NO,NO,NO,NO,NO},{F7,F8,NO,NO,NO,NO,NO,NO},
{G7,H8,NO,NO,NO,NO,NO,NO},{E6,D6,C6,B6,A6,NO,NO,NO},
{G6,H6,NO,NO,NO,NO,NO,NO},{E5,D4,C3,B2,A1,NO,NO,NO},
{F5,F4,F3,F2,F1,NO,NO,NO},{G5,H4,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G6 */
{{F7,E8,NO,NO,NO,NO,NO,NO},{G7,G8,NO,NO,NO,NO,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F6,E6,D6,C6,B6,A6,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{F5,E4,D3,C2,B1,NO,NO,NO},
{G5,G4,G3,G2,G1,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H6 */
{{G7,F8,NO,NO,NO,NO,NO,NO},{H7,H8,NO,NO,NO,NO,NO,NO},
{G6,F6,E6,D6,C6,B6,A6,NO},{G5,F4,E3,D2,C1,NO,NO,NO},
{H5,H4,H3,H2,H1,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A7 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{B7,C7,D7,E7,F7,G7,H7,NO},{B6,C5,D4,E3,F2,G1,NO,NO},
{A6,A5,A4,A3,A2,A1,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{C7,D7,E7,F7,G7,H7,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C6,D5,E4,F3,G2,H1,NO,NO},
{B6,B5,B4,B3,B2,B1,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{D7,E7,F7,G7,H7,NO,NO,NO},
{B7,A7,NO,NO,NO,NO,NO,NO},{D6,E5,F4,G3,H2,NO,NO,NO},
{C6,C5,C4,C3,C2,C1,NO,NO},{B6,A5,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{E7,F7,G7,H7,NO,NO,NO,NO},
{C7,B7,A7,NO,NO,NO,NO,NO},{E6,F5,G4,H3,NO,NO,NO,NO},
{D6,D5,D4,D3,D2,D1,NO,NO},{C6,B5,A4,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{D7,C7,B7,A7,NO,NO,NO,NO},
{F7,G7,H7,NO,NO,NO,NO,NO},{D6,C5,B4,A3,NO,NO,NO,NO},
{E6,E5,E4,E3,E2,E1,NO,NO},{F6,G5,H4,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{E7,D7,C7,B7,A7,NO,NO,NO},
{G7,H7,NO,NO,NO,NO,NO,NO},{E6,D5,C4,B3,A2,NO,NO,NO},
{F6,F5,F4,F3,F2,F1,NO,NO},{G6,H5,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{F7,E7,D7,C7,B7,A7,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F6,E5,D4,C3,B2,A1,NO,NO},
{G6,G5,G4,G3,G2,G1,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H7 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{G7,F7,E7,D7,C7,B7,A7,NO},{G6,F5,E4,D3,C2,B1,NO,NO},
{H6,H5,H4,H3,H2,H1,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN A8 */
{{B8,C8,D8,E8,F8,G8,H8,NO},{B7,C6,D5,E4,F3,G2,H1,NO},
{A7,A6,A5,A4,A3,A2,A1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN B8 */
{{C8,D8,E8,F8,G8,H8,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{C7,D6,E5,F4,G3,H2,NO,NO},{B7,B6,B5,B4,B3,B2,B1,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN C8 */
{{D8,E8,F8,G8,H8,NO,NO,NO},{B8,A8,NO,NO,NO,NO,NO,NO},
{D7,E6,F5,G4,H3,NO,NO,NO},{C7,C6,C5,C4,C3,C2,C1,NO},
{B7,A6,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN D8 */
{{E8,F8,G8,H8,NO,NO,NO,NO},{C8,B8,A8,NO,NO,NO,NO,NO},
{E7,F6,G5,H4,NO,NO,NO,NO},{D7,D6,D5,D4,D3,D2,D1,NO},
{C7,B6,A5,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN E8 */
{{D8,C8,B8,A8,NO,NO,NO,NO},{F8,G8,H8,NO,NO,NO,NO,NO},
{D7,C6,B5,A4,NO,NO,NO,NO},{E7,E6,E5,E4,E3,E2,E1,NO},
{F7,G6,H5,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN F8 */
{{E8,D8,C8,B8,A8,NO,NO,NO},{G8,H8,NO,NO,NO,NO,NO,NO},
{E7,D6,C5,B4,A3,NO,NO,NO},{F7,F6,F5,F4,F3,F2,F1,NO},
{G7,H6,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN G8 */
{{F8,E8,D8,C8,B8,A8,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{F7,E6,D5,C4,B3,A2,NO,NO},{G7,G6,G5,G4,G3,G2,G1,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_QUEEN H8 */
{{G8,F8,E8,D8,C8,B8,A8,NO},{G7,F6,E5,D4,C3,B2,A1,NO},
{H7,H6,H5,H4,H3,H2,H1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_QUEEN */
/* begin BLACK_QUEEN */
{/* BLACK_QUEEN A1 */
{{B1,C1,D1,E1,F1,G1,H1,NO},{B2,C3,D4,E5,F6,G7,H8,NO},
{A2,A3,A4,A5,A6,A7,A8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B1 */
{{C1,D1,E1,F1,G1,H1,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{C2,D3,E4,F5,G6,H7,NO,NO},{B2,B3,B4,B5,B6,B7,B8,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C1 */
{{D1,E1,F1,G1,H1,NO,NO,NO},{B1,A1,NO,NO,NO,NO,NO,NO},
{D2,E3,F4,G5,H6,NO,NO,NO},{C2,C3,C4,C5,C6,C7,C8,NO},
{B2,A3,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D1 */
{{E1,F1,G1,H1,NO,NO,NO,NO},{C1,B1,A1,NO,NO,NO,NO,NO},
{E2,F3,G4,H5,NO,NO,NO,NO},{D2,D3,D4,D5,D6,D7,D8,NO},
{C2,B3,A4,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E1 */
{{D1,C1,B1,A1,NO,NO,NO,NO},{F1,G1,H1,NO,NO,NO,NO,NO},
{D2,C3,B4,A5,NO,NO,NO,NO},{E2,E3,E4,E5,E6,E7,E8,NO},
{F2,G3,H4,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F1 */
{{E1,D1,C1,B1,A1,NO,NO,NO},{G1,H1,NO,NO,NO,NO,NO,NO},
{E2,D3,C4,B5,A6,NO,NO,NO},{F2,F3,F4,F5,F6,F7,F8,NO},
{G2,H3,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G1 */
{{F1,E1,D1,C1,B1,A1,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{F2,E3,D4,C5,B6,A7,NO,NO},{G2,G3,G4,G5,G6,G7,G8,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H1 */
{{G1,F1,E1,D1,C1,B1,A1,NO},{G2,F3,E4,D5,C6,B7,A8,NO},
{H2,H3,H4,H5,H6,H7,H8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A2 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{B2,C2,D2,E2,F2,G2,H2,NO},{B3,C4,D5,E6,F7,G8,NO,NO},
{A3,A4,A5,A6,A7,A8,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{C2,D2,E2,F2,G2,H2,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C3,D4,E5,F6,G7,H8,NO,NO},
{B3,B4,B5,B6,B7,B8,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{B1,NO,NO,NO,NO,NO,NO,NO},{D2,E2,F2,G2,H2,NO,NO,NO},
{B2,A2,NO,NO,NO,NO,NO,NO},{D3,E4,F5,G6,H7,NO,NO,NO},
{C3,C4,C5,C6,C7,C8,NO,NO},{B3,A4,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{E2,F2,G2,H2,NO,NO,NO,NO},
{C2,B2,A2,NO,NO,NO,NO,NO},{E3,F4,G5,H6,NO,NO,NO,NO},
{D3,D4,D5,D6,D7,D8,NO,NO},{C3,B4,A5,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{D2,C2,B2,A2,NO,NO,NO,NO},
{F2,G2,H2,NO,NO,NO,NO,NO},{D3,C4,B5,A6,NO,NO,NO,NO},
{E3,E4,E5,E6,E7,E8,NO,NO},{F3,G4,H5,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{G1,NO,NO,NO,NO,NO,NO,NO},{E2,D2,C2,B2,A2,NO,NO,NO},
{G2,H2,NO,NO,NO,NO,NO,NO},{E3,D4,C5,B6,A7,NO,NO,NO},
{F3,F4,F5,F6,F7,F8,NO,NO},{G3,H4,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{F2,E2,D2,C2,B2,A2,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F3,E4,D5,C6,B7,A8,NO,NO},
{G3,G4,G5,G6,G7,G8,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H2 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{G2,F2,E2,D2,C2,B2,A2,NO},{G3,F4,E5,D6,C7,B8,NO,NO},
{H3,H4,H5,H6,H7,H8,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A3 */
{{B2,C1,NO,NO,NO,NO,NO,NO},{A2,A1,NO,NO,NO,NO,NO,NO},
{B3,C3,D3,E3,F3,G3,H3,NO},{B4,C5,D6,E7,F8,NO,NO,NO},
{A4,A5,A6,A7,A8,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B3 */
{{C2,D1,NO,NO,NO,NO,NO,NO},{B2,B1,NO,NO,NO,NO,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{C3,D3,E3,F3,G3,H3,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{C4,D5,E6,F7,G8,NO,NO,NO},
{B4,B5,B6,B7,B8,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C3 */
{{D2,E1,NO,NO,NO,NO,NO,NO},{C2,C1,NO,NO,NO,NO,NO,NO},
{B2,A1,NO,NO,NO,NO,NO,NO},{D3,E3,F3,G3,H3,NO,NO,NO},
{B3,A3,NO,NO,NO,NO,NO,NO},{D4,E5,F6,G7,H8,NO,NO,NO},
{C4,C5,C6,C7,C8,NO,NO,NO},{B4,A5,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D3 */
{{E2,F1,NO,NO,NO,NO,NO,NO},{D2,D1,NO,NO,NO,NO,NO,NO},
{C2,B1,NO,NO,NO,NO,NO,NO},{E3,F3,G3,H3,NO,NO,NO,NO},
{C3,B3,A3,NO,NO,NO,NO,NO},{E4,F5,G6,H7,NO,NO,NO,NO},
{D4,D5,D6,D7,D8,NO,NO,NO},{C4,B5,A6,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E3 */
{{D2,C1,NO,NO,NO,NO,NO,NO},{E2,E1,NO,NO,NO,NO,NO,NO},
{F2,G1,NO,NO,NO,NO,NO,NO},{D3,C3,B3,A3,NO,NO,NO,NO},
{F3,G3,H3,NO,NO,NO,NO,NO},{D4,C5,B6,A7,NO,NO,NO,NO},
{E4,E5,E6,E7,E8,NO,NO,NO},{F4,G5,H6,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F3 */
{{E2,D1,NO,NO,NO,NO,NO,NO},{F2,F1,NO,NO,NO,NO,NO,NO},
{G2,H1,NO,NO,NO,NO,NO,NO},{E3,D3,C3,B3,A3,NO,NO,NO},
{G3,H3,NO,NO,NO,NO,NO,NO},{E4,D5,C6,B7,A8,NO,NO,NO},
{F4,F5,F6,F7,F8,NO,NO,NO},{G4,H5,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G3 */
{{F2,E1,NO,NO,NO,NO,NO,NO},{G2,G1,NO,NO,NO,NO,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{F3,E3,D3,C3,B3,A3,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{F4,E5,D6,C7,B8,NO,NO,NO},
{G4,G5,G6,G7,G8,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H3 */
{{G2,F1,NO,NO,NO,NO,NO,NO},{H2,H1,NO,NO,NO,NO,NO,NO},
{G3,F3,E3,D3,C3,B3,A3,NO},{G4,F5,E6,D7,C8,NO,NO,NO},
{H4,H5,H6,H7,H8,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A4 */
{{B3,C2,D1,NO,NO,NO,NO,NO},{A3,A2,A1,NO,NO,NO,NO,NO},
{B4,C4,D4,E4,F4,G4,H4,NO},{B5,C6,D7,E8,NO,NO,NO,NO},
{A5,A6,A7,A8,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B4 */
{{C3,D2,E1,NO,NO,NO,NO,NO},{B3,B2,B1,NO,NO,NO,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{C4,D4,E4,F4,G4,H4,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{C5,D6,E7,F8,NO,NO,NO,NO},
{B5,B6,B7,B8,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C4 */
{{D3,E2,F1,NO,NO,NO,NO,NO},{C3,C2,C1,NO,NO,NO,NO,NO},
{B3,A2,NO,NO,NO,NO,NO,NO},{D4,E4,F4,G4,H4,NO,NO,NO},
{B4,A4,NO,NO,NO,NO,NO,NO},{D5,E6,F7,G8,NO,NO,NO,NO},
{C5,C6,C7,C8,NO,NO,NO,NO},{B5,A6,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D4 */
{{E3,F2,G1,NO,NO,NO,NO,NO},{D3,D2,D1,NO,NO,NO,NO,NO},
{C3,B2,A1,NO,NO,NO,NO,NO},{E4,F4,G4,H4,NO,NO,NO,NO},
{C4,B4,A4,NO,NO,NO,NO,NO},{E5,F6,G7,H8,NO,NO,NO,NO},
{D5,D6,D7,D8,NO,NO,NO,NO},{C5,B6,A7,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E4 */
{{D3,C2,B1,NO,NO,NO,NO,NO},{E3,E2,E1,NO,NO,NO,NO,NO},
{F3,G2,H1,NO,NO,NO,NO,NO},{D4,C4,B4,A4,NO,NO,NO,NO},
{F4,G4,H4,NO,NO,NO,NO,NO},{D5,C6,B7,A8,NO,NO,NO,NO},
{E5,E6,E7,E8,NO,NO,NO,NO},{F5,G6,H7,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F4 */
{{E3,D2,C1,NO,NO,NO,NO,NO},{F3,F2,F1,NO,NO,NO,NO,NO},
{G3,H2,NO,NO,NO,NO,NO,NO},{E4,D4,C4,B4,A4,NO,NO,NO},
{G4,H4,NO,NO,NO,NO,NO,NO},{E5,D6,C7,B8,NO,NO,NO,NO},
{F5,F6,F7,F8,NO,NO,NO,NO},{G5,H6,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G4 */
{{F3,E2,D1,NO,NO,NO,NO,NO},{G3,G2,G1,NO,NO,NO,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{F4,E4,D4,C4,B4,A4,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{F5,E6,D7,C8,NO,NO,NO,NO},
{G5,G6,G7,G8,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H4 */
{{G3,F2,E1,NO,NO,NO,NO,NO},{H3,H2,H1,NO,NO,NO,NO,NO},
{G4,F4,E4,D4,C4,B4,A4,NO},{G5,F6,E7,D8,NO,NO,NO,NO},
{H5,H6,H7,H8,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A5 */
{{B4,C3,D2,E1,NO,NO,NO,NO},{A4,A3,A2,A1,NO,NO,NO,NO},
{B5,C5,D5,E5,F5,G5,H5,NO},{B6,C7,D8,NO,NO,NO,NO,NO},
{A6,A7,A8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B5 */
{{C4,D3,E2,F1,NO,NO,NO,NO},{B4,B3,B2,B1,NO,NO,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{C5,D5,E5,F5,G5,H5,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{C6,D7,E8,NO,NO,NO,NO,NO},
{B6,B7,B8,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C5 */
{{D4,E3,F2,G1,NO,NO,NO,NO},{C4,C3,C2,C1,NO,NO,NO,NO},
{B4,A3,NO,NO,NO,NO,NO,NO},{D5,E5,F5,G5,H5,NO,NO,NO},
{B5,A5,NO,NO,NO,NO,NO,NO},{D6,E7,F8,NO,NO,NO,NO,NO},
{C6,C7,C8,NO,NO,NO,NO,NO},{B6,A7,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D5 */
{{E4,F3,G2,H1,NO,NO,NO,NO},{D4,D3,D2,D1,NO,NO,NO,NO},
{C4,B3,A2,NO,NO,NO,NO,NO},{E5,F5,G5,H5,NO,NO,NO,NO},
{C5,B5,A5,NO,NO,NO,NO,NO},{E6,F7,G8,NO,NO,NO,NO,NO},
{D6,D7,D8,NO,NO,NO,NO,NO},{C6,B7,A8,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E5 */
{{D4,C3,B2,A1,NO,NO,NO,NO},{E4,E3,E2,E1,NO,NO,NO,NO},
{F4,G3,H2,NO,NO,NO,NO,NO},{D5,C5,B5,A5,NO,NO,NO,NO},
{F5,G5,H5,NO,NO,NO,NO,NO},{D6,C7,B8,NO,NO,NO,NO,NO},
{E6,E7,E8,NO,NO,NO,NO,NO},{F6,G7,H8,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F5 */
{{E4,D3,C2,B1,NO,NO,NO,NO},{F4,F3,F2,F1,NO,NO,NO,NO},
{G4,H3,NO,NO,NO,NO,NO,NO},{E5,D5,C5,B5,A5,NO,NO,NO},
{G5,H5,NO,NO,NO,NO,NO,NO},{E6,D7,C8,NO,NO,NO,NO,NO},
{F6,F7,F8,NO,NO,NO,NO,NO},{G6,H7,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G5 */
{{F4,E3,D2,C1,NO,NO,NO,NO},{G4,G3,G2,G1,NO,NO,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{F5,E5,D5,C5,B5,A5,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{F6,E7,D8,NO,NO,NO,NO,NO},
{G6,G7,G8,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H5 */
{{G4,F3,E2,D1,NO,NO,NO,NO},{H4,H3,H2,H1,NO,NO,NO,NO},
{G5,F5,E5,D5,C5,B5,A5,NO},{G6,F7,E8,NO,NO,NO,NO,NO},
{H6,H7,H8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A6 */
{{B5,C4,D3,E2,F1,NO,NO,NO},{A5,A4,A3,A2,A1,NO,NO,NO},
{B6,C6,D6,E6,F6,G6,H6,NO},{B7,C8,NO,NO,NO,NO,NO,NO},
{A7,A8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B6 */
{{C5,D4,E3,F2,G1,NO,NO,NO},{B5,B4,B3,B2,B1,NO,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{C6,D6,E6,F6,G6,H6,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{C7,D8,NO,NO,NO,NO,NO,NO},
{B7,B8,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C6 */
{{D5,E4,F3,G2,H1,NO,NO,NO},{C5,C4,C3,C2,C1,NO,NO,NO},
{B5,A4,NO,NO,NO,NO,NO,NO},{D6,E6,F6,G6,H6,NO,NO,NO},
{B6,A6,NO,NO,NO,NO,NO,NO},{D7,E8,NO,NO,NO,NO,NO,NO},
{C7,C8,NO,NO,NO,NO,NO,NO},{B7,A8,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D6 */
{{E5,F4,G3,H2,NO,NO,NO,NO},{D5,D4,D3,D2,D1,NO,NO,NO},
{C5,B4,A3,NO,NO,NO,NO,NO},{E6,F6,G6,H6,NO,NO,NO,NO},
{C6,B6,A6,NO,NO,NO,NO,NO},{E7,F8,NO,NO,NO,NO,NO,NO},
{D7,D8,NO,NO,NO,NO,NO,NO},{C7,B8,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E6 */
{{D5,C4,B3,A2,NO,NO,NO,NO},{E5,E4,E3,E2,E1,NO,NO,NO},
{F5,G4,H3,NO,NO,NO,NO,NO},{D6,C6,B6,A6,NO,NO,NO,NO},
{F6,G6,H6,NO,NO,NO,NO,NO},{D7,C8,NO,NO,NO,NO,NO,NO},
{E7,E8,NO,NO,NO,NO,NO,NO},{F7,G8,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F6 */
{{E5,D4,C3,B2,A1,NO,NO,NO},{F5,F4,F3,F2,F1,NO,NO,NO},
{G5,H4,NO,NO,NO,NO,NO,NO},{E6,D6,C6,B6,A6,NO,NO,NO},
{G6,H6,NO,NO,NO,NO,NO,NO},{E7,D8,NO,NO,NO,NO,NO,NO},
{F7,F8,NO,NO,NO,NO,NO,NO},{G7,H8,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G6 */
{{F5,E4,D3,C2,B1,NO,NO,NO},{G5,G4,G3,G2,G1,NO,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{F6,E6,D6,C6,B6,A6,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{F7,E8,NO,NO,NO,NO,NO,NO},
{G7,G8,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H6 */
{{G5,F4,E3,D2,C1,NO,NO,NO},{H5,H4,H3,H2,H1,NO,NO,NO},
{G6,F6,E6,D6,C6,B6,A6,NO},{G7,F8,NO,NO,NO,NO,NO,NO},
{H7,H8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A7 */
{{B6,C5,D4,E3,F2,G1,NO,NO},{A6,A5,A4,A3,A2,A1,NO,NO},
{B7,C7,D7,E7,F7,G7,H7,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B7 */
{{C6,D5,E4,F3,G2,H1,NO,NO},{B6,B5,B4,B3,B2,B1,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{C7,D7,E7,F7,G7,H7,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{B8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C7 */
{{D6,E5,F4,G3,H2,NO,NO,NO},{C6,C5,C4,C3,C2,C1,NO,NO},
{B6,A5,NO,NO,NO,NO,NO,NO},{D7,E7,F7,G7,H7,NO,NO,NO},
{B7,A7,NO,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D7 */
{{E6,F5,G4,H3,NO,NO,NO,NO},{D6,D5,D4,D3,D2,D1,NO,NO},
{C6,B5,A4,NO,NO,NO,NO,NO},{E7,F7,G7,H7,NO,NO,NO,NO},
{C7,B7,A7,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{D8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E7 */
{{D6,C5,B4,A3,NO,NO,NO,NO},{E6,E5,E4,E3,E2,E1,NO,NO},
{F6,G5,H4,NO,NO,NO,NO,NO},{D7,C7,B7,A7,NO,NO,NO,NO},
{F7,G7,H7,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{E8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F7 */
{{E6,D5,C4,B3,A2,NO,NO,NO},{F6,F5,F4,F3,F2,F1,NO,NO},
{G6,H5,NO,NO,NO,NO,NO,NO},{E7,D7,C7,B7,A7,NO,NO,NO},
{G7,H7,NO,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G7 */
{{F6,E5,D4,C3,B2,A1,NO,NO},{G6,G5,G4,G3,G2,G1,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{F7,E7,D7,C7,B7,A7,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{G8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H7 */
{{G6,F5,E4,D3,C2,B1,NO,NO},{H6,H5,H4,H3,H2,H1,NO,NO},
{G7,F7,E7,D7,C7,B7,A7,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN A8 */
{{B7,C6,D5,E4,F3,G2,H1,NO},{A7,A6,A5,A4,A3,A2,A1,NO},
{B8,C8,D8,E8,F8,G8,H8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN B8 */
{{C7,D6,E5,F4,G3,H2,NO,NO},{B7,B6,B5,B4,B3,B2,B1,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{C8,D8,E8,F8,G8,H8,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN C8 */
{{D7,E6,F5,G4,H3,NO,NO,NO},{C7,C6,C5,C4,C3,C2,C1,NO},
{B7,A6,NO,NO,NO,NO,NO,NO},{D8,E8,F8,G8,H8,NO,NO,NO},
{B8,A8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN D8 */
{{E7,F6,G5,H4,NO,NO,NO,NO},{D7,D6,D5,D4,D3,D2,D1,NO},
{C7,B6,A5,NO,NO,NO,NO,NO},{E8,F8,G8,H8,NO,NO,NO,NO},
{C8,B8,A8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN E8 */
{{D7,C6,B5,A4,NO,NO,NO,NO},{E7,E6,E5,E4,E3,E2,E1,NO},
{F7,G6,H5,NO,NO,NO,NO,NO},{D8,C8,B8,A8,NO,NO,NO,NO},
{F8,G8,H8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN F8 */
{{E7,D6,C5,B4,A3,NO,NO,NO},{F7,F6,F5,F4,F3,F2,F1,NO},
{G7,H6,NO,NO,NO,NO,NO,NO},{E8,D8,C8,B8,A8,NO,NO,NO},
{G8,H8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN G8 */
{{F7,E6,D5,C4,B3,A2,NO,NO},{G7,G6,G5,G4,G3,G2,G1,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{F8,E8,D8,C8,B8,A8,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_QUEEN H8 */
{{G7,F6,E5,D4,C3,B2,A1,NO},{H7,H6,H5,H4,H3,H2,H1,NO},
{G8,F8,E8,D8,C8,B8,A8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end BLACK_QUEEN */
/* begin WHITE_ROOK */
{/* WHITE_ROOK A1 */
{{A2,A3,A4,A5,A6,A7,A8,NO},{B1,C1,D1,E1,F1,G1,H1,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B1 */
{{B2,B3,B4,B5,B6,B7,B8,NO},{C1,D1,E1,F1,G1,H1,NO,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C1 */
{{C2,C3,C4,C5,C6,C7,C8,NO},{D1,E1,F1,G1,H1,NO,NO,NO},
{B1,A1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D1 */
{{D2,D3,D4,D5,D6,D7,D8,NO},{E1,F1,G1,H1,NO,NO,NO,NO},
{C1,B1,A1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E1 */
{{E2,E3,E4,E5,E6,E7,E8,NO},{D1,C1,B1,A1,NO,NO,NO,NO},
{F1,G1,H1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F1 */
{{F2,F3,F4,F5,F6,F7,F8,NO},{E1,D1,C1,B1,A1,NO,NO,NO},
{G1,H1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G1 */
{{G2,G3,G4,G5,G6,G7,G8,NO},{F1,E1,D1,C1,B1,A1,NO,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H1 */
{{H2,H3,H4,H5,H6,H7,H8,NO},{G1,F1,E1,D1,C1,B1,A1,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A2 */
{{A3,A4,A5,A6,A7,A8,NO,NO},{B2,C2,D2,E2,F2,G2,H2,NO},
{A1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B2 */
{{B3,B4,B5,B6,B7,B8,NO,NO},{C2,D2,E2,F2,G2,H2,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C2 */
{{C3,C4,C5,C6,C7,C8,NO,NO},{D2,E2,F2,G2,H2,NO,NO,NO},
{B2,A2,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D2 */
{{D3,D4,D5,D6,D7,D8,NO,NO},{E2,F2,G2,H2,NO,NO,NO,NO},
{C2,B2,A2,NO,NO,NO,NO,NO},{D1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E2 */
{{E3,E4,E5,E6,E7,E8,NO,NO},{D2,C2,B2,A2,NO,NO,NO,NO},
{F2,G2,H2,NO,NO,NO,NO,NO},{E1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F2 */
{{F3,F4,F5,F6,F7,F8,NO,NO},{E2,D2,C2,B2,A2,NO,NO,NO},
{G2,H2,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G2 */
{{G3,G4,G5,G6,G7,G8,NO,NO},{F2,E2,D2,C2,B2,A2,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H2 */
{{H3,H4,H5,H6,H7,H8,NO,NO},{G2,F2,E2,D2,C2,B2,A2,NO},
{H1,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A3 */
{{A4,A5,A6,A7,A8,NO,NO,NO},{B3,C3,D3,E3,F3,G3,H3,NO},
{A2,A1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B3 */
{{B4,B5,B6,B7,B8,NO,NO,NO},{C3,D3,E3,F3,G3,H3,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{B2,B1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C3 */
{{C4,C5,C6,C7,C8,NO,NO,NO},{D3,E3,F3,G3,H3,NO,NO,NO},
{B3,A3,NO,NO,NO,NO,NO,NO},{C2,C1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D3 */
{{D4,D5,D6,D7,D8,NO,NO,NO},{E3,F3,G3,H3,NO,NO,NO,NO},
{C3,B3,A3,NO,NO,NO,NO,NO},{D2,D1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E3 */
{{E4,E5,E6,E7,E8,NO,NO,NO},{D3,C3,B3,A3,NO,NO,NO,NO},
{F3,G3,H3,NO,NO,NO,NO,NO},{E2,E1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F3 */
{{F4,F5,F6,F7,F8,NO,NO,NO},{E3,D3,C3,B3,A3,NO,NO,NO},
{G3,H3,NO,NO,NO,NO,NO,NO},{F2,F1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G3 */
{{G4,G5,G6,G7,G8,NO,NO,NO},{F3,E3,D3,C3,B3,A3,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{G2,G1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H3 */
{{H4,H5,H6,H7,H8,NO,NO,NO},{G3,F3,E3,D3,C3,B3,A3,NO},
{H2,H1,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A4 */
{{A5,A6,A7,A8,NO,NO,NO,NO},{B4,C4,D4,E4,F4,G4,H4,NO},
{A3,A2,A1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B4 */
{{B5,B6,B7,B8,NO,NO,NO,NO},{C4,D4,E4,F4,G4,H4,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{B3,B2,B1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C4 */
{{C5,C6,C7,C8,NO,NO,NO,NO},{D4,E4,F4,G4,H4,NO,NO,NO},
{B4,A4,NO,NO,NO,NO,NO,NO},{C3,C2,C1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D4 */
{{D5,D6,D7,D8,NO,NO,NO,NO},{E4,F4,G4,H4,NO,NO,NO,NO},
{C4,B4,A4,NO,NO,NO,NO,NO},{D3,D2,D1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E4 */
{{E5,E6,E7,E8,NO,NO,NO,NO},{D4,C4,B4,A4,NO,NO,NO,NO},
{F4,G4,H4,NO,NO,NO,NO,NO},{E3,E2,E1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F4 */
{{F5,F6,F7,F8,NO,NO,NO,NO},{E4,D4,C4,B4,A4,NO,NO,NO},
{G4,H4,NO,NO,NO,NO,NO,NO},{F3,F2,F1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G4 */
{{G5,G6,G7,G8,NO,NO,NO,NO},{F4,E4,D4,C4,B4,A4,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{G3,G2,G1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H4 */
{{H5,H6,H7,H8,NO,NO,NO,NO},{G4,F4,E4,D4,C4,B4,A4,NO},
{H3,H2,H1,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A5 */
{{A6,A7,A8,NO,NO,NO,NO,NO},{B5,C5,D5,E5,F5,G5,H5,NO},
{A4,A3,A2,A1,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B5 */
{{B6,B7,B8,NO,NO,NO,NO,NO},{C5,D5,E5,F5,G5,H5,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{B4,B3,B2,B1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C5 */
{{C6,C7,C8,NO,NO,NO,NO,NO},{D5,E5,F5,G5,H5,NO,NO,NO},
{B5,A5,NO,NO,NO,NO,NO,NO},{C4,C3,C2,C1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D5 */
{{D6,D7,D8,NO,NO,NO,NO,NO},{E5,F5,G5,H5,NO,NO,NO,NO},
{C5,B5,A5,NO,NO,NO,NO,NO},{D4,D3,D2,D1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E5 */
{{E6,E7,E8,NO,NO,NO,NO,NO},{D5,C5,B5,A5,NO,NO,NO,NO},
{F5,G5,H5,NO,NO,NO,NO,NO},{E4,E3,E2,E1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F5 */
{{F6,F7,F8,NO,NO,NO,NO,NO},{E5,D5,C5,B5,A5,NO,NO,NO},
{G5,H5,NO,NO,NO,NO,NO,NO},{F4,F3,F2,F1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G5 */
{{G6,G7,G8,NO,NO,NO,NO,NO},{F5,E5,D5,C5,B5,A5,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{G4,G3,G2,G1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H5 */
{{H6,H7,H8,NO,NO,NO,NO,NO},{G5,F5,E5,D5,C5,B5,A5,NO},
{H4,H3,H2,H1,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A6 */
{{A7,A8,NO,NO,NO,NO,NO,NO},{B6,C6,D6,E6,F6,G6,H6,NO},
{A5,A4,A3,A2,A1,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B6 */
{{B7,B8,NO,NO,NO,NO,NO,NO},{C6,D6,E6,F6,G6,H6,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{B5,B4,B3,B2,B1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C6 */
{{C7,C8,NO,NO,NO,NO,NO,NO},{D6,E6,F6,G6,H6,NO,NO,NO},
{B6,A6,NO,NO,NO,NO,NO,NO},{C5,C4,C3,C2,C1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D6 */
{{D7,D8,NO,NO,NO,NO,NO,NO},{E6,F6,G6,H6,NO,NO,NO,NO},
{C6,B6,A6,NO,NO,NO,NO,NO},{D5,D4,D3,D2,D1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E6 */
{{E7,E8,NO,NO,NO,NO,NO,NO},{D6,C6,B6,A6,NO,NO,NO,NO},
{F6,G6,H6,NO,NO,NO,NO,NO},{E5,E4,E3,E2,E1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F6 */
{{F7,F8,NO,NO,NO,NO,NO,NO},{E6,D6,C6,B6,A6,NO,NO,NO},
{G6,H6,NO,NO,NO,NO,NO,NO},{F5,F4,F3,F2,F1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G6 */
{{G7,G8,NO,NO,NO,NO,NO,NO},{F6,E6,D6,C6,B6,A6,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{G5,G4,G3,G2,G1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H6 */
{{H7,H8,NO,NO,NO,NO,NO,NO},{G6,F6,E6,D6,C6,B6,A6,NO},
{H5,H4,H3,H2,H1,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A7 */
{{A8,NO,NO,NO,NO,NO,NO,NO},{B7,C7,D7,E7,F7,G7,H7,NO},
{A6,A5,A4,A3,A2,A1,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B7 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{C7,D7,E7,F7,G7,H7,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{B6,B5,B4,B3,B2,B1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{D7,E7,F7,G7,H7,NO,NO,NO},
{B7,A7,NO,NO,NO,NO,NO,NO},{C6,C5,C4,C3,C2,C1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{E7,F7,G7,H7,NO,NO,NO,NO},
{C7,B7,A7,NO,NO,NO,NO,NO},{D6,D5,D4,D3,D2,D1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{D7,C7,B7,A7,NO,NO,NO,NO},
{F7,G7,H7,NO,NO,NO,NO,NO},{E6,E5,E4,E3,E2,E1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{E7,D7,C7,B7,A7,NO,NO,NO},
{G7,H7,NO,NO,NO,NO,NO,NO},{F6,F5,F4,F3,F2,F1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G7 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{F7,E7,D7,C7,B7,A7,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{G6,G5,G4,G3,G2,G1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H7 */
{{H8,NO,NO,NO,NO,NO,NO,NO},{G7,F7,E7,D7,C7,B7,A7,NO},
{H6,H5,H4,H3,H2,H1,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK A8 */
{{B8,C8,D8,E8,F8,G8,H8,NO},{A7,A6,A5,A4,A3,A2,A1,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK B8 */
{{C8,D8,E8,F8,G8,H8,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{B7,B6,B5,B4,B3,B2,B1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK C8 */
{{D8,E8,F8,G8,H8,NO,NO,NO},{B8,A8,NO,NO,NO,NO,NO,NO},
{C7,C6,C5,C4,C3,C2,C1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK D8 */
{{E8,F8,G8,H8,NO,NO,NO,NO},{C8,B8,A8,NO,NO,NO,NO,NO},
{D7,D6,D5,D4,D3,D2,D1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK E8 */
{{D8,C8,B8,A8,NO,NO,NO,NO},{F8,G8,H8,NO,NO,NO,NO,NO},
{E7,E6,E5,E4,E3,E2,E1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK F8 */
{{E8,D8,C8,B8,A8,NO,NO,NO},{G8,H8,NO,NO,NO,NO,NO,NO},
{F7,F6,F5,F4,F3,F2,F1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK G8 */
{{F8,E8,D8,C8,B8,A8,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{G7,G6,G5,G4,G3,G2,G1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_ROOK H8 */
{{G8,F8,E8,D8,C8,B8,A8,NO},{H7,H6,H5,H4,H3,H2,H1,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_ROOK */
/* begin BLACK_ROOK */
{/* BLACK_ROOK A1 */
{{B1,C1,D1,E1,F1,G1,H1,NO},{A2,A3,A4,A5,A6,A7,A8,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B1 */
{{C1,D1,E1,F1,G1,H1,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{B2,B3,B4,B5,B6,B7,B8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C1 */
{{D1,E1,F1,G1,H1,NO,NO,NO},{B1,A1,NO,NO,NO,NO,NO,NO},
{C2,C3,C4,C5,C6,C7,C8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D1 */
{{E1,F1,G1,H1,NO,NO,NO,NO},{C1,B1,A1,NO,NO,NO,NO,NO},
{D2,D3,D4,D5,D6,D7,D8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E1 */
{{D1,C1,B1,A1,NO,NO,NO,NO},{F1,G1,H1,NO,NO,NO,NO,NO},
{E2,E3,E4,E5,E6,E7,E8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F1 */
{{E1,D1,C1,B1,A1,NO,NO,NO},{G1,H1,NO,NO,NO,NO,NO,NO},
{F2,F3,F4,F5,F6,F7,F8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G1 */
{{F1,E1,D1,C1,B1,A1,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{G2,G3,G4,G5,G6,G7,G8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H1 */
{{G1,F1,E1,D1,C1,B1,A1,NO},{H2,H3,H4,H5,H6,H7,H8,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A2 */
{{A1,NO,NO,NO,NO,NO,NO,NO},{B2,C2,D2,E2,F2,G2,H2,NO},
{A3,A4,A5,A6,A7,A8,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B2 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{C2,D2,E2,F2,G2,H2,NO,NO},
{A2,NO,NO,NO,NO,NO,NO,NO},{B3,B4,B5,B6,B7,B8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{D2,E2,F2,G2,H2,NO,NO,NO},
{B2,A2,NO,NO,NO,NO,NO,NO},{C3,C4,C5,C6,C7,C8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{E2,F2,G2,H2,NO,NO,NO,NO},
{C2,B2,A2,NO,NO,NO,NO,NO},{D3,D4,D5,D6,D7,D8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{D2,C2,B2,A2,NO,NO,NO,NO},
{F2,G2,H2,NO,NO,NO,NO,NO},{E3,E4,E5,E6,E7,E8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{E2,D2,C2,B2,A2,NO,NO,NO},
{G2,H2,NO,NO,NO,NO,NO,NO},{F3,F4,F5,F6,F7,F8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G2 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{F2,E2,D2,C2,B2,A2,NO,NO},
{H2,NO,NO,NO,NO,NO,NO,NO},{G3,G4,G5,G6,G7,G8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H2 */
{{H1,NO,NO,NO,NO,NO,NO,NO},{G2,F2,E2,D2,C2,B2,A2,NO},
{H3,H4,H5,H6,H7,H8,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A3 */
{{A2,A1,NO,NO,NO,NO,NO,NO},{B3,C3,D3,E3,F3,G3,H3,NO},
{A4,A5,A6,A7,A8,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B3 */
{{B2,B1,NO,NO,NO,NO,NO,NO},{C3,D3,E3,F3,G3,H3,NO,NO},
{A3,NO,NO,NO,NO,NO,NO,NO},{B4,B5,B6,B7,B8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C3 */
{{C2,C1,NO,NO,NO,NO,NO,NO},{D3,E3,F3,G3,H3,NO,NO,NO},
{B3,A3,NO,NO,NO,NO,NO,NO},{C4,C5,C6,C7,C8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D3 */
{{D2,D1,NO,NO,NO,NO,NO,NO},{E3,F3,G3,H3,NO,NO,NO,NO},
{C3,B3,A3,NO,NO,NO,NO,NO},{D4,D5,D6,D7,D8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E3 */
{{E2,E1,NO,NO,NO,NO,NO,NO},{D3,C3,B3,A3,NO,NO,NO,NO},
{F3,G3,H3,NO,NO,NO,NO,NO},{E4,E5,E6,E7,E8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F3 */
{{F2,F1,NO,NO,NO,NO,NO,NO},{E3,D3,C3,B3,A3,NO,NO,NO},
{G3,H3,NO,NO,NO,NO,NO,NO},{F4,F5,F6,F7,F8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G3 */
{{G2,G1,NO,NO,NO,NO,NO,NO},{F3,E3,D3,C3,B3,A3,NO,NO},
{H3,NO,NO,NO,NO,NO,NO,NO},{G4,G5,G6,G7,G8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H3 */
{{H2,H1,NO,NO,NO,NO,NO,NO},{G3,F3,E3,D3,C3,B3,A3,NO},
{H4,H5,H6,H7,H8,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A4 */
{{A3,A2,A1,NO,NO,NO,NO,NO},{B4,C4,D4,E4,F4,G4,H4,NO},
{A5,A6,A7,A8,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B4 */
{{B3,B2,B1,NO,NO,NO,NO,NO},{C4,D4,E4,F4,G4,H4,NO,NO},
{A4,NO,NO,NO,NO,NO,NO,NO},{B5,B6,B7,B8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C4 */
{{C3,C2,C1,NO,NO,NO,NO,NO},{D4,E4,F4,G4,H4,NO,NO,NO},
{B4,A4,NO,NO,NO,NO,NO,NO},{C5,C6,C7,C8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D4 */
{{D3,D2,D1,NO,NO,NO,NO,NO},{E4,F4,G4,H4,NO,NO,NO,NO},
{C4,B4,A4,NO,NO,NO,NO,NO},{D5,D6,D7,D8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E4 */
{{E3,E2,E1,NO,NO,NO,NO,NO},{D4,C4,B4,A4,NO,NO,NO,NO},
{F4,G4,H4,NO,NO,NO,NO,NO},{E5,E6,E7,E8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F4 */
{{F3,F2,F1,NO,NO,NO,NO,NO},{E4,D4,C4,B4,A4,NO,NO,NO},
{G4,H4,NO,NO,NO,NO,NO,NO},{F5,F6,F7,F8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G4 */
{{G3,G2,G1,NO,NO,NO,NO,NO},{F4,E4,D4,C4,B4,A4,NO,NO},
{H4,NO,NO,NO,NO,NO,NO,NO},{G5,G6,G7,G8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H4 */
{{H3,H2,H1,NO,NO,NO,NO,NO},{G4,F4,E4,D4,C4,B4,A4,NO},
{H5,H6,H7,H8,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A5 */
{{A4,A3,A2,A1,NO,NO,NO,NO},{B5,C5,D5,E5,F5,G5,H5,NO},
{A6,A7,A8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B5 */
{{B4,B3,B2,B1,NO,NO,NO,NO},{C5,D5,E5,F5,G5,H5,NO,NO},
{A5,NO,NO,NO,NO,NO,NO,NO},{B6,B7,B8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C5 */
{{C4,C3,C2,C1,NO,NO,NO,NO},{D5,E5,F5,G5,H5,NO,NO,NO},
{B5,A5,NO,NO,NO,NO,NO,NO},{C6,C7,C8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D5 */
{{D4,D3,D2,D1,NO,NO,NO,NO},{E5,F5,G5,H5,NO,NO,NO,NO},
{C5,B5,A5,NO,NO,NO,NO,NO},{D6,D7,D8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E5 */
{{E4,E3,E2,E1,NO,NO,NO,NO},{D5,C5,B5,A5,NO,NO,NO,NO},
{F5,G5,H5,NO,NO,NO,NO,NO},{E6,E7,E8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F5 */
{{F4,F3,F2,F1,NO,NO,NO,NO},{E5,D5,C5,B5,A5,NO,NO,NO},
{G5,H5,NO,NO,NO,NO,NO,NO},{F6,F7,F8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G5 */
{{G4,G3,G2,G1,NO,NO,NO,NO},{F5,E5,D5,C5,B5,A5,NO,NO},
{H5,NO,NO,NO,NO,NO,NO,NO},{G6,G7,G8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H5 */
{{H4,H3,H2,H1,NO,NO,NO,NO},{G5,F5,E5,D5,C5,B5,A5,NO},
{H6,H7,H8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A6 */
{{A5,A4,A3,A2,A1,NO,NO,NO},{B6,C6,D6,E6,F6,G6,H6,NO},
{A7,A8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B6 */
{{B5,B4,B3,B2,B1,NO,NO,NO},{C6,D6,E6,F6,G6,H6,NO,NO},
{A6,NO,NO,NO,NO,NO,NO,NO},{B7,B8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C6 */
{{C5,C4,C3,C2,C1,NO,NO,NO},{D6,E6,F6,G6,H6,NO,NO,NO},
{B6,A6,NO,NO,NO,NO,NO,NO},{C7,C8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D6 */
{{D5,D4,D3,D2,D1,NO,NO,NO},{E6,F6,G6,H6,NO,NO,NO,NO},
{C6,B6,A6,NO,NO,NO,NO,NO},{D7,D8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E6 */
{{E5,E4,E3,E2,E1,NO,NO,NO},{D6,C6,B6,A6,NO,NO,NO,NO},
{F6,G6,H6,NO,NO,NO,NO,NO},{E7,E8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F6 */
{{F5,F4,F3,F2,F1,NO,NO,NO},{E6,D6,C6,B6,A6,NO,NO,NO},
{G6,H6,NO,NO,NO,NO,NO,NO},{F7,F8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G6 */
{{G5,G4,G3,G2,G1,NO,NO,NO},{F6,E6,D6,C6,B6,A6,NO,NO},
{H6,NO,NO,NO,NO,NO,NO,NO},{G7,G8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H6 */
{{H5,H4,H3,H2,H1,NO,NO,NO},{G6,F6,E6,D6,C6,B6,A6,NO},
{H7,H8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A7 */
{{A6,A5,A4,A3,A2,A1,NO,NO},{B7,C7,D7,E7,F7,G7,H7,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B7 */
{{B6,B5,B4,B3,B2,B1,NO,NO},{C7,D7,E7,F7,G7,H7,NO,NO},
{A7,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C7 */
{{C6,C5,C4,C3,C2,C1,NO,NO},{D7,E7,F7,G7,H7,NO,NO,NO},
{B7,A7,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D7 */
{{D6,D5,D4,D3,D2,D1,NO,NO},{E7,F7,G7,H7,NO,NO,NO,NO},
{C7,B7,A7,NO,NO,NO,NO,NO},{D8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E7 */
{{E6,E5,E4,E3,E2,E1,NO,NO},{D7,C7,B7,A7,NO,NO,NO,NO},
{F7,G7,H7,NO,NO,NO,NO,NO},{E8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F7 */
{{F6,F5,F4,F3,F2,F1,NO,NO},{E7,D7,C7,B7,A7,NO,NO,NO},
{G7,H7,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G7 */
{{G6,G5,G4,G3,G2,G1,NO,NO},{F7,E7,D7,C7,B7,A7,NO,NO},
{H7,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H7 */
{{H6,H5,H4,H3,H2,H1,NO,NO},{G7,F7,E7,D7,C7,B7,A7,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK A8 */
{{A7,A6,A5,A4,A3,A2,A1,NO},{B8,C8,D8,E8,F8,G8,H8,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK B8 */
{{B7,B6,B5,B4,B3,B2,B1,NO},{C8,D8,E8,F8,G8,H8,NO,NO},
{A8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK C8 */
{{C7,C6,C5,C4,C3,C2,C1,NO},{D8,E8,F8,G8,H8,NO,NO,NO},
{B8,A8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK D8 */
{{D7,D6,D5,D4,D3,D2,D1,NO},{E8,F8,G8,H8,NO,NO,NO,NO},
{C8,B8,A8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK E8 */
{{E7,E6,E5,E4,E3,E2,E1,NO},{D8,C8,B8,A8,NO,NO,NO,NO},
{F8,G8,H8,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK F8 */
{{F7,F6,F5,F4,F3,F2,F1,NO},{E8,D8,C8,B8,A8,NO,NO,NO},
{G8,H8,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK G8 */
{{G7,G6,G5,G4,G3,G2,G1,NO},{F8,E8,D8,C8,B8,A8,NO,NO},
{H8,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_ROOK H8 */
{{H7,H6,H5,H4,H3,H2,H1,NO},{G8,F8,E8,D8,C8,B8,A8,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end BLACK_ROOK */
/* begin WHITE_BISHOP */
{/* WHITE_BISHOP A1 */
{{B2,C3,D4,E5,F6,G7,H8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B1 */
{{C2,D3,E4,F5,G6,H7,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C1 */
{{D2,E3,F4,G5,H6,NO,NO,NO},{B2,A3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D1 */
{{E2,F3,G4,H5,NO,NO,NO,NO},{C2,B3,A4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E1 */
{{D2,C3,B4,A5,NO,NO,NO,NO},{F2,G3,H4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F1 */
{{E2,D3,C4,B5,A6,NO,NO,NO},{G2,H3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G1 */
{{F2,E3,D4,C5,B6,A7,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H1 */
{{G2,F3,E4,D5,C6,B7,A8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A2 */
{{B3,C4,D5,E6,F7,G8,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B2 */
{{C3,D4,E5,F6,G7,H8,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{C1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C2 */
{{D3,E4,F5,G6,H7,NO,NO,NO},{B3,A4,NO,NO,NO,NO,NO,NO},
{D1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D2 */
{{E3,F4,G5,H6,NO,NO,NO,NO},{C3,B4,A5,NO,NO,NO,NO,NO},
{E1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E2 */
{{D3,C4,B5,A6,NO,NO,NO,NO},{F3,G4,H5,NO,NO,NO,NO,NO},
{D1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F2 */
{{E3,D4,C5,B6,A7,NO,NO,NO},{G3,H4,NO,NO,NO,NO,NO,NO},
{E1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G2 */
{{F3,E4,D5,C6,B7,A8,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{F1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H2 */
{{G3,F4,E5,D6,C7,B8,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A3 */
{{B4,C5,D6,E7,F8,NO,NO,NO},{B2,C1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B3 */
{{C4,D5,E6,F7,G8,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{C2,D1,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C3 */
{{D4,E5,F6,G7,H8,NO,NO,NO},{B4,A5,NO,NO,NO,NO,NO,NO},
{D2,E1,NO,NO,NO,NO,NO,NO},{B2,A1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D3 */
{{E4,F5,G6,H7,NO,NO,NO,NO},{C4,B5,A6,NO,NO,NO,NO,NO},
{E2,F1,NO,NO,NO,NO,NO,NO},{C2,B1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E3 */
{{D4,C5,B6,A7,NO,NO,NO,NO},{F4,G5,H6,NO,NO,NO,NO,NO},
{D2,C1,NO,NO,NO,NO,NO,NO},{F2,G1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F3 */
{{E4,D5,C6,B7,A8,NO,NO,NO},{G4,H5,NO,NO,NO,NO,NO,NO},
{E2,D1,NO,NO,NO,NO,NO,NO},{G2,H1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G3 */
{{F4,E5,D6,C7,B8,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{F2,E1,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H3 */
{{G4,F5,E6,D7,C8,NO,NO,NO},{G2,F1,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A4 */
{{B5,C6,D7,E8,NO,NO,NO,NO},{B3,C2,D1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B4 */
{{C5,D6,E7,F8,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{C3,D2,E1,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C4 */
{{D5,E6,F7,G8,NO,NO,NO,NO},{B5,A6,NO,NO,NO,NO,NO,NO},
{D3,E2,F1,NO,NO,NO,NO,NO},{B3,A2,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D4 */
{{E5,F6,G7,H8,NO,NO,NO,NO},{C5,B6,A7,NO,NO,NO,NO,NO},
{E3,F2,G1,NO,NO,NO,NO,NO},{C3,B2,A1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E4 */
{{D5,C6,B7,A8,NO,NO,NO,NO},{F5,G6,H7,NO,NO,NO,NO,NO},
{D3,C2,B1,NO,NO,NO,NO,NO},{F3,G2,H1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F4 */
{{E5,D6,C7,B8,NO,NO,NO,NO},{G5,H6,NO,NO,NO,NO,NO,NO},
{E3,D2,C1,NO,NO,NO,NO,NO},{G3,H2,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G4 */
{{F5,E6,D7,C8,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{F3,E2,D1,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H4 */
{{G5,F6,E7,D8,NO,NO,NO,NO},{G3,F2,E1,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A5 */
{{B6,C7,D8,NO,NO,NO,NO,NO},{B4,C3,D2,E1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B5 */
{{C6,D7,E8,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{C4,D3,E2,F1,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C5 */
{{D6,E7,F8,NO,NO,NO,NO,NO},{B6,A7,NO,NO,NO,NO,NO,NO},
{D4,E3,F2,G1,NO,NO,NO,NO},{B4,A3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D5 */
{{E6,F7,G8,NO,NO,NO,NO,NO},{C6,B7,A8,NO,NO,NO,NO,NO},
{E4,F3,G2,H1,NO,NO,NO,NO},{C4,B3,A2,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E5 */
{{D6,C7,B8,NO,NO,NO,NO,NO},{F6,G7,H8,NO,NO,NO,NO,NO},
{D4,C3,B2,A1,NO,NO,NO,NO},{F4,G3,H2,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F5 */
{{E6,D7,C8,NO,NO,NO,NO,NO},{G6,H7,NO,NO,NO,NO,NO,NO},
{E4,D3,C2,B1,NO,NO,NO,NO},{G4,H3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G5 */
{{F6,E7,D8,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{F4,E3,D2,C1,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H5 */
{{G6,F7,E8,NO,NO,NO,NO,NO},{G4,F3,E2,D1,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A6 */
{{B7,C8,NO,NO,NO,NO,NO,NO},{B5,C4,D3,E2,F1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B6 */
{{C7,D8,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{C5,D4,E3,F2,G1,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C6 */
{{D7,E8,NO,NO,NO,NO,NO,NO},{B7,A8,NO,NO,NO,NO,NO,NO},
{D5,E4,F3,G2,H1,NO,NO,NO},{B5,A4,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D6 */
{{E7,F8,NO,NO,NO,NO,NO,NO},{C7,B8,NO,NO,NO,NO,NO,NO},
{E5,F4,G3,H2,NO,NO,NO,NO},{C5,B4,A3,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E6 */
{{D7,C8,NO,NO,NO,NO,NO,NO},{F7,G8,NO,NO,NO,NO,NO,NO},
{D5,C4,B3,A2,NO,NO,NO,NO},{F5,G4,H3,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F6 */
{{E7,D8,NO,NO,NO,NO,NO,NO},{G7,H8,NO,NO,NO,NO,NO,NO},
{E5,D4,C3,B2,A1,NO,NO,NO},{G5,H4,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G6 */
{{F7,E8,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{F5,E4,D3,C2,B1,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H6 */
{{G7,F8,NO,NO,NO,NO,NO,NO},{G5,F4,E3,D2,C1,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A7 */
{{B8,NO,NO,NO,NO,NO,NO,NO},{B6,C5,D4,E3,F2,G1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B7 */
{{C8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{C6,D5,E4,F3,G2,H1,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{D6,E5,F4,G3,H2,NO,NO,NO},{B6,A5,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{E6,F5,G4,H3,NO,NO,NO,NO},{C6,B5,A4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E7 */
{{D8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{D6,C5,B4,A3,NO,NO,NO,NO},{F6,G5,H4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F7 */
{{E8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{E6,D5,C4,B3,A2,NO,NO,NO},{G6,H5,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G7 */
{{F8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{F6,E5,D4,C3,B2,A1,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H7 */
{{G8,NO,NO,NO,NO,NO,NO,NO},{G6,F5,E4,D3,C2,B1,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP A8 */
{{B7,C6,D5,E4,F3,G2,H1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP B8 */
{{C7,D6,E5,F4,G3,H2,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP C8 */
{{D7,E6,F5,G4,H3,NO,NO,NO},{B7,A6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP D8 */
{{E7,F6,G5,H4,NO,NO,NO,NO},{C7,B6,A5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP E8 */
{{D7,C6,B5,A4,NO,NO,NO,NO},{F7,G6,H5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP F8 */
{{E7,D6,C5,B4,A3,NO,NO,NO},{G7,H6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP G8 */
{{F7,E6,D5,C4,B3,A2,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* WHITE_BISHOP H8 */
{{G7,F6,E5,D4,C3,B2,A1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
},
/* end WHITE_BISHOP */
/* begin BLACK_BISHOP */
{/* BLACK_BISHOP A1 */
{{B2,C3,D4,E5,F6,G7,H8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B1 */
{{C2,D3,E4,F5,G6,H7,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C1 */
{{D2,E3,F4,G5,H6,NO,NO,NO},{B2,A3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D1 */
{{E2,F3,G4,H5,NO,NO,NO,NO},{C2,B3,A4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E1 */
{{D2,C3,B4,A5,NO,NO,NO,NO},{F2,G3,H4,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F1 */
{{E2,D3,C4,B5,A6,NO,NO,NO},{G2,H3,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G1 */
{{F2,E3,D4,C5,B6,A7,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H1 */
{{G2,F3,E4,D5,C6,B7,A8,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A2 */
{{B1,NO,NO,NO,NO,NO,NO,NO},{B3,C4,D5,E6,F7,G8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B2 */
{{C1,NO,NO,NO,NO,NO,NO,NO},{A1,NO,NO,NO,NO,NO,NO,NO},
{C3,D4,E5,F6,G7,H8,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{B1,NO,NO,NO,NO,NO,NO,NO},
{D3,E4,F5,G6,H7,NO,NO,NO},{B3,A4,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{C1,NO,NO,NO,NO,NO,NO,NO},
{E3,F4,G5,H6,NO,NO,NO,NO},{C3,B4,A5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E2 */
{{D1,NO,NO,NO,NO,NO,NO,NO},{F1,NO,NO,NO,NO,NO,NO,NO},
{D3,C4,B5,A6,NO,NO,NO,NO},{F3,G4,H5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F2 */
{{E1,NO,NO,NO,NO,NO,NO,NO},{G1,NO,NO,NO,NO,NO,NO,NO},
{E3,D4,C5,B6,A7,NO,NO,NO},{G3,H4,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G2 */
{{F1,NO,NO,NO,NO,NO,NO,NO},{H1,NO,NO,NO,NO,NO,NO,NO},
{F3,E4,D5,C6,B7,A8,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H2 */
{{G1,NO,NO,NO,NO,NO,NO,NO},{G3,F4,E5,D6,C7,B8,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A3 */
{{B2,C1,NO,NO,NO,NO,NO,NO},{B4,C5,D6,E7,F8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B3 */
{{C2,D1,NO,NO,NO,NO,NO,NO},{A2,NO,NO,NO,NO,NO,NO,NO},
{C4,D5,E6,F7,G8,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C3 */
{{D2,E1,NO,NO,NO,NO,NO,NO},{B2,A1,NO,NO,NO,NO,NO,NO},
{D4,E5,F6,G7,H8,NO,NO,NO},{B4,A5,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D3 */
{{E2,F1,NO,NO,NO,NO,NO,NO},{C2,B1,NO,NO,NO,NO,NO,NO},
{E4,F5,G6,H7,NO,NO,NO,NO},{C4,B5,A6,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E3 */
{{D2,C1,NO,NO,NO,NO,NO,NO},{F2,G1,NO,NO,NO,NO,NO,NO},
{D4,C5,B6,A7,NO,NO,NO,NO},{F4,G5,H6,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F3 */
{{E2,D1,NO,NO,NO,NO,NO,NO},{G2,H1,NO,NO,NO,NO,NO,NO},
{E4,D5,C6,B7,A8,NO,NO,NO},{G4,H5,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G3 */
{{F2,E1,NO,NO,NO,NO,NO,NO},{H2,NO,NO,NO,NO,NO,NO,NO},
{F4,E5,D6,C7,B8,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H3 */
{{G2,F1,NO,NO,NO,NO,NO,NO},{G4,F5,E6,D7,C8,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A4 */
{{B3,C2,D1,NO,NO,NO,NO,NO},{B5,C6,D7,E8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B4 */
{{C3,D2,E1,NO,NO,NO,NO,NO},{A3,NO,NO,NO,NO,NO,NO,NO},
{C5,D6,E7,F8,NO,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C4 */
{{D3,E2,F1,NO,NO,NO,NO,NO},{B3,A2,NO,NO,NO,NO,NO,NO},
{D5,E6,F7,G8,NO,NO,NO,NO},{B5,A6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D4 */
{{E3,F2,G1,NO,NO,NO,NO,NO},{C3,B2,A1,NO,NO,NO,NO,NO},
{E5,F6,G7,H8,NO,NO,NO,NO},{C5,B6,A7,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E4 */
{{D3,C2,B1,NO,NO,NO,NO,NO},{F3,G2,H1,NO,NO,NO,NO,NO},
{D5,C6,B7,A8,NO,NO,NO,NO},{F5,G6,H7,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F4 */
{{E3,D2,C1,NO,NO,NO,NO,NO},{G3,H2,NO,NO,NO,NO,NO,NO},
{E5,D6,C7,B8,NO,NO,NO,NO},{G5,H6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G4 */
{{F3,E2,D1,NO,NO,NO,NO,NO},{H3,NO,NO,NO,NO,NO,NO,NO},
{F5,E6,D7,C8,NO,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H4 */
{{G3,F2,E1,NO,NO,NO,NO,NO},{G5,F6,E7,D8,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A5 */
{{B4,C3,D2,E1,NO,NO,NO,NO},{B6,C7,D8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B5 */
{{C4,D3,E2,F1,NO,NO,NO,NO},{A4,NO,NO,NO,NO,NO,NO,NO},
{C6,D7,E8,NO,NO,NO,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C5 */
{{D4,E3,F2,G1,NO,NO,NO,NO},{B4,A3,NO,NO,NO,NO,NO,NO},
{D6,E7,F8,NO,NO,NO,NO,NO},{B6,A7,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D5 */
{{E4,F3,G2,H1,NO,NO,NO,NO},{C4,B3,A2,NO,NO,NO,NO,NO},
{E6,F7,G8,NO,NO,NO,NO,NO},{C6,B7,A8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E5 */
{{D4,C3,B2,A1,NO,NO,NO,NO},{F4,G3,H2,NO,NO,NO,NO,NO},
{D6,C7,B8,NO,NO,NO,NO,NO},{F6,G7,H8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F5 */
{{E4,D3,C2,B1,NO,NO,NO,NO},{G4,H3,NO,NO,NO,NO,NO,NO},
{E6,D7,C8,NO,NO,NO,NO,NO},{G6,H7,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G5 */
{{F4,E3,D2,C1,NO,NO,NO,NO},{H4,NO,NO,NO,NO,NO,NO,NO},
{F6,E7,D8,NO,NO,NO,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H5 */
{{G4,F3,E2,D1,NO,NO,NO,NO},{G6,F7,E8,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A6 */
{{B5,C4,D3,E2,F1,NO,NO,NO},{B7,C8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B6 */
{{C5,D4,E3,F2,G1,NO,NO,NO},{A5,NO,NO,NO,NO,NO,NO,NO},
{C7,D8,NO,NO,NO,NO,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C6 */
{{D5,E4,F3,G2,H1,NO,NO,NO},{B5,A4,NO,NO,NO,NO,NO,NO},
{D7,E8,NO,NO,NO,NO,NO,NO},{B7,A8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D6 */
{{E5,F4,G3,H2,NO,NO,NO,NO},{C5,B4,A3,NO,NO,NO,NO,NO},
{E7,F8,NO,NO,NO,NO,NO,NO},{C7,B8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E6 */
{{D5,C4,B3,A2,NO,NO,NO,NO},{F5,G4,H3,NO,NO,NO,NO,NO},
{D7,C8,NO,NO,NO,NO,NO,NO},{F7,G8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F6 */
{{E5,D4,C3,B2,A1,NO,NO,NO},{G5,H4,NO,NO,NO,NO,NO,NO},
{E7,D8,NO,NO,NO,NO,NO,NO},{G7,H8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G6 */
{{F5,E4,D3,C2,B1,NO,NO,NO},{H5,NO,NO,NO,NO,NO,NO,NO},
{F7,E8,NO,NO,NO,NO,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H6 */
{{G5,F4,E3,D2,C1,NO,NO,NO},{G7,F8,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A7 */
{{B6,C5,D4,E3,F2,G1,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B7 */
{{C6,D5,E4,F3,G2,H1,NO,NO},{A6,NO,NO,NO,NO,NO,NO,NO},
{C8,NO,NO,NO,NO,NO,NO,NO},{A8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C7 */
{{D6,E5,F4,G3,H2,NO,NO,NO},{B6,A5,NO,NO,NO,NO,NO,NO},
{D8,NO,NO,NO,NO,NO,NO,NO},{B8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D7 */
{{E6,F5,G4,H3,NO,NO,NO,NO},{C6,B5,A4,NO,NO,NO,NO,NO},
{E8,NO,NO,NO,NO,NO,NO,NO},{C8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E7 */
{{D6,C5,B4,A3,NO,NO,NO,NO},{F6,G5,H4,NO,NO,NO,NO,NO},
{D8,NO,NO,NO,NO,NO,NO,NO},{F8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F7 */
{{E6,D5,C4,B3,A2,NO,NO,NO},{G6,H5,NO,NO,NO,NO,NO,NO},
{E8,NO,NO,NO,NO,NO,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G7 */
{{F6,E5,D4,C3,B2,A1,NO,NO},{H6,NO,NO,NO,NO,NO,NO,NO},
{F8,NO,NO,NO,NO,NO,NO,NO},{H8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H7 */
{{G6,F5,E4,D3,C2,B1,NO,NO},{G8,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP A8 */
{{B7,C6,D5,E4,F3,G2,H1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP B8 */
{{C7,D6,E5,F4,G3,H2,NO,NO},{A7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP C8 */
{{D7,E6,F5,G4,H3,NO,NO,NO},{B7,A6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP D8 */
{{E7,F6,G5,H4,NO,NO,NO,NO},{C7,B6,A5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP E8 */
{{D7,C6,B5,A4,NO,NO,NO,NO},{F7,G6,H5,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP F8 */
{{E7,D6,C5,B4,A3,NO,NO,NO},{G7,H6,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP G8 */
{{F7,E6,D5,C4,B3,A2,NO,NO},{H7,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}},
/* BLACK_BISHOP H8 */
{{G7,F6,E5,D4,C3,B2,A1,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO},
{NO,NO,NO,NO,NO,NO,NO,NO},{NO,NO,NO,NO,NO,NO,NO,NO}}
}
/* end BLACK_BISHOP */
};
#endif // !defined(CHESS_H_INCLUDED)