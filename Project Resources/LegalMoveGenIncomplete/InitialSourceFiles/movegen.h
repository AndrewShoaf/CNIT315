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
#if !defined(MOVEGEN_H_INCLUDED)
#define MOVEGEN_H_INCLUDED

#include "search.h"

class Position; // pre-announce

// RootMoveList implements the list of moves for a given root position,
// NOT the list of moves for a given game!
// The array-based implementation is based on list.h from
// N. Dale and C. Weems, Programming and Problem Solving with C++, 5th Edition
// The BIG addition is the generate_from_position(Position& pos) member function
// Delete() and IsThere() are excluded, because we don't need them
// sort will be added later
class RootMoveList 
{
public:
	RootMoveList(); // Constructor: Makes an empty list
	void generate_from_position(Position& pos); // generates the root move list! 
	void operator++() { currentPos++; }
	void Insert(RootMove item);
	void Delete(RootMove item);
	bool IsThere(RootMove item) const;
    void ResetList(); // set currentPos to point at the first item
    RootMove GetNextItem(); // return item at the currentPos
    int GetLength() const;
    bool IsEmpty() const;
    bool IsFull() const;
    bool HasNext() const;
	void print() const;
private:
    int length;
    int currentPos;
    RootMove rmlist[MAX_MOVES]; // MAX_MOVES==256. No position is known with more than 218 moves to choose from
};

#endif // !defined(MOVEGEN_H_INCLUDED)
