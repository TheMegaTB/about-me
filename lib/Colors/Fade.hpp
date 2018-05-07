#ifndef FADE_HPP
#define FADE_HPP

#include <Arduino.h>
#include "Colors.hpp"

class ColorFade {
  // Fade configuration
  Color* from;
  Color* to;
  uint16_t duration;

  // Fade state
  Color* current;
  uint16_t time;

public:
  bool completed;

  ColorFade(Color* from, Color* to, uint16_t duration) : from(from), to(to), duration(duration), current(from), time(0) {}

  Color* step(uint16_t stepSize) {
    double progress =  ((double) this->time) / ((double) this->duration);

    // TODO This may cause memory leaks since the previous this->current is kept in memory
    // TODO MARK Source of evil #1
    // this->current = this->from->getIntermediateColor(this->to, min(progress, 1.0));

    this->completed = this->time >= this->duration;

    this->time += stepSize;
    return this->current;
  }

  void reset() {
      this->time = 0;
      this->current = this->from;
  }
};

#endif
