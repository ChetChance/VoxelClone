#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <string>
#include <vector>

#include "stb_image.h"

class textureHandler {
public:
	static unsigned int appendTexture(std::string texturePath);
};

#endif // TEXTURE_HANDLER_H