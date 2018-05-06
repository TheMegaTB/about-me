#ifndef FADE_HPP
#define FADE_HPP

#include <Arduino.h>

template<typename C>
class ColorFade {
  // Fade configuration
  C from;
  C to;
  uint16_t duration;

  // Fade state
  C current;
  uint16_t time;

public:
  bool completed;

  ColorFade(C from, C to, uint16_t duration) : from(from), to(to), duration(duration), current(from), time(0) {}

  C step(uint16_t stepSize) {
    double progress =  ((double) this->time) / ((double) this->duration);
    this->current = this->from.getIntermediateColor(this->to, min(progress, 1.0));

    this->completed = this->time >= this->duration;

    this->time += stepSize;
    return this->current;
  }
};

#endif
