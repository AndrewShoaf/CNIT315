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
// position.h defined the critically important class Position.
// In our chess program, positions are instances of this class.
// This file is derived from position.h in Stockfish 2.2.2.
// It comes third in a series of headers: chess.h -> bitboard.h ->
// position.h -> search.h -> timeman.h -> (thread.h)...
#include <string>
#include "bitboard.h"

// D.A.G., 03/08/2012: PieceToChar was modified to comply
// with the order of pieces in Piece (chess.h).
/// To convert a Piece to and from a FEN char:
const string PieceToChar(" .PpNnKkQqRrBb");
inline const std::string square_to_string(Square s)
{
  char ch[] = { file_to_char(file_of(s)), rank_to_char(rank_of(s)), 0 };
  return ch;
}
class Position; // pre-announce
/// The checkInfo struct is initialized at constructor time and
/// keeps the information used to detect if a move gives check.
struct CheckInfo
{
  explicit CheckInfo(const Position&);

  Bitboard dcCandidates; // Bitboard of discovered check candidates
  Bitboard pinned;       // Bitboard of pinned pieces
  Bitboard checkSq[NUM_PIECE_TYPES]; // Bitboards of checkers of various piece types
  Square ksq;            // king square
};

/// The StateInfo struct stores information we need to restore a Position
/// object to its previous state when we retract a move. Whenever a move
/// is made on the board (by calling Position::do_move), an StateInfo object
/// must be passed as a parameter.

struct StateInfo
{
  // Key pawnKey, materialKey;
  int npMaterial[2]; // non-pawn material
  int castleRights, rule50, pliesFromNull;
  int value; // Score
  Square epSquare; // en passant square

  Key key; // hash table key
  Bitboard checkersBB; // bitboard of enemy pieces currently giving check
  PieceType capturedType;
  StateInfo* previous; // to build a list with pointers
};

/// The position data structure. A position consists of the following data:
///
///    * For each piece type, a bitboard representing the squares occupied
///      by pieces of that type.
///    * For each color, a bitboard representing the squares occupied by
///      pieces of that color.
///    * A bitboard of all occupied squares.
///    * A bitboard of all checking pieces.
///    * A 64-entry array of pieces, indexed by the squares of the board.
///    * The current side to move.
///    * Information about the castling rights for both sides.
///    * The initial files of the kings and both pairs of rooks. This is
///      used to implement the Chess960 castling rules.
///    * The en passant square (which is SQ_NONE if no en passant capture is
///      possible).
///    * The squares of the kings for both sides.
///    * Hash keys for the position itself, the current pawn structure, and
///      the current material situation.
///    * Hash keys for all previous positions in the game for detecting
///      repetition draws.
///    * A counter for detecting 50 move rule draws.

class Position
{
  // No copy c'tor or assignment operator allowed
  Position(const Position&);
  Position& operator=(const Position&);

public:
  Position() {}
  Position(const Position& pos, int th) { copy(pos, th); }
  Position(const std::string& fen, bool isChess960, int th);

  // Text input/output
  void copy(const Position& pos, int th);
  void from_fen(const std::string& fen, bool isChess960);
  const std::string to_fen() const;
  void print(Move m) const; // D.A.G., 02/26/2012: Removed = MOVE_NONE after m

  // The piece on a given square
  Piece piece_on(Square s) const;
  Piece piece_moved(Move m) const;
  bool square_is_empty(Square s) const;

  // Side to move
  Color side_to_move() const;

  // Bitboard representation of the position
  Bitboard empty_squares() const;
  Bitboard occupied_squares() const;
  Bitboard pieces(Color c) const;
  Bitboard pieces(PieceType pt) const;
  Bitboard pieces(PieceType pt, Color c) const;
  Bitboard pieces(PieceType pt1, PieceType pt2) const;
  Bitboard pieces(PieceType pt1, PieceType pt2, Color c) const;

  // Number of pieces of each color and type
  int piece_count(Color c, PieceType pt) const;

  // The en passant square
  Square ep_square() const;

