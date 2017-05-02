// Container

#include "container.h"

#include <algorithm>

namespace HA {
  Container::Container() {
    set_has_window(false);
    set_redraw_on_allocate(false);
  }

  Container::~Container() {
    for (std::vector<Gtk::Widget*>::iterator cit = children_.begin(); cit != children_.end(); cit++) {
      (*cit)->unparent();
    }
    children_.clear();
  }

  Gtk::SizeRequestMode Container::get_request_mode_vfunc() const {
    return Gtk::SIZE_REQUEST_CONSTANT_SIZE;
  }

  void Container::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const {
    minimum_width = 0;
    natural_width = 0;
    for (std::vector<Gtk::Widget*>::const_iterator cit = children_.begin(); cit != children_.end(); cit++) {
      int child_minimum_width;
      int child_natural_width;
      (*cit)->get_preferred_width(child_minimum_width, child_natural_width);

      minimum_width = std::max(minimum_width, child_minimum_width);
      natural_width = std::max(natural_width, child_natural_width);
    }
  }

  void Container::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const {
    minimum_height = 0;
    natural_height = 0;
    for (std::vector<Gtk::Widget*>::const_iterator cit = children_.begin(); cit != children_.end(); cit++) {
      int child_minimum_height;
      int child_natural_height;
      (*cit)->get_preferred_height(child_minimum_height, child_natural_height);

      minimum_height = std::max(minimum_height, child_minimum_height);
      natural_height = std::max(natural_height, child_natural_height);
    }
  }

  void Container::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const {
    minimum_height = 0;
    natural_height = 0;
    for (std::vector<Gtk::Widget*>::const_iterator cit = children_.begin(); cit != children_.end(); cit++) {
      int child_minimum_height;
      int child_natural_height;
      (*cit)->get_preferred_height(child_minimum_height, child_natural_height);

      minimum_height = std::max(minimum_height, child_minimum_height);
      natural_height = std::max(natural_height, child_natural_height);
    }
  }

  void Container::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const {
    minimum_width = 0;
    natural_width = 0;
    for (std::vector<Gtk::Widget*>::const_iterator cit = children_.begin(); cit != children_.end(); cit++) {
      int child_minimum_width;
      int child_natural_width;
      (*cit)->get_preferred_width(child_minimum_width, child_natural_width);

      minimum_width = std::max(minimum_width, child_minimum_width);
      natural_width = std::max(natural_width, child_natural_width);
    }
  }

  void Container::on_size_allocate(Gtk::Allocation& allocation) {
    set_allocation(allocation);

    std::vector<Gtk::Widget*> children_ = get_children();
    for (std::vector<Gtk::Widget*>::iterator it = children_.begin(); it != children_.end(); it++) {
      (*it)->size_allocate(allocation);
    }
  }
  
  void Container::forall_vfunc(gboolean include_internals, GtkCallback callback, gpointer callback_data) {
    for (std::vector<Gtk::Widget*>::iterator it = children_.begin(); it != children_.end(); it++) {
      callback((*it)->gobj(), callback_data);
    }
  }

  void Container::on_add(Gtk::Widget* child) {
    children_.push_back(child);
    child->set_parent(*this);
  }

  void Container::on_remove(Gtk::Widget* child) {
    std::vector<Gtk::Widget*>::iterator it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
      child->unparent();
      children_.erase(it);
    }
  }
  
  GType Container::child_type_vfunc() const {
    return Gtk::Widget::get_type();
  }

  bool Container::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    return Gtk::Container::on_draw(context);
  }
}
