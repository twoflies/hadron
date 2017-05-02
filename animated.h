// Animated Layout

#ifndef _ANIMATED_H_
#define _ANIMATED_H_

#include <map>
#include <pthread.h>

#include "layout.h"
#include "easing.h"

namespace HA {

  struct Animation;

  const long DEFAULT_DURATION = 1000;
  const Easing DEFAULT_EASING = Easing::Linear;

  class AnimatedLayout : public Layout {
  public:
    AnimatedLayout();
    ~AnimatedLayout();
    Animation* moveWidgetTo(Gtk::Widget* widget, double x, double y, long duration = DEFAULT_DURATION, Easing easing = DEFAULT_EASING);
    
  private:
    void addCallback();
    void removeCallback();
    static gboolean tick_(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer user_data);
    gboolean tick(GtkWidget* widget, GdkFrameClock* frame_clock);
    static void tick_destroy(gpointer data);
    
  private:
    pthread_mutex_t tickMutex_;
    guint callbackId_;
    std::map<Animation*, Animation*> animations_;
  };
  
}

#endif // _ANIMATED_H_
