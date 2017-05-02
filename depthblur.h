// Depth Blur

#ifndef _DEPTHBLUR_H_
#define _DEPTHBLUR_H_

#include "container.h"

#include <string>
#include <map>

namespace HA {

  const int DEPTH_BLUR_SHALLOW = 3;
  const int DEPTH_BLUR_DEEP = 5;
    
  class DepthBlur : public Container {
  public:
    DepthBlur();
    virtual ~DepthBlur();
    int getDepth() const;
    void setDepth(int depth);
    void cacheBlur(const std::string& key);

  protected:
    bool getBlur(const std::string& key, Cairo::RefPtr<Cairo::ImageSurface>& blur);
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& context);

  private:
    void blur_image_surface(Cairo::RefPtr<Cairo::ImageSurface>& sourceSurface, int radius);

  private:
    int depth_;
    std::map<std::string, Cairo::RefPtr<Cairo::ImageSurface>> blurCache_;
  };
  
}

#endif // _DEPTHBLUR_H_
