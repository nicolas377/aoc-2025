#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// right moves increase the dial
int main(int argc, char* argv[]) {
  int numZeros = 0;
  int dial = 50;

  FILE* moves = fopen(argv[1], "r");

  assert(moves != NULL);

  int toDial;
  char direction;

  while (fscanf(moves, "%c%d\n", &direction, &toDial) != EOF) {
    for (int i=1; i<=toDial; ++i) {
      if (direction == 'R') dial = (dial - 1 + 100) % 100;
      else dial = (dial + 1) % 100;

      if (dial == 0) numZeros++;
    }

    // only count a zero if a click lands on it during or at the end of a turn
    // a click lands on it during a turn if:
    //   we have to reorient a number
    //   we need to subtract one if we started at 0 and then underflowed

    // printf("Dial is: %d after a change of %c%d\n", dial, direction, toDial);
  }

  printf("%d\n", numZeros);
}
