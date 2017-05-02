// Test

#ifndef _TEST_H_
#define _TEST_H_

#include <gtkmm/window.h>

#include "animated.h"
#include "rbutton.h"

namespace HA {

  class TestWindow : public Gtk::Window {
  public:
    TestWindow();
    ~TestWindow();
    bool on_key_press_event(GdkEventKey* event);
    void on_clicked(int data);

  private:
    AnimatedLayout layout_;
    RingButton statusButton_;
    RingButton nodeButton_;
  };
  
}

#endif // _TEST_H
