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
// D.A.G., 03/18/2012: timeman.h is a header for declarations 
// having to do with time management. In a game with limited time
// for thinking, the program has to decide when to stop thinking
// and make the current best move. However, LegalMoveGen does not
// do that yet. Thus timeman.h merely helps make sure that we can invoke
// system_time() in void benchmark(int argc, char* argv[]) 
// and measure how long the work of benchmark takes.
// It comes fifth in a series of headers: chess.h -> bitboard.h ->
// position.h -> search.h -> timeman.h -> (thread.h)...

#if defined(_WIN32) || defined(_WIN64)

#define _CRT_SECURE_NO_DEPRECATE
#define NOMINMAX // disable macros min() and max()
#include <windows.h>
#include <sys/timeb.h>

#else

#  include <sys/time.h>
#  include <sys/types.h>
#  include <unistd.h>
#  if defined(__hpux)
#     include <sys/pstat.h>
#  endif

#endif

#if !defined(NO_PREFETCH)
#  include <xmmintrin.h>
#endif

//#include <algorithm>
//#include <iomanip>
//#include <iostream>
//#include <sstream>

#include "search.h"

extern int system_time();