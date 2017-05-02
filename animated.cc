// Animated Layout

#include "animated.h"

namespace HA {

  struct Animation {
    Gtk::Widget* widget;
    gint64 startTime;
    double startX;
    double startY;
    double endX;
    double endY;
    long duration;
    Easing easing;
  };

  AnimatedLayout::AnimatedLayout() {
    pthread_mutex_init(&tickMutex_, NULL);
  }

  AnimatedLayout::~AnimatedLayout() {
  }

  Animation* AnimatedLayout::moveWidgetTo(Gtk::Widget* widget, double x, double y, long duration, Easing easing) {
    Animation* animation = new Animation();
    animation->widget = widget;
    animation->startTime = g_get_monotonic_time();
    animation->startX = getWidgetX(widget);
    animation->startY = getWidgetY(widget);
    animation->endX = x;
    animation->endY = y;
    animation->duration = duration;
    animation->easing = easing;

    pthread_mutex_lock(&tickMutex_);
    
    animations_[animation] = animation;
    if (animations_.size() == 1) {
      addCallback();
    }

    pthread_mutex_unlock(&tickMutex_);
    
    return animation;
  }

  void AnimatedLayout::addCallback() {
    callbackId_ = gtk_widget_add_tick_callback((GtkWidget*)this->gobj(), &AnimatedLayout::tick_, this, &AnimatedLayout::tick_destroy);
  }

  void AnimatedLayout::removeCallback() {
    gtk_widget_remove_tick_callback((GtkWidget*)this->gobj(), callbackId_);
  }

  gboolean AnimatedLayout::tick_(GtkWidget* widget, GdkFrameClock* frame_clock, gpointer user_data) {
    return ((AnimatedLayout*)user_data)->tick(widget, frame_clock);
  }

  gboolean AnimatedLayout::tick(GtkWidget* widget, GdkFrameClock* frame_clock) {
    pthread_mutex_lock(&tickMutex_);
    
    std::map<Animation*, Animation*> copy = std::map<Animation*, Animation*>(animations_);

    pthread_mutex_unlock(&tickMutex_);

    std::vector<Animation*> completed;
    for (std::map<Animation*, Animation*>::iterator it = copy.begin(); it != copy.end(); it++) {
      Animation* animation = it->second;

      gint64 time = gdk_frame_clock_get_frame_time(frame_clock) - animation->startTime;
      gint64 duration = animation->duration * 1000;
      if (time >= duration) {
	time = duration;
	completed.push_back(animation);
      }
      
      double x = EasingFunc::interpolateQuintInOut(time, animation->startX, animation->endX, duration);
      double y = EasingFunc::interpolateQuintInOut(time, animation->startY, animation->endY, duration);

      update(animation->widget, x, y);
    }

    queue_resize();

    pthread_mutex_lock(&tickMutex_);

    for (std::vector<Animation*>::iterator it = completed.begin(); it != completed.end(); it++) {
      animations_.erase(*it);
    }

    if (animations_.empty()) {
      removeCallback();
    }

    pthread_mutex_unlock(&tickMutex_);

    return G_SOURCE_CONTINUE;
  }

  void AnimatedLayout::tick_destroy(gpointer data) {
  }
}
