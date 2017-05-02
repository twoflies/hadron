// Easing

#include "easing.h"

namespace HA {

  double EasingFunc::interpolateLinear(double time, double startValue, double endValue, double duration) {
    return ((endValue - startValue) * (time / duration)) + startValue;
  }

  double EasingFunc::interpolateQuintInOut(double time, double startValue, double endValue, double duration) {
    time /= (duration / 2);
    double value = endValue - startValue;
    if (time < 1) {
      return ((value / 2) * time * time * time * time * time) + startValue;
    }
    
    time -= 2;
    return (value / 2) * ((time * time * time * time * time) + 2) + startValue;
  }
  
}
