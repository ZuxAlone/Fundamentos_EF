#include "GamePlayScreen.h"
#include "Game.h"

#include "ImageLoader.h"
#include <iostream>
#include "ResourceManager.h"
#include "PapuEngine.h"
#include "ScreenIndices.h"


GamePlayScreen::GamePlayScreen(Window* window) :
	_window(window)
{
	_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GamePlayScreen::~GamePlayScreen()
{
}


void GamePlayScreen::build() {
	_player = new Player();
	glm::vec2 playerPos = glm::vec2(-30, -_window->getScreenHeight() / 2);
	_player->init(1.0f, playerPos, "Textures/player.png", &_camera);
	_spriteBatch.init();
	background = new Background("Textures/Fondos/game.png");
	gameOverBackground = new Background("Textures/Fondos/gam-over.png");
	_puntaje = 0;
	_timer = 60;
	randomEngine = std::mt19937(time(nullptr));
	enemyPicker = std::uniform_int_distribution<int>(1, 3);
}
void GamePlayScreen::destroy() {

}
void GamePlayScreen::onExit() {
}
void GamePlayScreen::onEntry() {
	_program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());
	_spriteFont = new SpriteFont("Fonts/OrbitronFont.ttf", 40);
	_gameState = GameState::Playing;
	time(&actTime);
	time(&spawnTime);
}
void GamePlayScreen::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_program.use();

	glActiveTexture(GL_TEXTURE0);

	GLuint pLocation = _program.getUniformLocation("P");

	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	GLuint imageLocation = _program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);

	_spriteBatch.begin();

	// Renderizar el fondo, agentes y UI cuando el estado del juego sea "Jugando"
	if (_gameState == GameState::Playing) {
		drawAgents();
		drawUI();
		background->draw(_spriteBatch);
	}
	// Renderizar el fondo y UI de game over cuando haya terminado el juego
	else {
		drawGameOverUI();
		gameOverBackground->draw(_spriteBatch);
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	_window->swapBuffer();
}

void GamePlayScreen::drawUI() {
	glm::vec2 pos;
	Color color;
	color.set(255, 255, 255, 255);
	char buffer[256];

	pos = glm::vec2(-350, _window->getScreenHeight() / 2 - 50);
	sprintf_s(buffer, "Tiempo: %d", _timer);
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.7f, 0.7f), 0.0f, color);

	pos = glm::vec2(350, _window->getScreenHeight() / 2 - 50);
	sprintf_s(buffer, "Puntaje: %d", _puntaje);
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.7f, 0.7f), 0.0f, color, Justification::RIGHT);

	pos = glm::vec2(-350, -120);
	sprintf_s(buffer, "Instrucciones:");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.4f, 0.4f), 0.0f, color);

	pos = glm::vec2(-350, -140);
	sprintf_s(buffer, "Cuando el alien toque al jugador");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.3f, 0.3f), 0.0f, color);

	pos = glm::vec2(-350, -160);
	sprintf_s(buffer, "Presionar la tecla");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.4f, 0.3f), 0.0f, color);

	pos = glm::vec2(-350, -180);
	sprintf_s(buffer, "Alien amarillo -> Q");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.4f, 0.3f), 0.0f, color);

	pos = glm::vec2(-350, -200);
	sprintf_s(buffer, "Alien rojo -> W");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.4f, 0.3f), 0.0f, color);

	pos = glm::vec2(-350, -220);
	sprintf_s(buffer, "Alien verde -> E");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.4f, 0.3f), 0.0f, color);

	pos = glm::vec2(-350, -240);
	sprintf_s(buffer, "La tecla incorrecta costara puntos");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(0.3f, 0.3f), 0.0f, color);
}

void GamePlayScreen::drawGameOverUI() {
	glm::vec2 pos;
	Color color;
	color.set(255, 255, 255, 255);
	char buffer[256];

	pos = glm::vec2(0, 180);
	sprintf_s(buffer, "Puntaje obtenido");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(1), 0.0f, color, Justification::MIDDLE);

	pos = glm::vec2(0, 130);
	sprintf_s(buffer, "%d", _puntaje);
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(1), 0.0f, color, Justification::MIDDLE);

	pos = glm::vec2(0, -250);
	sprintf_s(buffer, "GAME OVER");
	_spriteFont->draw(_spriteBatch, buffer, pos, glm::vec2(1.5), 0.0f, color, Justification::MIDDLE);
}