  // Current king position for each color
  Square king_square(Color c) const;

  // Castling rights
  bool can_castle(CastleRight f) const;
  bool can_castle(Color c) const;
  Square castle_rook_square(CastleRight f) const;

  // Bitboards for pinned pieces and discovered check candidates
  Bitboard discovered_check_candidates() const;
  Bitboard pinned_pieces() const;

  // Checking pieces and under check information
  Bitboard checkers() const;
  bool in_check() const;

  // Piece lists
  const Square* piece_list(Color c, PieceType pt) const;

  // Information about attacks to or from a given square
  // D.A.G., 03/18/2012: Commented out computation of attacks for now.
  /*
  Bitboard attackers_to(Square s) const;
  Bitboard attackers_to(Square s, Bitboard occ) const;
  Bitboard attacks_from(Piece p, Square s) const;
  static Bitboard attacks_from(Piece p, Square s, Bitboard occ);
  template<PieceType> Bitboard attacks_from(Square s) const;
  template<PieceType> Bitboard attacks_from(Square s, Color c) const;
  */

  // Properties of moves
  // D.A.G., 03/18/2012. Commented out:
  /*
  bool move_gives_check(Move m, const CheckInfo& ci) const;
  bool move_attacks_square(Move m, Square s) const;
  bool pl_move_is_legal(Move m, Bitboard pinned) const;
  bool is_pseudo_legal(const Move m) const;
  */
  bool is_capture(Move m) const;
  bool is_capture_or_promotion(Move m) const;
  bool is_passed_pawn_push(Move m) const;

  // Piece captured with previous moves
  PieceType captured_piece_type() const;

  // Information about pawns
  bool pawn_is_passed(Color c, Square s) const;

  // Doing and undoing moves
  // D.A.G., 03/19/2012: Commented out. We just generate moves at this point.
  /*
  void do_move(Move m, StateInfo& st);
  void do_move(Move m, StateInfo& st, const CheckInfo& ci, bool moveIsCheck);
  void undo_move(Move m);
  template<bool Do> void do_null_move(StateInfo& st);
  */

  // Static exchange evaluation
  // D.A.G., 03/19/2012: Commented out. We don't evaluate exchanges yet.
  /*
  int see(Move m) const;
  int see_sign(Move m) const;
  */

  // Accessing hash keys
  Key key() const;
  Key exclusion_key() const;
  // D.A.G., 02/26/2012. Commented out access to pawn_key and material_key.
  /*
  Key pawn_key() const;
  Key material_key() const;
  */

  // Incremental evaluation

  int value() const;
  int non_pawn_material(Color c) const;
  // D.A.G., 03/19/2012. Commenting out pst_delta for now, PART 1 of 3:
  /*
  Score pst_delta(Piece piece, Square from, Square to) const;
  */

  // Other properties of the position
  template<bool SkipRepetition> bool is_draw() const;
  int startpos_ply_counter() const;
  bool opposite_colored_bishops() const;
  bool has_pawn_on_7th(Color c) const;
  bool is_chess960() const;

  // Current thread ID searching on the position
  int thread() const;

  int64_t nodes_searched() const;
  void set_nodes_searched(int64_t n);

  // Position consistency check, for debugging
  // D.A.G., 03/19/2012: Commented out.
  /*
  bool pos_is_ok(int* failedStep = NULL) const;
  void flip_me();
  */

  // Global initialization
  static void init();


private:

  // Initialization helper functions (used while setting up a position)
  void clear();
  void put_piece(Piece p, Square s);
  void set_castle_right(Color c, Square rsq);
  bool move_is_legal(const Move m) const;

  // Helper template functions
  template<bool Do> void do_castle_move(Move m);
  template<bool FindPinned> Bitboard hidden_checkers() const;

  // Computing hash keys from scratch (for initialization and debugging)
  Key compute_key() const;
  Key compute_pawn_key() const;
  Key compute_material_key() const;

