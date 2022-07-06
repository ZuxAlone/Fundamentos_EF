#include "Agent.h"
#include "ResourceManager.h"
#include <algorithm>

Agent::Agent()
{

}

void Agent::draw(SpriteBacth& spritebatch) {
	int textureID = ResourceManager::getTexture(_texturePath).id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(_position.x, _position.y, AGENT_WIDTH, AGENT_WIDTH);
	spritebatch.draw(destRect, uvRect, textureID, 0.0f, color);
}

bool Agent::collideWithAgent(Agent* agent) {
	glm::vec2 centerPosA = _position + glm::vec2(AGENT_WIDTH / 2);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_WIDTH / 2);
	glm::vec2 dist = centerPosA - centerPosB;
	const float MIN_DISTANCE = AGENT_RADIUS * 2;
	float distance = glm::length(dist);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) return true;
	return false;
}

Agent::~Agent()
{
}