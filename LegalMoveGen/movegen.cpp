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
#include <algorithm>
#include <cassert>

#include "movegen.h"

RootMoveList::RootMoveList()
{
   length = 0;
   currentPos = 0;
}

bool RootMoveList::IsEmpty() const
{
   return (length == 0);
}

bool RootMoveList::IsFull() const
{
   return (length == MAX_MOVES);
}

int RootMoveList::GetLength() const
{
   return length;
}

void RootMoveList::Insert(RootMove item)
{
   rmlist[length] = item;
   length++;
}

void RootMoveList::Delete(RootMove item)
{
   int index = 0;
   while (index < length && item != rmlist[index].pv[0])
      index++;
   if (index < length) // item found
   {
      rmlist[index] = rmlist[length-1];
      length--;
   }
}

bool RootMoveList::IsThere(RootMove item) const
{
   int index = 0; // Index variable
   while (index < length && item != rmlist[index].pv[0])
      index++;
   return (index < length);
}

void RootMoveList::ResetList()
{
   currentPos = 0;
}

bool RootMoveList::HasNext() const
{
   return (currentPos != length);
}

RootMove RootMoveList::GetNextItem()
{
   return rmlist[currentPos++];
}

void RootMoveList::generate_from_position(Position& pos)
{ // TO DO:
}

void RootMoveList::print() const
{ // TO DO:
}



