// Widget

#include "widget.h"

namespace HA {

  const int MINIMUM_WIDTH = 100;
  const int MINIMUM_HEIGHT = 100;
  const int NATURAL_WIDTH = 200;
  const int NATURAL_HEIGHT = 200;

  Widget::Widget() : Glib::ObjectBase("ha_widget"), Gtk::Widget() {
    set_has_window(true);
  }

  Widget::~Widget() {
  }

  Gtk::SizeRequestMode Widget::get_request_mode_vfunc() const {
    return Gtk::Widget::get_request_mode_vfunc();
  }

  void Widget::get_preferred_width_vfunc(int& minimum_width, int& natural_width) const {
    minimum_width = MINIMUM_WIDTH;
    natural_width = NATURAL_WIDTH;
  }

  void Widget::get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const {
    minimum_height = MINIMUM_HEIGHT;
    natural_height = NATURAL_HEIGHT;
  }

  void Widget::get_preferred_height_vfunc(int& minimum_height, int& natural_height) const {
    minimum_height = MINIMUM_HEIGHT;
    natural_height = NATURAL_HEIGHT;
  }

  void Widget::get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const {
    minimum_width = MINIMUM_WIDTH;
    natural_width = MINIMUM_HEIGHT;
  }

  void Widget::on_size_allocate(Gtk::Allocation& allocation) {
    set_allocation(allocation);

    if (gdkWindow_) {
      gdkWindow_->move_resize(allocation.get_x(), allocation.get_y(), allocation.get_width(), allocation.get_height() );
    }
  }

  void Widget::on_map() {
    Gtk::Widget::on_map();
  }

  void Widget::on_unmap() {
    Gtk::Widget::on_unmap();
  }

  void Widget::on_realize() {
    set_realized();

    if (!gdkWindow_) {
      GdkWindowAttr attributes;
      memset(&attributes, 0, sizeof(attributes));

      Gtk::Allocation allocation = get_allocation();

      attributes.x = allocation.get_x();
      attributes.y = allocation.get_y();
      attributes.width = allocation.get_width();
      attributes.height = allocation.get_height();

      attributes.event_mask = get_events() | Gdk::EXPOSURE_MASK;
      attributes.window_type = GDK_WINDOW_CHILD;
      attributes.wclass = GDK_INPUT_OUTPUT;

      gdkWindow_ = Gdk::Window::create(get_parent_window(), &attributes, GDK_WA_X | GDK_WA_Y);
      set_window(gdkWindow_);

      gdkWindow_->set_user_data(gobj());
    }
  }

  void Widget::on_unrealize() {
    if (gdkWindow_) { 
      gdkWindow_.reset();
    }

    Gtk::Widget::on_unrealize();
  }

  bool Widget::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    return true;
  }
}
