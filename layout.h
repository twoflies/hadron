// Layout

#ifndef _LAYOUT_H_
#define _LAYOUT_H_

#include <map>

#include "container.h"

namespace HA {

  enum LayoutFlags {
    None = 0x0,
    XProportional = 0x01,
    YProportional = 0x02,
    Proportional = XProportional | YProportional,

    NoChange = 0x10
  };

  struct WidgetLayout;

  class Layout : public Container {
  public:
    Layout();
    virtual ~Layout();
    void add(Gtk::Widget& widget, double x = 0.5, double y = 0.5, LayoutFlags flags = Proportional);
    void update(Gtk::Widget* widget, double x, double y, LayoutFlags flags = NoChange);
    double getWidgetX(Gtk::Widget* widget) const;
    double getWidgetY(Gtk::Widget* widget) const;
    LayoutFlags getWidgetLayoutFlags(Gtk::Widget* widget) const;

  protected:
    virtual void on_size_allocate(Gtk::Allocation& allocation);
    virtual void on_remove(Gtk::Widget* child);

  private:
    void resolveWidgetAllocation(Gtk::Widget* widget, WidgetLayout* layout, Gtk::Allocation& allocation);
    WidgetLayout* getWidgetLayout(Gtk::Widget* widget) const;
    void removeWidgetLayout(Gtk::Widget* widget);
 
  private:
    std::map<Gtk::Widget*, WidgetLayout*> layouts_;
  };
}

#endif // _LAYOUT_H_
