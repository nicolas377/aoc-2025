#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// UINT64_MAX is 20 characters long in base 10, we'll never need more than that
#define MAX_NUM_BUFFER 21

// recall length doesn't include the \0 at the end
bool checkIfDuplicate(char* buffer, int bufLength, int chunkSize) {
  int prevChunkStart = 0;

  // Start from chunk 2 and check chunk 1 vs 2
  for (int i = 1; i < bufLength/chunkSize; i++) {
    // printf("\n");
    if (memcmp(buffer + prevChunkStart, buffer + i*chunkSize, chunkSize) != 0) return false;
    prevChunkStart += chunkSize;
  }

  return true;
}

bool isInvalidP1(uint64_t num) {
  char str[MAX_NUM_BUFFER];
  int length = snprintf(str, MAX_NUM_BUFFER, "%llu", num);

  if (length % 2 != 0) return false;

  return checkIfDuplicate(str, length, length/2);
}

bool isInvalidP2(uint64_t num) {
  char str[MAX_NUM_BUFFER];
  int length = snprintf(str, MAX_NUM_BUFFER, "%llu", num);

  for (int i = 1; i <= length/2; i++) {
    // printf("%s of length %d - splitting into chunks of %d\n", str, length, i);
    if (length % i == 0 && checkIfDuplicate(str, length, i)) return true;  
  }

  return false;
}

int main(int argc, char* argv[]) {
  assert(argc >= 2);
  FILE* rawBuffer = fopen(argv[1], "r");
  assert(rawBuffer != NULL);

  uint64_t lo, hi, totalP1 = 0, totalP2 = 0;
  while (fscanf(rawBuffer, "%lld-%lld,", &lo, &hi) != EOF) {
    // printf("Range of lo %lld and hi %lld\n", lo, hi);
    for (uint64_t i = lo; i <= hi; i++) {
      if (i > UINT64_MAX - totalP1) fprintf(stderr, "OVERFLOW DETECTED AT %lld", i);
      if (isInvalidP1(i)) {
        totalP1 += i;
        // printf("%lld is invalid for P1\n", i);
      }
      if (isInvalidP2(i)) {
        totalP2 += i;
        // printf("%lld is invalid for P2\n", i);
      }
    }
  }

  printf("Part 1 total - %lld\nPart 2 total - %lld", totalP1, totalP2);
  fclose(rawBuffer);
}
