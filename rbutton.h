// Button

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <string>

#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>

#include "depthblur.h"
#include "ring.h"
#include "color.h"

namespace HA {

  typedef sigc::signal<void, int> signal_clicked_t;

  class RingButton : public Gtk::EventBox {
  public:
    RingButton();
    virtual ~RingButton();
    const std::string& getLabel() const;
    void setLabel(const std::string& label);
    double getRadius() const;
    void setRadius(double radius);
    const Gdk::RGBA& getColor() const;
    void setColor(const Gdk::RGBA& color);
    signal_clicked_t signal_clicked() const;

  protected:
    virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;
    virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
    virtual void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
    virtual void on_realize();
    virtual bool on_pressed(GdkEventButton* event);
    virtual bool on_released(GdkEventButton* event);

  private:
    int getNaturalWidth() const;
    int getNaturalHeight() const;
    void calculateLayout();

  private:
    std::string label_;
    Gdk::RGBA color_;
    Gtk::Label labelWidget_;
    RingContainer ringContainer_;
    DepthBlur depthBlur_;
    signal_clicked_t signal_clicked_;
  };
  
}

#endif // _BUTTON_H
