#include "Stats.h"
#include "SDL.h"

int Stats::getFPS() {
  int delta = frame.end - frame.start;

  return delta == 0
    ? 1000
    : (int)(1000.0f / (frame.end - frame.start));
}

void Stats::trackFrameStart() {
  frame.start = SDL_GetTicks();
}

void Stats::trackFrameEnd() {
  frame.end = SDL_GetTicks();
}