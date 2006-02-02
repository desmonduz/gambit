//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Gambit frontend to Gametracer global Newton method
//
// This file is part of Gambit
// Copyright (c) 2005, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include <unistd.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "libgambit/libgambit.h"

#include "nfgame.h"
#include "gnmgame.h"
#include "gnm.h"

// GNM CONSTANTS
#define STEPS 100
#define FUZZ 1e-12
#define LNMFREQ 3
#define LNMMAX 10
#define LAMBDAMIN -10.0
#define WOBBLE 0
#define THRESHOLD 1e-2

int g_numDecimals = 6;

void PrintProfile(std::ostream &p_stream,
		  const Gambit::Game &p_game, cvector *p_profile)
{
  p_stream.setf(std::ios::fixed);
  p_stream << "NE";
  for (int i = 0; i < p_game->MixedProfileLength(); i++) {
    p_stream << "," << std::setprecision(g_numDecimals) << (*p_profile)[i];
  }
  p_stream << std::endl;
}

void PrintBanner(std::ostream &p_stream)
{
  p_stream << "Compute Nash equilibria using a global Newton method\n";
  p_stream << "Gametracer version 0.2, Copyright (C) 2002, Ben Blum and Christian Shelton\n";
  p_stream << "Gambit version " VERSION ", Copyright (C) 2005, The Gambit Project\n";
  p_stream << "This is free software, distributed under the GNU GPL\n\n";
}

void PrintHelp(char *progname)
{
  PrintBanner(std::cerr);
  std::cerr << "Usage: " << progname << " [OPTIONS]\n";
  std::cerr << "Accepts game on standard input.\n";

  std::cerr << "Options:\n";
  std::cerr << "  -d DECIMALS      show equilibria as floating point with DECIMALS digits\n";
  std::cerr << "  -h               print this help message\n";
  std::cerr << "  -q               quiet mode (suppresses banner)\n";
  exit(1);
}

void Solve(const Gambit::Game &p_game)
{
  int i;

  int *actions = new int[p_game->NumPlayers()];
  int veclength = p_game->NumPlayers();
  for (int pl = 1; pl <= p_game->NumPlayers(); pl++) {
    actions[pl-1] = p_game->GetPlayer(pl)->NumStrategies();
    veclength *= p_game->GetPlayer(pl)->NumStrategies();
  }
  cvector payoffs(veclength);
  
  gnmgame *A = new nfgame(p_game->NumPlayers(), actions, payoffs);
  
  int *profile = new int[p_game->NumPlayers()];
  for (Gambit::StrategyIterator iter(p_game); !iter.AtEnd(); iter++) {
    for (int pl = 1; pl <= p_game->NumPlayers(); pl++) {
      profile[pl-1] = iter->GetStrategy(pl)->GetNumber() - 1;
    }

    for (int pl = 1; pl <= p_game->NumPlayers(); pl++) {
      A->setPurePayoff(pl-1, profile, iter->GetPayoff<double>(pl));
    }
  }

  cvector g(A->getNumActions()); // choose a random perturbation ray
  int numEq;

  cvector **answers;
  do {
    for(i = 0; i < A->getNumActions(); i++) {
#if !defined(HAVE_DRAND48)
      g[i] = rand();
#else
      g[i] = drand48();
#endif  // HAVE_DRAND48
    }
    g /= g.norm(); // normalized
    numEq = GNM(*A, g, answers, STEPS, FUZZ, LNMFREQ, LNMMAX, LAMBDAMIN, WOBBLE, THRESHOLD);
  } while(numEq == 0);

  for (i = 0; i < numEq; i++) {
    PrintProfile(std::cout, p_game, answers[i]);
    free(answers[i]);
  }

  free(answers);

  delete A;
}

int main(int argc, char *argv[])
{
  opterr = 0;
  bool quiet = false;

  int c;
  while ((c = getopt(argc, argv, "d:qhS")) != -1) {
    switch (c) {
    case 'q':
      quiet = true;
      break;
    case 'd':
      g_numDecimals = atoi(optarg);
      break;
    case 'S':
      break;
    case 'h':
      PrintHelp(argv[0]);
      break;
    case '?':
      if (isprint(optopt)) {
	std::cerr << argv[0] << ": Unknown option `-" << ((char) optopt) << "'.\n";
      }
      else {
	std::cerr << argv[0] << ": Unknown option character `\\x" << optopt << "`.\n";
      }
      return 1;
    default:
      abort();
    }
  }

  if (!quiet) {
    PrintBanner(std::cerr);
  }

  try {
    Gambit::Game game = Gambit::ReadGame(std::cin);

    game->BuildComputedValues();

    Solve(game);
    return 0;
  }
  catch (Gambit::InvalidFileException) {
    std::cerr << "Error: Game not in a recognized format.\n";
    return 1;
  }
  catch (...) {
    std::cerr << "Error: An internal error occurred.\n";
    return 1;
  }
}
