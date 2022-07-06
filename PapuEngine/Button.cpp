#include "Button.h"
#include "ResourceManager.h"
#include <iostream>

Button::Button(std::string _path):path(_path)
{
	_textureID = ResourceManager::getTexture(path).id;
}

Button::~Button()
{
}

void Button::draw(SpriteBacth& spriteBatch)
{
	Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	color.a = 255;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(0,0,100,100);
	spriteBatch.draw(destRect, uvRect, _textureID, 0.0f, color);
}

bool Button::click(glm::vec2 position)
{
	if (position.x >= 0 && position.x <= 100) {
		std::cout << "Las x son iguales" << std::endl;
		if (position.y >= 0 && position.y <= 100) {
			std::cout << "Las y son iguales" << std::endl;
			return true;
		}
	}
	return false;
}
