// Test

#include "test.h"

#include <string>

#include "font.h"

#include "gtkmm/button.h"

namespace HA {

  const double STATUS_BUTTON_RADIUS = 300.0;
  const double NODE_BUTTON_RADIUS = 50.0;
  const double NODE_BUTTON_PADDING = 50.0;

  TestWindow::TestWindow() {
    override_background_color(Gdk::RGBA("#000000"));
    fullscreen();

    double statusButtonX = (2 * NODE_BUTTON_RADIUS) + (2 * NODE_BUTTON_PADDING);
    statusButton_.setLabel("Disarmed");
    statusButton_.setRadius(STATUS_BUTTON_RADIUS);
    statusButton_.signal_clicked().connect(sigc::mem_fun(*this, &TestWindow::on_clicked));
    layout_.add(statusButton_, statusButtonX, 0.5, LayoutFlags::YProportional);

    nodeButton_.setLabel("BD");
    nodeButton_.setRadius(NODE_BUTTON_RADIUS);
    layout_.add(nodeButton_, NODE_BUTTON_PADDING, 0.5, LayoutFlags::YProportional);

    add(layout_);
    
    show_all_children();

    add_events(Gdk::KEY_PRESS_MASK);
  }
  
  TestWindow::~TestWindow() {
  }

  bool TestWindow::on_key_press_event(GdkEventKey* event) {
    if (event->keyval == GDK_KEY_Escape) {
      hide();
      return true;
    }
    else if (event->keyval == GDK_KEY_g) {
      layout_.moveWidgetTo(&statusButton_, 500, 500, Easing::Linear);
			   
      return true;
    }
    
    return Gtk::Window::on_key_press_event(event);
  }

  void TestWindow::on_clicked(int data) {
    layout_.moveWidgetTo(&statusButton_, 800, 0.5, 500, Easing::QuintInOut);
  }
}

int main(int argc, char** argv) {
  using namespace HA;

  FontCache::initialize("./res/");
  
  Glib::RefPtr<Gtk::Application> application = Gtk::Application::create(argc, argv, "org.twoflies.hadron");

  TestWindow testWindow;
  return application->run(testWindow);
}
