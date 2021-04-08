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
// bitboard.cpp provides code essential to implementation of bitboards.
// It is derived from bitboard.cpp in Stockfish 2.2.2.
#include <iostream>
#include "bitboard.h"
using namespace std;

// Some bitboards were commented out by D.A.G.:
/*
Bitboard RMasks[NUM_SQUARES];
Bitboard RMagics[NUM_SQUARES];
Bitboard* RAttacks[NUM_SQUARES];
int RShifts[NUM_SQUARES];

Bitboard BMasks[NUM_SQUARES];
Bitboard BMagics[NUM_SQUARES];
Bitboard* BAttacks[NUM_SQUARES];
int BShifts[NUM_SQUARES];
*/

Bitboard SquareBB[NUM_SQUARES];
Bitboard FileBB[NUM_FILES];
Bitboard RankBB[NUM_RANKS];
Bitboard AdjacentFilesBB[NUM_FILES];
Bitboard ThisAndAdjacentFilesBB[NUM_FILES];
Bitboard InFrontBB[NUM_COLORS][NUM_RANKS];
Bitboard StepAttacksBB[NUM_PIECES][NUM_SQUARES];
Bitboard BetweenBB[NUM_SQUARES][NUM_SQUARES];
Bitboard SquaresInFrontMask[NUM_COLORS][NUM_SQUARES];
Bitboard PassedPawnMask[NUM_COLORS][NUM_SQUARES];
Bitboard AttackSpanMask[NUM_COLORS][NUM_SQUARES];
//Bitboard PseudoAttacks[NUM_PIECE_TYPES][NUM_SQUARES];

int SquareDistance[NUM_SQUARES][NUM_SQUARES];

/// print_bitboard() prints a bitboard in an easily readable format to the
/// standard output. This is sometimes useful for debugging.
void print_bitboard(Bitboard b) {

  for (Rank r = RANK_8; r >= RANK_1; r=Rank(r-1))
  {
      std::cout << "+---+---+---+---+---+---+---+---+" << '\n';
      for (File f = FILE_A; f <= FILE_H; f=File(f+1))
          std::cout << "| " << ((b & make_square(f, r)).any() ? "X " : "  ");

      std::cout << "|\n";
  }
  std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
}

/// bitboards_init() initializes various bitboard arrays. It is called during
/// program initialization.
void bitboards_init() {
  /*
  for (Bitboard b = 0; b < 256; b++)
      BitCount8Bit[b] = (uint8_t)popcount<Max15>(b);
  */
  for (Square s = A1; s <= H8; s=Square(s+1))
      SquareBB[s] = 1ULL << s;

  FileBB[FILE_A] = FileABB;
  RankBB[RANK_1] = Rank1BB;

  for (File f = FILE_B; f <= FILE_H; f=File(f+1))
  {
      FileBB[f] = FileBB[f - 1] << 1;
      RankBB[f] = RankBB[f - 1] << 8;
  }

  for (File f = FILE_A; f <= FILE_H; f=File(f+1))
  {
      AdjacentFilesBB[f] = (f > FILE_A ? FileBB[f - 1] : 0) | (f < FILE_H ? FileBB[f + 1] : 0);
      ThisAndAdjacentFilesBB[f] = FileBB[f] | AdjacentFilesBB[f];
  }

  for (Rank rw = RANK_7, rb = RANK_2; rw >= RANK_1; rw=Rank(rw-1), rb=Rank(rb+1))
  {
      InFrontBB[WHITE][rw] = InFrontBB[WHITE][rw + 1] | RankBB[rw + 1];
      InFrontBB[BLACK][rb] = InFrontBB[BLACK][rb - 1] | RankBB[rb - 1];
  }

  for (Color c = WHITE; c <= BLACK; c=Color(c+1))
      for (Square s = A1; s <= H8; s=Square(s+1))
      {
          SquaresInFrontMask[c][s] = in_front_bb(c, s) & file_bb(s);
          PassedPawnMask[c][s]     = in_front_bb(c, s) & this_and_adjacent_files_bb(file_of(s));
          AttackSpanMask[c][s]     = in_front_bb(c, s) & adjacent_files_bb(file_of(s));
      }

  for (Square s1 = A1; s1 <= H8; s1=Square(s1+1))
      for (Square s2 = A1; s2 <= H8; s2=Square(s2+1))
          SquareDistance[s1][s2] = std::max(file_distance(s1, s2), rank_distance(s1, s2));
  // D.A.G., 02/25/2012: BSF stands for Bit Scan Forward. It is an x86 instruction 
  // to "find first set", i.e. compute the index (position) of the least significant
  // bit set to 1 in an unsigned machine word. Commented out the computation of BSFTable:
  /*
  for (int i = 0; i < 64; i++)
      if (!Is64Bit) // Matt Taylor's folding trick for 32 bit systems
      {
          Bitboard b = 1ULL << i;
          b ^= b - 1;
          b ^= b >> 32;
          BSFTable[(uint32_t)(b * 0x783A9B23) >> 26] = i;
      }
      else
          BSFTable[((1ULL << i) * 0x218A392CD3D5DBFULL) >> 58] = i;
  */
  
  // steps were rearranged by D.A.G. to comply with 
  // the order of piece types in PieceType from chess.h
  // (NO_PIECE, PAWN, KNIGHT, KING, QUEEN, ROOK, BISHOP):
  int steps[][9] = { {}, { 7, 9 }, { 17, 15, 10, 6, -6, -10, -15, -17 },
                     { 9, 7, -7, -9, 8, 1, -1, -8 }, {}, {}, {}};
  // Compute StepAttacksBB:
  for (Color c = WHITE; c <= BLACK; c=Color(c+1))
      for (PieceType pt = PAWN; pt <= BISHOP; pt=PieceType(pt+1))
          for (Square s = A1; s <= H8; s=Square(s+1))
              for (int k = 0; steps[pt][k]; k++)
              {
                  Square to = Square(s + Square(c == WHITE ? steps[pt][k] : -steps[pt][k]));

                  if (square_is_ok(to) && square_distance(s, to) < 3)
                      StepAttacksBB[make_piece(c, pt)][s] |= to;
              }
  // D.A.G. commented out computation of PseudoAttacks:
  /*
  Square RDeltas[] = { DELTA_N,  DELTA_E,  DELTA_S,  DELTA_W  };
  Square BDeltas[] = { DELTA_NE, DELTA_SE, DELTA_SW, DELTA_NW };

  init_magics(RTable, RAttacks, RMagics, RMasks, RShifts, RDeltas, r_index);
  init_magics(BTable, BAttacks, BMagics, BMasks, BShifts, BDeltas, b_index);

  for (Square s = SQ_A1; s <= SQ_H8; s++)
  {
      PseudoAttacks[BISHOP][s] = bishop_attacks_bb(s, 0);
      PseudoAttacks[ROOK][s]   = rook_attacks_bb(s, 0);
      PseudoAttacks[QUEEN][s]  = PseudoAttacks[BISHOP][s] | PseudoAttacks[ROOK][s];
  }

  for (Square s1 = SQ_A1; s1 <= SQ_H8; s1++)
      for (Square s2 = SQ_A1; s2 <= SQ_H8; s2++)
          if (PseudoAttacks[QUEEN][s1] & s2)
          {
              Square delta = (s2 - s1) / square_distance(s1, s2);

              for (Square s = s1 + delta; s != s2; s += delta)
                  BetweenBB[s1][s2] |= s;
          }
  */
}