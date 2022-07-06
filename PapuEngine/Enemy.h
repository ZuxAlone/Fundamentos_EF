#pragma 
#include <SDL\SDL.h>
#include "Agent.h"

enum class EnemyType { Amarillo, Rojo, Verde };

class Enemy : public Agent
{
private:
	EnemyType _type;

public:
	Enemy();
	~Enemy();

	inline EnemyType getType() const { return _type; }

	void init(float speed, glm::vec2 position, std::string texturePath, EnemyType type);
	void update();

	void losePoints(int& puntaje, unsigned int key);
	void winPoints(int& puntaje);
};