void GamePlayScreen::update() {
	checkInput();
	draw();
	_camera.update();
	_inputManager.update();

	// Actualizar los agentes y el tiempo en estado "Jugando"
	if (_gameState == GameState::Playing) {
		spawnEnemies();
		updateAgents();
		updateTime();
	}
}

void GamePlayScreen::spawnEnemies() {
	time_t auxTime = time(nullptr);

	// Realizar el spawn de los enemigos cada 2 segundos
	if (auxTime - spawnTime == 2) {
		spawnTime = auxTime;
		glm::vec2 enemyPos = glm::vec2(-30, _window->getScreenHeight() / 2 - 50);
		Enemy* newEnem;
		switch (enemyPicker(randomEngine)) {
		case 1:
			newEnem = new Enemy();
			newEnem->init(0.3f, enemyPos, "Textures/amarillo.png", EnemyType::Amarillo);
			_enemies.push_back(newEnem);
			break;
		case 2:
			newEnem = new Enemy();
			newEnem->init(0.3f, enemyPos, "Textures/rojo.png", EnemyType::Rojo);
			_enemies.push_back(newEnem);
			break;
		case 3:
			newEnem = new Enemy();
			newEnem->init(0.3f, enemyPos, "Textures/verde.png", EnemyType::Verde);
			_enemies.push_back(newEnem);
			break;
		}
	}
}

void GamePlayScreen::drawAgents() {
	// Dibujar al jugador y a los enemigos
	_player->draw(_spriteBatch);

	for (size_t i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->draw(_spriteBatch);
	}
}

void GamePlayScreen::updateAgents() {
	for (size_t i = 0; i < _enemies.size(); i++)
	{
		_enemies[i]->update();
		// Revisión de la colisión de los enemigos con el player, se revisa la tecla presionada
		if ((_enemies[i]->collideWithAgent(_player) && checkKey(_enemies[i])) ||
			_enemies[i]->getPosition().y < -_window->getScreenHeight() / 2 - 100) {
			// Si el enemigo sale de pantalla y se escapa también resta puntos
			if (_enemies[i]->getPosition().y < -_window->getScreenHeight() / 2 - 100) _puntaje -= 100;
			// Se eliminarán los enemigos del arreglo para mantener el programa limpio
			delete _enemies[i];
			_enemies[i] = _enemies.back();
			_enemies.pop_back();
		}
	}
}

void GamePlayScreen::updateTime() {
	// Actualización del tiempo
	time_t auxTime = time(nullptr);
	if (auxTime - actTime == 1) {
		actTime = auxTime;
		_timer--;
	}
	// Si el timer llega a 0 o el puntaje es menor a 0 termina el juego
	if (_timer == 0 || _puntaje < 0) _gameState = GameState::GameOver;
}

bool GamePlayScreen::checkKey(Enemy* enemy) {
	// Dependiendo de la tecla presiona y el tipo de enemigo se sumará o restará puntos
	if (_inputManager.isKeyDown(SDLK_q)) {
		if (enemy->getType() != EnemyType::Amarillo) enemy->losePoints(_puntaje, SDLK_q);
		else enemy->winPoints(_puntaje);
		return true;
	}
	if (_inputManager.isKeyDown(SDLK_w)) {
		if (enemy->getType() != EnemyType::Rojo) enemy->losePoints(_puntaje, SDLK_w);
		else enemy->winPoints(_puntaje);
		return true;
	}
	if (_inputManager.isKeyDown(SDLK_e)) {
		if (enemy->getType() != EnemyType::Verde) enemy->losePoints(_puntaje, SDLK_e);
		else enemy->winPoints(_puntaje);
		return true;
	}
	return false;
}

void GamePlayScreen::checkInput() {
	SDL_Event event;
	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_currentState = ScreenState::EXIT_APPLICATION;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case  SDL_KEYUP:
			_inputManager.releaseKey(event.key.keysym.sym);
			break;
		case  SDL_KEYDOWN:
			_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		}
		if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
			_currentState = ScreenState::EXIT_APPLICATION;
		}
		// En la vista de Game Over, al dar click se mostrará un mensaje en consola
		if (_inputManager.isKeyDown(SDL_BUTTON_LEFT) && _gameState == GameState::GameOver) {
			std::cout << "Para el otro anio sera :D" << std::endl;
		}
	}
}

int GamePlayScreen::getNextScreen() const {
	return -1;
};

int GamePlayScreen::getPreviousScreen() const {
	return -1;
}
