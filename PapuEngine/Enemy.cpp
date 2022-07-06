#include "Enemy.h"

Enemy::Enemy() {

}

Enemy::~Enemy() {

}

void Enemy::init(float speed, glm::vec2 position, std::string texturePath, EnemyType type) {
	_speed = speed;
	_position = position;
	_texturePath = texturePath;
	_type = type;
	color.set(255, 255, 255, 255);
}

void Enemy::update() {
	glm::vec2 direction = glm::vec2(0.0f, -1.0f);
	_position += direction * _speed;
}

void Enemy::losePoints(int& puntaje, unsigned int key) {
	switch (_type) {
	case EnemyType::Amarillo:
		if (key == SDLK_w) puntaje -= 15;
		else if (key == SDLK_e) puntaje /= 2;
		break;
	case EnemyType::Rojo:
		if (key == SDLK_q) puntaje -= 10;
		else if (key == SDLK_e) puntaje -= 5;
		break;
	case EnemyType::Verde:
		if (key == SDLK_q) puntaje -= 20;
		else if (key == SDLK_w) puntaje -= 15;
		break;
	}
}

void Enemy::winPoints(int& puntaje) {
	switch (_type) {
	case EnemyType::Amarillo:
		puntaje += 10;
		break;
	case EnemyType::Rojo:
		puntaje += 20;
		break;
	case EnemyType::Verde:
		puntaje *= 2;
		break;
	}
}