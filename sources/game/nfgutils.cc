//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Utility functions for normal form games
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
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

#include "base/gmisc.h"
#include "nfg.h"
#include "nfstrat.h"
#include "nfgciter.h"


gbtNfgGame CompressNfg(const gbtNfgGame &nfg, const gbtNfgSupport &S)
{
  gbtNfgGame N(S.NumStrats());
  N.SetTitle(nfg.GetTitle());

  for (int pl = 1; pl <= N.NumPlayers(); pl++)  {
    gbtNfgPlayer player = N.GetPlayer(pl);
    player.SetLabel(nfg.GetPlayer(pl).GetLabel());
    for (int st = 1; st <= N.NumStrats(pl); st++) {
      player.GetStrategy(st).SetLabel(S.GetStrategy(pl, st).GetLabel());
    }
  }

  for (int outc = 1; outc <= nfg.NumOutcomes(); outc++)  {
    gbtNfgOutcome outcome = N.NewOutcome();

    outcome.SetLabel(nfg.GetOutcome(outc).GetLabel());

    for (int pl = 1; pl <= N.NumPlayers(); pl++) {
      outcome.SetPayoff(nfg.GetPlayer(pl),
			nfg.GetOutcome(outc).GetPayoff(nfg.GetPlayer(pl)));
    }
  }

  gbtNfgContIterator oiter(S);
  gbtNfgSupport newS(N);
  gbtNfgContIterator niter(newS);
  
  do   {
    if (!oiter.GetOutcome().IsNull()) {
      niter.SetOutcome(N.GetOutcome(oiter.GetOutcome().GetId()));
    }
    else {
      niter.SetOutcome(0);
    }

    oiter.NextContingency();
  }  while (niter.NextContingency());

  return N;
}

gNumber MinPayoff(const gbtNfgGame &nfg, int player)
{
  int index, p, p1, p2;
  gNumber minpay;
  
  if (nfg.NumOutcomes() == 0)  return (gNumber) 0;

  if (player)
    p1 = p2 = player;
  else   {
    p1 = 1;
    p2 = nfg.NumPlayers();
  }

  minpay = nfg.GetOutcome(1).GetPayoff(nfg.GetPlayer(p1));
  for (index = 2; index <= nfg.NumOutcomes(); index++)  {
    for (p = p1; p <= p2; p++)
      if (nfg.GetOutcome(index).GetPayoff(nfg.GetPlayer(p)) < minpay)
	minpay = nfg.GetOutcome(index).GetPayoff(nfg.GetPlayer(p));
  }
  return minpay;
}

gNumber MaxPayoff(const gbtNfgGame &nfg, int player)
{
  int index, p, p1, p2;
  gNumber maxpay;

  if (nfg.NumOutcomes() == 0)  return (gNumber) 0;

  if (player)
    p1 = p2 = player;
  else   {
    p1 = 1;
    p2 = nfg.NumPlayers();
  }

  maxpay = nfg.GetOutcome(1).GetPayoff(nfg.GetPlayer(p1));
  for (index = 2; index <= nfg.NumOutcomes(); index++)  {
    for (p = p1; p <= p2; p++)
      if (nfg.GetOutcome(index).GetPayoff(nfg.GetPlayer(p)) > maxpay)
	maxpay = nfg.GetOutcome(index).GetPayoff(nfg.GetPlayer(p));
  }
  return maxpay;
}

void RandomNfg(gbtNfgGame nfg)
{
  for (int pl = 1; pl <= nfg.NumPlayers(); pl++)
    for (int outc = 1; outc <= nfg.NumOutcomes(); outc++)
      nfg.GetOutcome(outc).SetPayoff(nfg.GetPlayer(pl), gNumber(Uniform()));
}  



