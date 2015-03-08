#include <cstring>
#include <cstdio>

typedef unsigned int letter;

// #define N 12  // now defined with -D at compile time

letter sequence[2*N];
letter remaining[N];
int    offset[N+1];       // for move i offset[i] is the next free slot in the sequence
int    lindex[N];         // for move i lindex[i]  is the lindex in remaining of the chosen letter  (lindex[i] = sequence(offset[i]) - 1?
int    nrChosen = 0;     // number of letters already chosen

bool makeChoiceIfPossible(int i)  // try ith entry in remaining to see if that move can be made nex, make it if so
{
  letter chosen = remaining[i];
  int o1;
  int o2;

  if (chosen == 0)             // that letter is already used in the sequence so far
    return false;

  o1 = offset[nrChosen];
  o2 = o1 + chosen + 1;

  if (o2 >= 2*N)               // the choice does have to fit
    return false;
  if (sequence[o2] != 0)       // sequence[o1] is empty, but sequence[o2] must also be
    return false;

  sequence[o1] = chosen;
  sequence[o2] = chosen;
  remaining[i] = 0;
  lindex[nrChosen] = i;
  nrChosen++;
  while (sequence[o1] != 0)
    o1++;
  offset[nrChosen] = o1;
  return true;
}

void undoChoice()
{
  nrChosen--;
  int o = offset[nrChosen];
  int i = lindex[nrChosen];
  letter chosen = sequence[o];
  sequence[o] = 0;
  sequence[o + chosen + 1] = 0;
  remaining[i] = chosen;
}

void initSequence()
{
  memset(sequence, 0, sizeof sequence);
  for (int i = 0; i < N; i++)
    remaining[i] = i+1;
  memset(offset, 0, sizeof offset);
  nrChosen = 0;
}

void print()
{
  printf("[%d", sequence[0]);
  for (int k = 1; k < 2*N; k++)
    printf(",%d", sequence[k]);
  printf("]\n");
}

void trace()
{
  printf("    %d -> [%d", nrChosen, sequence[0]);
  for (int k = 1; k < 2*N; k++)
    printf(",%d", sequence[k]);
  printf("] [%d", remaining[0]);
  for (int k = 1; k < N; k++)
    printf(",%d", remaining[k]);
  printf("] [%d", offset[0]);
  for (int k = 1; k < N; k++)
    printf(",%d", offset[k]);
  printf("] [%d", lindex[0]);
  for (int k = 1; k < N; k++)
    printf(",%d", lindex[k]);
  printf("]\n");
}

void recurse()
{
  // trace();
  
  if (nrChosen == N)
    {
      print();
    }
  else
    {
      for (int i = 0; i < N; i++)
	{
	  if (makeChoiceIfPossible(i))
	    {
	      recurse();
	    }
	}
    }

  undoChoice();
}

int main(int argc, char *argv[])
{
  initSequence();

  recurse();
  
  return 0;
}
