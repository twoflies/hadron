// Layout

#include "layout.h"

#include <iostream>

namespace HA {

  inline LayoutFlags operator|(LayoutFlags a, LayoutFlags b) {
    return static_cast<LayoutFlags>(static_cast<int>(a) | static_cast<int>(b));
  }

  struct WidgetLayout {
    double x;
    double y;
    LayoutFlags flags;

    WidgetLayout(double x, double y, LayoutFlags flags) {
      this->x = x;
      this->y = y;
      this->flags = flags;
    }
  };
  
  Layout::Layout() {
  }

  Layout::~Layout() {
    for (std::map<Gtk::Widget*, WidgetLayout*>::iterator it = layouts_.begin(); it != layouts_.end(); it++) {
      delete it->second;
    }
    layouts_.clear();
  }
  
  void Layout::add(Gtk::Widget& widget, double x, double y, LayoutFlags flags) {
    Container::add(widget);

    layouts_[&widget] = new WidgetLayout(x, y, flags);
  }

  void Layout::update(Gtk::Widget* widget, double x, double y, LayoutFlags flags) {
    WidgetLayout* layout = getWidgetLayout(widget);
    if (layout == NULL) {
      return;
    }

    layout->x = x;
    layout->y = y;
    if (flags != NoChange) {
      layout->flags = flags;
    }
  }

  double Layout::getWidgetX(Gtk::Widget* widget) const {
    WidgetLayout* layout = getWidgetLayout(widget);
    if (layout == NULL) {
      return -1;
    }

    return layout->x;
  }

  double Layout::getWidgetY(Gtk::Widget* widget) const {
    WidgetLayout* layout = getWidgetLayout(widget);
    if (layout == NULL) {
      return -1;
    }

    return layout->y;
  }

  LayoutFlags Layout::getWidgetLayoutFlags(Gtk::Widget* widget) const {
    WidgetLayout* layout = getWidgetLayout(widget);
    if (layout == NULL) {
      return LayoutFlags::None;
    }

    return layout->flags;
  }

  void Layout::on_size_allocate(Gtk::Allocation& allocation) {
    set_allocation(allocation);

    for (std::vector<Gtk::Widget*>::iterator it = children_.begin(); it != children_.end(); it++) {
      Gtk::Widget* widget = *it;
      WidgetLayout* layout = getWidgetLayout(widget);
      if (layout == NULL) {
	widget->size_allocate(allocation);
	continue;
      }

      Gtk::Allocation widgetAllocation(allocation);
      resolveWidgetAllocation(widget, layout, widgetAllocation);
      widget->size_allocate(widgetAllocation);
    }
  }

  void Layout::on_remove(Gtk::Widget* child) {
    Container::on_remove(child);

    std::map<Gtk::Widget*, WidgetLayout*>::iterator it = layouts_.find(child);
    if (it != layouts_.end()) {
      delete it->second;
      layouts_.erase(it);
    }
  }

  void Layout::resolveWidgetAllocation(Gtk::Widget* widget, WidgetLayout* layout, Gtk::Allocation& allocation) {
    int minimum_width;
    int natural_width;
    widget->get_preferred_width(minimum_width, natural_width);
    int width = MAX(MIN(natural_width, allocation.get_width()), minimum_width);
    int minimum_height;
    int natural_height;
    widget->get_preferred_height_for_width(width, minimum_height, natural_height);
    int height = MAX(MIN(natural_height, allocation.get_height()), minimum_height);
    int x;
    if ((layout->flags & XProportional) == XProportional) {
      x = (int)round((allocation.get_width() * layout->x) - (width / 2.0));
    }
    else {
      x = (int)round(layout->x);
    }

    int y;
    if ((layout->flags & YProportional) == YProportional) {
      y = (int)round((allocation.get_height() * layout->y) - (height / 2.0));
    }
    else {
      y = (int)round(layout->y);
    }

    allocation.set_x(x);
    allocation.set_y(y);
    allocation.set_width(width);
    allocation.set_height(height);
  }

  WidgetLayout* Layout::getWidgetLayout(Gtk::Widget* widget) const {
    std::map<Gtk::Widget*, WidgetLayout*>::const_iterator cit = layouts_.find(widget);
    return (cit != layouts_.end()) ? cit->second : NULL;
  }
  
  void Layout::removeWidgetLayout(Gtk::Widget* widget) {
    std::map<Gtk::Widget*, WidgetLayout*>::iterator it = layouts_.find(widget);
    if (it != layouts_.end()) {
      layouts_.erase(it);
    }
  }
}
