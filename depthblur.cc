// Depth Blur

#include "depthblur.h"

#include "gtk/gtk.h"

namespace HA {

  const std::string DEFAULT_BLUR_KEY = "_default";

  DepthBlur::DepthBlur() {
  }

  DepthBlur::~DepthBlur() {
  }

  int DepthBlur::getDepth() const {
    return depth_;
  }

  void DepthBlur::setDepth(int depth) {
    depth_ = depth;
    queue_draw();
  }

  void DepthBlur::cacheBlur(const std::string& key) {
    Gtk::Allocation allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();
    
    Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::Format::FORMAT_ARGB32, width, height);
    Cairo::RefPtr<Cairo::Context> surfaceContext = Cairo::Context::create(surface);
    
    Container::on_draw(surfaceContext);
    
    blur_image_surface(surface, depth_);
    
    blurCache_[key] = surface;    
  }

  bool DepthBlur::getBlur(const std::string& key, Cairo::RefPtr<Cairo::ImageSurface>& blur) {
    std::map<std::string, Cairo::RefPtr<Cairo::ImageSurface>>::iterator it = blurCache_.find(key);
    if (it == blurCache_.end()) {
      return false;
    }

    blur = it->second;
    return true;
  }

  bool DepthBlur::on_draw(const Cairo::RefPtr<Cairo::Context>& context) {
    if (depth_ > 0) {
      Cairo::RefPtr<Cairo::ImageSurface> surface;
      if (!getBlur(DEFAULT_BLUR_KEY, surface)) {
	cacheBlur(DEFAULT_BLUR_KEY);
	if (!getBlur(DEFAULT_BLUR_KEY, surface)) {
	  return false;
	}
      }

      context->set_source(surface, depth_ - 1, depth_  - 1);
      context->paint();
    }
    else {
      Container::on_draw(context);
    }

    return false;  // propagate
  }

  #define ARRAY_LENGTH(a) (sizeof (a) / sizeof (a)[0])

  /* Gaussian blur from https://www.cairographics.org/cookbook/blur.c/ */
  void DepthBlur::blur_image_surface(Cairo::RefPtr<Cairo::ImageSurface>& surface, int radius) {
    // Steve Hanov, 2009
    // Released into the public domain.
    
    // get width, height
    int width = surface->get_width();
    int height = surface->get_height();
    unsigned char* dst = (unsigned char*)malloc(width*height*4);
    unsigned* precalc = (unsigned*)malloc(width*height*sizeof(unsigned));
    unsigned char* src = surface->get_data();
    double mul=1.f/((radius*2)*(radius*2));
    int channel;
    
    // The number of times to perform the averaging. According to wikipedia,
    // three iterations is good enough to pass for a gaussian.
    const int MAX_ITERATIONS = 3; 
    int iteration;

    memcpy( dst, src, width*height*4 );

    for ( iteration = 0; iteration < MAX_ITERATIONS; iteration++ ) {
      for( channel = 0; channel < 4; channel++ ) {
	int x,y;
	
	// precomputation step.
	unsigned char* pix = src;
	unsigned* pre = precalc;
	
	pix += channel;
	for (y=0;y<height;y++) {
	  for (x=0;x<width;x++) {
	    int tot=pix[0];
	    if (x>0) tot+=pre[-1];
	    if (y>0) tot+=pre[-width];
	    if (x>0 && y>0) tot-=pre[-width-1];
	    *pre++=tot;
	    pix += 4;
	  }
	}
	
	// blur step.
	pix = dst + (int)radius * width * 4 + (int)radius * 4 + channel;
	for (y=radius;y<height-radius;y++) {
	  for (x=radius;x<width-radius;x++) {
	    int l = x < radius ? 0 : x - radius;
	    int t = y < radius ? 0 : y - radius;
	    int r = x + radius >= width ? width - 1 : x + radius;
	    int b = y + radius >= height ? height - 1 : y + radius;
	    int tot = precalc[r+b*width] + precalc[l+t*width] - 
	      precalc[l+b*width] - precalc[r+t*width];
	    *pix=(unsigned char)(tot*mul);
	    pix += 4;
	  }
	  pix += (int)radius * 2 * 4;
	}
      }
      memcpy( src, dst, width*height*4 );
    }
    
    free( dst );
    free( precalc );
    
    /*Cairo::RefPtr<Cairo::ImageSurface> tmp;
    int width, height;
    int src_stride, dst_stride;
    int x, y, z, w;
    uint8_t *src, *dst;
    uint32_t *s, *d, a, p;
    int i, j, k;
    uint8_t kernel[17];
    const int size = ARRAY_LENGTH(kernel);
    const int half = size / 2;
     
    width = surface->get_width();
    height = surface->get_height();
 
    switch (surface->get_format()) {
    case Cairo::Format::FORMAT_A1:
    default:
      // Don't even think about it!
      return;
      
    case Cairo::Format::FORMAT_A8:
      // Handle a8 surfaces by effectively unrolling the loops by a
      // factor of 4 - this is safe since we know that stride has to be a
      // multiple of uint32_t.
      width /= 4;
      break;
      
    case Cairo::Format::FORMAT_RGB24:
    case Cairo::Format::FORMAT_ARGB32:
      break;
    }
  
    src = surface->get_data();
    src_stride = surface->get_stride();

    tmp = Cairo::ImageSurface::create(Cairo::Format::FORMAT_ARGB32, width, height);
    dst = tmp->get_data();
    dst_stride = tmp->get_stride();
    
    a = 0;
    for (i = 0; i < size; i++) {
      double f = i - half;
      a += kernel[i] = exp (- f * f / 30.0) * 80;
    }
    
    // Horizontally blur from surface -> tmp
    for (i = 0; i < height; i++) {
      s = (uint32_t *) (src + i * src_stride);
      d = (uint32_t *) (dst + i * dst_stride);
      for (j = 0; j < width; j++) {
        if (radius < j && j < width - radius) {
	  d[j] = s[j];
	  continue;
        }
	
        x = y = z = w = 0;
        for (k = 0; k < size; k++) {
	  if (j - half + k < 0 || j - half + k >= width)
            continue;
	  
	  p = s[j - half + k];
	  
	  x += ((p >> 24) & 0xff) * kernel[k];
	  y += ((p >> 16) & 0xff) * kernel[k];
	  z += ((p >>  8) & 0xff) * kernel[k];
	  w += ((p >>  0) & 0xff) * kernel[k];
        }
        d[j] = (x / a << 24) | (y / a << 16) | (z / a << 8) | w / a;
      }
    }
    
    // Then vertically blur from tmp -> surface
    for (i = 0; i < height; i++) {
      s = (uint32_t *) (dst + i * dst_stride);
      d = (uint32_t *) (src + i * src_stride);
      for (j = 0; j < width; j++) {
        if (radius <= i && i < height - radius) {
	  d[j] = s[j];
	  continue;
        }
	
        x = y = z = w = 0;
        for (k = 0; k < size; k++) {
	  if (i - half + k < 0 || i - half + k >= height)
            continue;
	  
	  s = (uint32_t *) (dst + (i - half + k) * dst_stride);
	  p = s[j];
	  
	  x += ((p >> 24) & 0xff) * kernel[k];
	  y += ((p >> 16) & 0xff) * kernel[k];
	  z += ((p >>  8) & 0xff) * kernel[k];
	  w += ((p >>  0) & 0xff) * kernel[k];
        }
        d[j] = (x / a << 24) | (y / a << 16) | (z / a << 8) | w / a;
      }
    }
   
    tmp->finish();
    surface->mark_dirty();*/
  }
}
