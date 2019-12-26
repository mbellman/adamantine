#include <cstdlib>
#include <ctime>
#include <cmath>

#include "subsystem/RNG.h"

void RNG::seed() {
  srand(time(0));
}

float RNG::random() {
  return (rand() % 1000) / 1000.0f;
}

float RNG::random(float low, float high) {
  float range = high - low;

  return low + floorf(range * random() + 1.0f);
}