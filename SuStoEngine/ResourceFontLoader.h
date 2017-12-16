#ifndef __RESOURCE_FONT_LOADER_H__
#define __RESOURCE_FONT_LOADER_H__

class ResourceFont;
class ResourceFontLoader
{
public:
	ResourceFontLoader();
	~ResourceFontLoader();

	bool ImportFont(const char* path, const char* name);
	void LoadFont(ResourceFont* font);
};

#endif 