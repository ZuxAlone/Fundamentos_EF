#pragma once

#include <SDL\SDL.h>
#include <ctime>
#include <random>
#include "IGameScreen.h"
#include "Camera2D.h"
#include "GLS_Program.h"
#include "Window.h"
#include "GLTexture.h"
#include "SpriteBacth.h"
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "SpriteFont.h"

enum class GameState { Playing, GameOver };

class GamePlayScreen : public IGameScreen
{
private:
	GLS_Program _program;
	Camera2D _camera;
	Window* _window = nullptr;
	SpriteBacth _spriteBatch;
	InputManager _inputManager;
	Background* background;
	Background* gameOverBackground;
	Player* _player;
	std::vector<Enemy*> _enemies;
	SpriteFont* _spriteFont;
	GameState _gameState;

	std::mt19937 randomEngine;
	std::uniform_int_distribution<int> enemyPicker;

	time_t actTime;
	time_t spawnTime;
	int _puntaje;
	int _timer;

	void spawnEnemies();
	void drawAgents();
	void updateAgents();
	void updateTime();
	void drawUI();
	void drawGameOverUI();
	bool checkKey(Enemy* enemy);
public:
	GamePlayScreen(Window* window);
	~GamePlayScreen();

	virtual void build() override;
	virtual void destroy() override;
	virtual void onExit() override;
	virtual void onEntry() override;
	virtual void draw()override;
	virtual void update()override;
	virtual int getNextScreen() const override;
	virtual int getPreviousScreen() const override;

	void checkInput();
};

