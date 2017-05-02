// Font Cache

#ifndef _FONT_H_
#define _FONT_H_

#include <map>
#include <pangomm/fontdescription.h>

namespace HA {

  class FontCache {
  public:
    static int initialize(const std::string& path);
    static const Pango::FontDescription& getFontDescription(int size);

  private:
    static std::map<int, Pango::FontDescription> cache_;
  };
}

#endif // _FONT_H_
