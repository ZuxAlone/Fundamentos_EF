#pragma once
#include "Agent.h"
#include "InputManager.h"
#include <vector>
#include "Camera2D.h"

class Player: public Agent
{
private:
	InputManager* _inputManager;
	int _currentGun;
	Camera2D* _camera;
public:
	Player();
	~Player();
	void init(float speed, glm::vec2 position, std::string texturePath, Camera2D* camera);
	void update();
};

