#include "Background.h"
#include "ResourceManager.h"

Background::Background(std::string _path) : path(_path)
{
	_textureID = ResourceManager::getTexture(path).id;
}

Background::~Background()
{
}

void Background::draw(SpriteBacth& spriteBatch)
{
	Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(-400, -250, 800, 500);
	spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, color);
}
