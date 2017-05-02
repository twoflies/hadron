// Container

#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <vector>

#include "gtkmm/container.h"

#include "widget.h"

namespace HA {

  class Container : public Gtk::Container {
  public:
    Container();
    virtual ~Container();

  protected:
    virtual Gtk::SizeRequestMode get_request_mode_vfunc() const;
    virtual void get_preferred_width_vfunc(int& minimum_width, int& natural_width) const;
    virtual void get_preferred_height_for_width_vfunc(int width, int& minimum_height, int& natural_height) const;
    virtual void get_preferred_height_vfunc(int& minimum_height, int& natural_height) const;
    virtual void get_preferred_width_for_height_vfunc(int height, int& minimum_width, int& natural_width) const;
    virtual void on_size_allocate(Gtk::Allocation& allocation);
    virtual void forall_vfunc(gboolean include_internals, GtkCallback callback, gpointer callback_data);
    virtual void on_add(Gtk::Widget* child);
    virtual void on_remove(Gtk::Widget* child);
    virtual GType child_type_vfunc() const;
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& context);
    
  protected:
    std::vector<Gtk::Widget*> children_;
  };
  
}

#endif // _CONTAINER_H_