  // Computing incremental evaluation scores and material counts
  // D.A.G., 02/25/2012. Commenting out evaluation for now, PART 2 of 3:
  /*
  Score pst(Piece p, Square s) const;
  Score compute_value() const;
  Value compute_non_pawn_material(Color c) const;
  */

  // Board
  Piece board[NUM_SQUARES];             // [square]

  // Bitboards
  Bitboard byTypeBB[NUM_PIECE_TYPES];   // [pieceType]
  Bitboard byColorBB[NUM_COLORS];       // [color]
  Bitboard occupied;

  // Piece counts
  int pieceCount[NUM_COLORS][NUM_PIECE_TYPES]; // [color][pieceType]

  // Piece lists
  Square pieceList[NUM_COLORS][NUM_PIECE_TYPES][NUM_SQUARES]; // [color][pieceType][index]
  int index[NUM_SQUARES];               // [square]

  // Other info
  int castleRightsMask[NUM_SQUARES];    // [square]
  Square castleRookSquare[NUM_CASTLE_RIGHTS]; // [castleRight]
  StateInfo startState;
  int64_t nodes;
  int startPosPly;
  Color sideToMove;
  int threadID;
  StateInfo* st;
  int chess960;

  // Static variables
  // pieceSquareTable was commented out by D.A.G., 02/25/2012:
  //static Score pieceSquareTable[NUM_PIECES][NUM_SQUARES]; // [piece][square]
  // zobrist was modified by D.A.G., 02/25/2012:
  static Key zobrist[NUM_PIECES][NUM_SQUARES];          // [piece][square]
  // zobEp was commented out by D.A.G., 02/25/2012:
  // EN_PASSANT is now a piece, covered by zobrist
  //static Key zobEp[64];                  // [square]
  static Key zobCastle[NUM_CASTLE_RIGHTS]; // [castleRight]
  static Key zobSideToMove; // very necessary!
  static Key zobExclusion;
};

inline int64_t Position::nodes_searched() const
{
  return nodes;
}

inline void Position::set_nodes_searched(int64_t n)
{
  nodes = n;
}

inline Piece Position::piece_on(Square s) const
{
  return board[s];
}

inline Piece Position::piece_moved(Move m) const
{
  return board[from_sq(m)];
}

inline bool Position::square_is_empty(Square s) const
{
  return board[s] == NO_PIECE;
}

inline Color Position::side_to_move() const
{
  return sideToMove;
}

inline Bitboard Position::occupied_squares() const
{
  return occupied;
}

inline Bitboard Position::empty_squares() const
{
  return ~occupied;
}

inline Bitboard Position::pieces(Color c) const
{
  return byColorBB[c];
}

inline Bitboard Position::pieces(PieceType pt) const
{
  return byTypeBB[pt];
}

inline Bitboard Position::pieces(PieceType pt, Color c) const
{
  return byTypeBB[pt] & byColorBB[c];
}

inline Bitboard Position::pieces(PieceType pt1, PieceType pt2) const
{
  return byTypeBB[pt1] | byTypeBB[pt2];
}

inline Bitboard Position::pieces(PieceType pt1, PieceType pt2, Color c) const
{
  return (byTypeBB[pt1] | byTypeBB[pt2]) & byColorBB[c];
}

inline int Position::piece_count(Color c, PieceType pt) const
{
  return pieceCount[c][pt];
}

inline const Square* Position::piece_list(Color c, PieceType pt) const
{
  return pieceList[c][pt];
}

inline Square Position::ep_square() const
{
  return st->epSquare;
}

inline Square Position::king_square(Color c) const
{
	//D.A.G., 03/18/2012. Assumes that there's exactly one king
	//of a given color, thus [0]:
	return pieceList[c][KING][0];
}

inline bool Position::can_castle(CastleRight f) const
{
  return (st->castleRights & f) != 0;
}

inline bool Position::can_castle(Color c) const
{
  return (st->castleRights & ((WHITE_OO | WHITE_OOO) << c))!=0;
}

