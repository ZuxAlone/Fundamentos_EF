#pragma once
#include <glm/glm.hpp>
#include <string>
#include "SpriteBacth.h"
#include "GLTexture.h"

class Background
{
private:
	std::string path;
	int _textureID;
public:

	Background(std::string _path);
	~Background();
	void draw(SpriteBacth& spriteBatch);
};

