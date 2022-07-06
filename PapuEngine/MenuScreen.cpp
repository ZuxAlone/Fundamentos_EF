#include "MenuScreen.h"
#include "ScreenIndices.h"
#include <iostream>


MenuScreen::MenuScreen(Window* window):_window(window)
{
	_screenIndex = SCREEN_INDEX_MENU;
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::build()
{
	background = new Background("Textures/Fondos/menu.png");
}

void MenuScreen::destroy()
{
	background = nullptr;
}

void MenuScreen::onExit()
{
	destroy();
}

void MenuScreen::onEntry()
{
	_program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	_program.addAtribute("vertexPosition");
	_program.addAtribute("vertexColor");
	_program.addAtribute("vertexUV");
	_program.linkShader();
	_spriteBatch.init();
	_camera.init(_window->getScreenWidth(), _window->getScreenHeight());
	spriteFont = new SpriteFont("Fonts/OrbitronFont.ttf", 40);
}

void MenuScreen::draw()
{
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

	// Renderizar el fondo y UI necesarios
	background->draw(_spriteBatch);
	drawFonts();
	
	_spriteBatch.end();
	_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	_program.unuse();
	_window->swapBuffer();
}

void MenuScreen::drawFonts() {
	Color color;
	color.set(255, 255, 255, 255); 
	char buffer[256];
	sprintf_s(buffer, "Examen Final Bica Corazon");
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-200, 150), glm::vec2(0.7f, 0.7f), 0.0f, color);

	sprintf_s(buffer, "Richard Jose Maguina Bernuy (U202021097)");
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-350, 80), glm::vec2(0.7f, 0.7f), 0.0f, color);

	sprintf_s(buffer, "Haz click para continuar...");
	spriteFont->draw(_spriteBatch, buffer, glm::vec2(-200, -200), glm::vec2(0.7f, 0.7f), 0.0f, color);
}

void MenuScreen::update()
{
	draw();
	_camera.update();
	_inputManager.update();
	checkInput();
}

void MenuScreen::checkInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			_currentState = ScreenState::EXIT_APPLICATION;
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(event.button.button);
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(event.motion.x,event.motion.y);
			break;
		}

		if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
			_currentState = ScreenState::EXIT_APPLICATION;
		}
		if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			_currentState = ScreenState::CHANGE_NEXT;
		}
	}
}

int MenuScreen::getNextScreen() const
{
	return SCREEN_INDEX_GAMEPLAY;
}

int MenuScreen::getPreviousScreen() const
{
	return SCREEN_INDEX_NO_INDEX;
}
