#pragma once
#include <glm\glm.hpp>
#include <string>
#include "SpriteBacth.h"
#include "GLTexture.h"

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Agent
{
protected:
	glm::vec2 _position;
	float _speed;
	std::string _texturePath;
	Color color;

public:
	Agent();
	glm::vec2 getPosition()const { return _position; };
	virtual void update() = 0;
	void draw(SpriteBacth& spritebatch);
	virtual ~Agent();
	bool collideWithAgent(Agent* agent);
};

