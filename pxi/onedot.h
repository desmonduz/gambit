// File: onedot.h -- header file for onedot.cc
// The program takes a data file generated by PXI (or others) and filters the
// data so that only one N-D point is recorded per value of lambda.  The point
// is chosen based on the distance from ideal point recorded in the last column
// off the data file.
#include <stdio.h>
#include "gambitio.h"
#include "gtuple.h"
#include "general.h"
#include "equdata.h"
#include "equtrac.h"

class	OneDot
{
int								num_infosets;
gTuple<DataLine>	best_point;
DataLine 					probs;

void	CopyHeader(gInput &in,gOutput &out);
public:
	OneDot(void);
	void Go(const char *infile,const char *outfile);
};

