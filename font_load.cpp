#include <boost/filesystem.hpp>
#include <FTGL/ftgl.h>

FTFont *font_load(std::string filename) {
  /// Try to load a font from the specified filename, running all checks first
  if(!(boost::filesystem::exists(filename) &&
       boost::filesystem::is_regular_file(filename))) {
    std::cout << "ERROR: font_load: no such file as " << filename << std::endl;
    return nullptr;
  }
  //FTFont *font = new FTTextureFont(filename.c_str());
  //FTFont *font = new FTBufferFont(filename.c_str());
  FTFont *font = new FTPolygonFont(filename.c_str());
  //FTFont *font = new FTExtrudeFont(filename.c_str());
  if(!font) {
    std::cout << "ERROR: font_load: could not load font from file " << filename << std::endl;
    return nullptr;
  }
  font->CharMap(ft_encoding_unicode);
  font->FaceSize(16, 72);  // points, display resolution: 10, 72 = 10PPEm, ignored for anything other than a buffer font
  font->UseDisplayList(true);
  font->Depth(16.0);

  return font;
}
