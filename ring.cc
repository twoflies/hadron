// Ring Container

#include "ring.h"

#include "color.h"

namespace HA {

  const double DEFAULT_RADIUS = 50.0;
  const double DEFAULT_HALO_RATIO = 0.05;
  const double MIN_HALO = 5.0;
  const double M_PI_2X = 2 * M_PI;
  const Gdk::RGBA DEFAULT_COLOR = BLUE;

  RingContainer::RingContainer() {
    radius_ = DEFAULT_RADIUS;
    haloRatio_ = DEFAULT_HALO_RATIO;
    color_ = DEFAULT_COLOR;
  }

  RingContainer::~RingContainer() {
  }

  double RingContainer::getRadius() const {
    return radius_;
  }
  
  void RingContainer::setRadius(double radius) {
    radius_ = radius;
  }

  double RingContainer::getHaloRatio() const {
    return haloRatio_;
  }

  void RingContainer::setHaloRatio(double haloRatio) {
    haloRatio_ = haloRatio;
  }
  
  const Gdk::RGBA& RingContainer::getColor() const {
    return color_;
  }
  
  void RingContainer::setColor(const Gdk::RGBA& color) {
    color_ = color;
  }

  void RingContainer::on_size_allocate(Gtk::Allocation& allocation) {
    Container::on_size_allocate(allocation);
    
    int width = allocation.get_width();
    int height = allocation.get_height();
    int min = MIN(width, height);

    double xc = min / 2.0;
    double yc = min / 2.0;
    double halo = MAX(radius_ * haloRatio_, MIN_HALO);
    double innerRadius = radius_ - halo;
    double outerRadius = radius_ + halo;

    innerGradient_ = Cairo::RadialGradient::create(xc, yc, innerRadius, xc, yc, radius_);
    innerGradient_->add_color_stop_rgba(0.0, color_.get_red(), color_.get_green(), color_.get_blue(), 0.0);
    innerGradient_->add_color_stop_rgba(1.0, color_.get_red(), color_.get_green(), color_.get_blue(), color_.get_alpha());

    outerGradient_ = Cairo::RadialGradient::create(xc, yc, radius_, xc, yc, outerRadius);
    outerGradient_->add_color_stop_rgba(0.0, color_.get_red(), color_.get_green(), color_.get_blue(), color_.get_alpha());
    outerGradient_->add_color_stop_rgba(1.0,  color_.get_red(), color_.get_green(), color_.get_blue(), 0.0);
  }

  bool RingContainer::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    int min = MIN(width, height);

    double xc = min / 2.0;
    double yc = min / 2.0;
    double halo = MAX(radius_ * haloRatio_, MIN_HALO);
    double outerRadius = radius_ + halo;

    context->set_source(outerGradient_);
    context->arc(xc, yc, outerRadius, 0.0, M_PI_2X);
    context->fill();

    context->set_source_rgba(0.0, 0.0, 0.0, 1.0);  // TODO: Clip instead?
    context->arc(xc, yc, radius_, 0.0, M_PI_2X);
    context->fill();
    
    context->set_source(innerGradient_);
    context->arc(xc, yc, radius_, 0.0, M_PI_2X);
    context->fill();

    context->set_line_width(2.0);
    context->set_source_rgba(color_.get_red(), color_.get_green(), color_.get_blue(), color_.get_alpha());
    context->arc(xc, yc, radius_, 0.0, M_PI_2X);
    context->stroke();

    return Container::on_draw(context);
  }
}