inline Square Position::castle_rook_square(CastleRight f) const
{
  return castleRookSquare[f];
}
// D.A.G., 03/18/2012: Commented out attacks_from for now.
/*
template<PieceType Pt>
inline Bitboard Position::attacks_from(Square s) const
{
  return  Pt == BISHOP ? bishop_attacks_bb(s, occupied_squares())
        : Pt == ROOK   ? rook_attacks_bb(s, occupied_squares())
        : Pt == QUEEN  ? attacks_from<ROOK>(s) | attacks_from<BISHOP>(s)
                       : StepAttacksBB[Pt][s];
}

template<>
inline Bitboard Position::attacks_from<PAWN>(Square s, Color c) const
{
  return StepAttacksBB[make_piece(c, PAWN)][s];
}

inline Bitboard Position::attacks_from(Piece p, Square s) const
{
  return attacks_from(p, s, occupied_squares());
}

inline Bitboard Position::attackers_to(Square s) const
{
  return attackers_to(s, occupied_squares());
}
*/
inline Bitboard Position::checkers() const
{
  return st->checkersBB;
}

inline bool Position::in_check() const
{
  return st->checkersBB != 0;
}

inline Bitboard Position::discovered_check_candidates() const
{
  return hidden_checkers<false>();
}

inline Bitboard Position::pinned_pieces() const
{
  return hidden_checkers<true>();
}

inline bool Position::pawn_is_passed(Color c, Square s) const
{
  return (pieces(PAWN, ~c) & passed_pawn_mask(c, s)).none();
}

inline Key Position::key() const
{
  return st->key;
}

inline Key Position::exclusion_key() const
{
  return st->key ^ zobExclusion;
}
// pawn_key() and material_key() commented out by D.A.G., 02/26/2012
/*
inline Key Position::pawn_key() const {
  return st->pawnKey;
}

inline Key Position::material_key() const {
  return st->materialKey;
}
*/
// Evaluation commented out by D.A.G., 02/25/2012, PART 3 of 3:
/*
inline Score Position::pst(Piece p, Square s) const {
  return pieceSquareTable[p][s];
}

inline Score Position::pst_delta(Piece piece, Square from, Square to) const {
  return pieceSquareTable[piece][to] - pieceSquareTable[piece][from];
}
*/
inline int Position::value() const {
  return st->value;
}

inline int Position::non_pawn_material(Color c) const {
  return st->npMaterial[c];
}

inline bool Position::is_passed_pawn_push(Move m) const
{
  return   board[from_sq(m)] == make_piece(sideToMove, PAWN)
        && pawn_is_passed(sideToMove, to_sq(m));
}

inline int Position::startpos_ply_counter() const
{
  return startPosPly + st->pliesFromNull; // HACK
}

inline bool Position::opposite_colored_bishops() const
{
  return   pieceCount[WHITE][BISHOP] == 1
        && pieceCount[BLACK][BISHOP] == 1
        && opposite_colors(pieceList[WHITE][BISHOP][0], pieceList[BLACK][BISHOP][0]);
}

inline bool Position::has_pawn_on_7th(Color c) const
{
  return (pieces(PAWN, c) & rank_bb(relative_rank(c, RANK_7))).any();
}

inline bool Position::is_chess960() const
{
  return chess960 != 0;
}

inline bool Position::is_capture_or_promotion(Move m) const
{
	// Commented out by D.A.G., 02/26/2012:
	//assert(is_ok(m));
	return is_ok(m) && (is_special(m) ? !is_castle(m) : !square_is_empty(to_sq(m)));
}

inline bool Position::is_capture(Move m) const
{
	// Note that castle is coded as "king captures the rook" // D.A.G.: It used to be!
	// Commented out by D.A.G., 02/26/2012:
	// assert(is_ok(m));

	// Modified by D.A.G., 03/18/2012: Castling no longer "captures"; EN_PASSANT is now a piece:
	return is_ok(m) && (!square_is_empty(to_sq(m)) /*&& !is_castle(m)*/) /*|| is_enpassant(m)*/;
}

inline PieceType Position::captured_piece_type() const
{
  return st->capturedType;
}

inline int Position::thread() const
{
  return threadID;
}
