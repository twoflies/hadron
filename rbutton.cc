// Button

#include "rbutton.h"

#include "font.h"

namespace HA {

  RingButton::RingButton() {
    color_ = BLUE;
    labelWidget_.override_color(color_);
    ringContainer_.setColor(color_);

    labelWidget_.set_text(label_);
    ringContainer_.add(labelWidget_);

    depthBlur_.setDepth(0);
    depthBlur_.add(ringContainer_);

    add(depthBlur_);
    
    set_events(Gdk::BUTTON_PRESS_MASK);
    signal_button_press_event().connect(sigc::mem_fun(*this, &RingButton::on_pressed));
    signal_button_release_event().connect(sigc::mem_fun(*this, &RingButton::on_released));
  }

  RingButton::~RingButton() {
  }

  const std::string& RingButton::getLabel() const {
    return label_;
  }

  void RingButton::setLabel(const std::string& label) {
    label_ = label;
    labelWidget_.set_text(label_);
    calculateLayout();
    queue_resize();
  }

  double RingButton::getRadius() const {
    return ringContainer_.getRadius();
  }

  void RingButton::setRadius(double radius) {
    ringContainer_.setRadius(radius);
    calculateLayout();
    queue_resize();
  }

  const Gdk::RGBA& RingButton::getColor() const {
    return color_;
  }

  void RingButton::setColor(const Gdk::RGBA& color) {
    color_ = color;
    ringContainer_.setColor(color_);
    labelWidget_.override_color(color_);
  }

  signal_clicked_t RingButton::signal_clicked() const {
    return signal_clicked_;
  }

  void RingButton::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const {
    minimum_width = natural_width = getNaturalWidth();
  }

  void RingButton::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const {
    minimum_height = natural_height = getNaturalHeight();
  }

  void RingButton::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const {
    minimum_height = natural_height = getNaturalHeight();
  }

  void RingButton::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const {
    minimum_width = natural_width = getNaturalWidth();
  }
  
  void RingButton::on_realize() {
    Gtk::EventBox::on_realize();
    calculateLayout();
  }

  bool RingButton::on_pressed(GdkEventButton* event) {
    depthBlur_.setDepth(DEPTH_BLUR_SHALLOW);
    return true;
  }

  bool RingButton::on_released(GdkEventButton* event) {
    signal_clicked_.emit(1);
    
    depthBlur_.setDepth(0);
    return true;
  }

  int RingButton::getNaturalWidth() const {
    // TODO: Push this into RingContainer
    double radius = ringContainer_.getRadius();
    double halo = radius * ringContainer_.getHaloRatio();
    
    return (int)round(radius * 2) + (2 * halo) + (4 * DEPTH_BLUR_SHALLOW);
  }
  
  int RingButton::getNaturalHeight() const {
    return getNaturalWidth();
  }

  void RingButton::calculateLayout() {
    if ((ringContainer_.getRadius() <= 0.0) || labelWidget_.get_text().empty()) {
      return;
    }
    
    double availableWidth = ringContainer_.getRadius() * M_SQRT2;
    int size = (int)round(availableWidth / label_.size());
    labelWidget_.override_font(FontCache::getFontDescription(size));
  }
}
