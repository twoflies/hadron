// Font Cache

#include "font.h"

#include <stdio.h>
#include <fontconfig/fontconfig.h>

namespace HA {

  const char* FONT = "Mylodon-Light.otf";
  const char* DESCRIPTION_FORMAT = "Mylodon Bold %2d";

  std::map<int, Pango::FontDescription> FontCache::cache_;

  int FontCache::initialize(const std::string& path) {
    std::string font = path + "/" + FONT; 
    FcBool result = FcConfigAppFontAddFile(FcConfigGetCurrent(), (const FcChar8*)font.c_str());
    return result ? 0 : -1;
  }

  const Pango::FontDescription& FontCache::getFontDescription(int size) {
    std::map<int, Pango::FontDescription>::iterator it = cache_.find(size);
    if (it == cache_.end()) {
      static char buffer[25];
      snprintf(buffer, sizeof(buffer), DESCRIPTION_FORMAT, size);
      Pango::FontDescription fontDescription(buffer);
      cache_[size] = fontDescription;
    }
    
    return cache_[size];
  }
}
