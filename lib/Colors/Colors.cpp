#include "Colors.hpp"

float getGammaCorrected(uint8_t value, double gammaValue) {
 return pow( (float) value / 255.0, gammaValue) * 255.0 + 0.5;
}

Color* RGBWColor::getIntermediateColor(Color* otherColor, double mix) {
    auto other = (RGBWColor*) otherColor;
    uint16_t c_r = this->r * (1 - mix) + other->r * mix;
    uint16_t c_g = this->g * (1 - mix) + other->g * mix;
    uint16_t c_b = this->b * (1 - mix) + other->b * mix;
    uint16_t c_w = this->w * (1 - mix) + other->w * mix;

    return new RGBWColor(c_r, c_g, c_b, c_w);
}

Color* RGBColor::getIntermediateColor(Color* otherColor, double mix) {
    auto other = (RGBColor*) otherColor;
    uint16_t c_r = this->r * (1 - mix) + other->r * mix;
    uint16_t c_g = this->g * (1 - mix) + other->g * mix;
    uint16_t c_b = this->b * (1 - mix) + other->b * mix;

    return new RGBColor(c_r, c_g, c_b);
}

Color* HSIColor::getIntermediateColor(Color* otherColor, double mix) {
    auto other = (HSIColor*) otherColor;
    float hueDistance = abs(other->h - this->h);
    if (this->h > other->h) {
        float newDistance = abs((other->h + 360) - this->h);
        if (newDistance < hueDistance) other->h += 360;
    } else {
        float newDistance = abs(other->h - (this->h + 360));
        if (newDistance < hueDistance) this->h += 360;
    }

    float h = this->h * (1 - mix) + other->h * mix;
    float s = this->s * (1 - mix) + other->s * mix;
    float i = this->i * (1 - mix) + other->i * mix;

    return new HSIColor(h, s, i);
}

RGBWColor* RGBWColor::getCorrected() {
  return new RGBWColor(
    getGammaCorrected(this->r, gammaValues.r),
    getGammaCorrected(this->g, gammaValues.g),
    getGammaCorrected(this->b, gammaValues.b),
    getGammaCorrected(this->w, gammaValues.w)
  );
}
