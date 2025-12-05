#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

// We shouldn't need more than this and it's a nice even power of 2
// CHANGE THE FSCANF IF THIS CHANGES
#define LINE_BUFFER_SIZE 128

uint8_t findLargestP1(uint8_t* numList, int length) {
  uint8_t firstNum = numList[0], secondNum = 0;

  for (int i = 1; i < length; i++) {
    // if this number should be at the front and we can have a second number
    if (numList[i] > firstNum && i != length-1) {
      firstNum = numList[i];
      secondNum = numList[i+1];
    }
    // if this number is larger than secondNum, replace it
    else if (numList[i] > secondNum) {
      // if the former secondNum is larger than firstNum, replace it
      if (secondNum > firstNum) firstNum = secondNum;
      secondNum = numList[i];
    }
  }

  return firstNum*10 + secondNum;
}

uint64_t bufferToNum(uint8_t* buf, int length) {
  uint64_t num = 0;
  uint64_t factor = 1; // 10^0
  for (int i = length - 1; i >= 0; i--) {
    num += buf[i]*factor;
    factor *= 10;
  }

  return num;
}

#define NO_DROP -1
void dropNumber(uint8_t* numList, int length, int dropSpot, int potentialNewNumber) {
  if (dropSpot == NO_DROP) return;
  // slide everything from toDrop+1 over one, then append currentDigit
  for (int i = dropSpot; i < length-1; i++) {
    numList[i] = numList[i+1];
  }
  numList[length-1] = potentialNewNumber;
}

int determineDropNumber(uint8_t* oldNumList, int length, int potentialNewNumber) {
  size_t sizeofList = length*sizeof(uint8_t);
  uint8_t* numList = malloc(sizeofList);
  int whereToDrop = NO_DROP;
  memcpy(numList, oldNumList, sizeofList);
  uint64_t currentMax = bufferToNum(numList, length);

  for (int i = 0; i < length; i++) {
    // try dropping here
    dropNumber(numList, length, i, potentialNewNumber);
    uint64_t curNum = bufferToNum(numList, length);
    if (curNum > currentMax) {
      whereToDrop = i;
      currentMax = curNum;
    }

    memcpy(numList, oldNumList, sizeofList); // reset it
  }

  free(numList);
  return whereToDrop;
}

// outLength is how many digits are gonna be in the output
uint64_t findLargestP2(uint8_t* numList, int inLength) {
  int outLength = 12;
  assert(inLength >= outLength);

  uint8_t* outBuf = malloc(outLength * sizeof(uint8_t));

  int pos = 0;
  // fill outBuf with the first digits
  for (int i = 0; i < outLength; i++) {
    outBuf[i] = numList[i];
    pos++;
  }

  // for all the extra digits
  for (; pos <= inLength; pos++) {
    int currentDigit = numList[pos];

    dropNumber(outBuf, outLength, determineDropNumber(outBuf, outLength, currentDigit), currentDigit);
  }

  // turn the buffer into a real number
  uint64_t final = bufferToNum(outBuf, outLength);
  free(outBuf);
  return final;
}

int main(int argc, char* argv[]) {
  assert(argc >= 2);
  FILE* rawBuffer = fopen(argv[1], "r");
  assert(rawBuffer != NULL);

  char lineBuf[LINE_BUFFER_SIZE] = "";
  uint8_t numBuf[LINE_BUFFER_SIZE];

  int length = 0, p1Sum = 0;
  uint64_t p2Sum = 0;

  while (fscanf(rawBuffer, "%127s%n\n", lineBuf, &length) == 1) {
    --length; // accounting for 1-indexed length
    for (int i = 0; lineBuf[i] != '\0' && i < LINE_BUFFER_SIZE; i++) {
      numBuf[i] = lineBuf[i] - '0';
      // printf("numBuf[%d/%d]: %d\n", i, length, numBuf[i]);
    }

    p1Sum += findLargestP1(numBuf, length);
    p2Sum += findLargestP2(numBuf, length);
  }

  printf("correct P1 sum is %d\n", p1Sum);
  printf("P2 sum is %" PRIu64 "\n", p2Sum);
  return 0;
}
