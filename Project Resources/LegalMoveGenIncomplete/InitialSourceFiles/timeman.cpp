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
// D.A.G., 03/18/2012: timeman.cpp is intended to contain the code 
// having to do with time management. In a game with limited time
// for thinking, the program has to decide when to stop thinking
// and make the current best move. However, LegalMoveGen does not
// do that yet. Thus timeman.cpp merely provides system_time() 
// for void benchmark(int argc, char* argv[]) to invoke
// and measure how long its work takes.
#include "timeman.h"
using namespace std;

/// system_time() returns the current system time, measured in milliseconds

int system_time() {

#if defined(_WIN32) || defined(_WIN64)
  struct _timeb t;
  _ftime(&t);
  return int(t.time * 1000 + t.millitm);
#else
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000 + t.tv_usec / 1000;
#endif
}