// Easing

#ifndef _EASING_H_
#define _EASING_H_

namespace HA {

  enum Easing {
    Linear,
    QuintInOut
  };

  class EasingFunc {
  public:
    static double interpolateLinear(double time, double startValue, double endValue, double duration);
    static double interpolateQuintInOut(double time, double startValue, double endValue, double duration);
  };
  
}

#endif // _EASING_H_
