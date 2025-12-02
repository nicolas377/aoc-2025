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
    if (direction == 'R') dial += toDial;
    else dial -= toDial;

    while (dial < 0) dial += 100;
    while (dial >= 100) dial -= 100;

    printf("Dial is: %d after a change of %c%d\n", dial, direction, toDial);
    if (dial == 0) numZeros++;
  }

  printf("%d\n", numZeros);
}
