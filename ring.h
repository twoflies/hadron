// Ring

#ifndef _RING_H_
#define _RING_H_

#include "container.h"

namespace HA {

  class RingContainer : public Container {
  public:
    RingContainer();
    virtual ~RingContainer();
    double getRadius() const;
    void setRadius(double radius);
    double getHaloRatio() const;
    void setHaloRatio(double haloRatio);
    const Gdk::RGBA& getColor() const;
    void setColor(const Gdk::RGBA& color);

  protected:
    virtual void on_size_allocate(Gtk::Allocation& allocation);
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& context);

  private:
    double radius_;
    double haloRatio_;
    Gdk::RGBA color_;
    Cairo::RefPtr<Cairo::RadialGradient> innerGradient_;
    Cairo::RefPtr<Cairo::RadialGradient> outerGradient_;
  };
  
}

#endif // _RING_H_
