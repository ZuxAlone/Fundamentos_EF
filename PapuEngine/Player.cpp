#include "Player.h"
#include <SDL\SDL.h>


void Player::init(float speed, glm::vec2 position, std::string texturePath, Camera2D* camera) {
	_speed = speed;
	_position = position;
	_texturePath = texturePath;
	color.set(255, 255, 255, 255);
	_camera = camera;
}
void Player::update() {
}


Player::Player():_currentGun(-1)
{
}


Player::~Player()
{
}
