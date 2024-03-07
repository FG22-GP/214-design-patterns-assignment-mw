#include "stateStack.h"

#include "dataStructuresAndMethods.h"
#include "enemyManager.h"
#include "gameEngine.h"
#include "obstacleManager.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "timerManager.h"


Button::Button(const char* spritePath, int height, int width, Vector2<float> position) {
	_height = height;
	_width = width;

	_position = position;
	
	_sprite = std::make_shared<Sprite>();
	_sprite->h = height;
	_sprite->w = width;
	_sprite->Load(spritePath);

	_boxCollider = AABB::makeFromPositionSize(position, height, width);
	
}

bool Button::ClickedOn() {
	Vector2<float> cursorPosition = GetCursorPosition();
	Circle circle;
	circle.position = cursorPosition;
	circle.radius = 10.f;
	if (GetMouseButtonPressed(SDL_BUTTON_LEFT)) {
		if (AABBCircleIntersect(_boxCollider, circle)) {
			return true;
		}
	}
	return false;
}
void Button::Render() {
	_sprite->RenderCentered(_position);
}

void Button::SetTargetPosition(Vector2<float> position) {
	_position = position;
	_boxCollider = AABB::makeFromPositionSize(position, _sprite->h, _sprite->w);
}
GameStateHandler::GameStateHandler() {
	_buttons[ButtonType::MainMenu] = std::make_shared<Button>(_mainMenuButtonSprite, 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.6f));
	_buttons[ButtonType::Play] = std::make_shared<Button>(_playButtonSprite, 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.3f));
	_buttons[ButtonType::Quit] = std::make_shared<Button>(_quitButtonSprite, 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.8f));
	_buttons[ButtonType::Restart] = std::make_shared<Button>(_restartButtonSprite, 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.4f));
	_buttons[ButtonType::Resume] = std::make_shared<Button>(_resumeButtonSprite, 64, 128, Vector2<float>(windowWidth * 0.5f, windowHeight * 0.2f));
}

GameStateHandler::~GameStateHandler() {}

//Add a new state which will be the state that is called in update
void GameStateHandler::AddState(std::shared_ptr<State> state) {
	_states.emplace_back(state);
}
//Removes all the states except the first one which is MainMenu state
void GameStateHandler::BackToFirstState() {
	while (_states.size() > 1) {
		_states.pop_back();
	}
	_states.back()->SetButtonPositions();
}
//Replaces removes the current state and add a new state
void GameStateHandler::ReplaceCurrentState(std::shared_ptr<State> state) {
	_states.pop_back();
	_states.emplace_back(state);
}
/*Removes the current state and starts updating the former state
and sets the positions of the necessary buttons for said state*/
void GameStateHandler::RemoveCurrentState() {
	_states.pop_back();
	_states.back()->SetButtonPositions();
}
//Updates the state at the back of the vector
void GameStateHandler::UpdateState() {
	_states.back()->Update();
}

void GameStateHandler::RenderState() {
	_states.back()->Render();
}

void GameStateHandler::RenderStateText() {
	_states.back()->RenderText();
}

GameState::GameState() {
	playerCharacter->Respawn();
}

void GameState::SetButtonPositions() {}

void GameState::Update() {
	//enemyManager->UpdateQuadTree();
	//projectileManager->UpdateQuadTree();

	//enemyManager->Update(); 
	obstacleManager->UpdateObstacles();
	//projectileManager->Update();
	playerCharacter->Update();
	timerManager->Update();
}

void GameState::Render() {
	enemyManager->Render();
	obstacleManager->RenderObstacles();
	playerCharacter->Render();
	projectileManager->Render();
}

void GameState::RenderText() {
	playerCharacter->RenderText();
}

GameOverState::GameOverState() {
	SetButtonPositions();
}

void GameOverState::SetButtonPositions() {
	_buttons[ButtonType::Restart]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.2f));
	_buttons[ButtonType::MainMenu]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.5f));
	_buttons[ButtonType::Quit]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.8f));
}

void GameOverState::Update() {
	if (_buttons[ButtonType::MainMenu]->ClickedOn()) {
		gameStateHandler->BackToFirstState();

	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;

	} else if (_buttons[ButtonType::Restart]->ClickedOn()) {
		gameStateHandler->ReplaceCurrentState(std::make_shared<GameState>());
	}
}

void GameOverState::Render() {
	_buttons[ButtonType::MainMenu]->Render();
	_buttons[ButtonType::Quit]->Render();
	_buttons[ButtonType::Restart]->Render();
}

void GameOverState::RenderText() {}

MenuState::MenuState() {
	SetButtonPositions();
}

void MenuState::SetButtonPositions() {
	_buttons[ButtonType::Play]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.3f));
	_buttons[ButtonType::Quit]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.7f));
}

void MenuState::Update() {
	if (_buttons[ButtonType::Play]->ClickedOn()) {
		gameStateHandler->AddState(std::make_shared<GameState>());

	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;
	}
}

void MenuState::Render() {
	_buttons[ButtonType::Play]->Render();
	_buttons[ButtonType::Quit]->Render();
}

void MenuState::RenderText() {}

PauseState::PauseState() {
	SetButtonPositions();
}

void PauseState::SetButtonPositions() {
	_buttons[ButtonType::Resume]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.2f));
	_buttons[ButtonType::Restart]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.4f));
	_buttons[ButtonType::MainMenu]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.6f));
	_buttons[ButtonType::Quit]->SetTargetPosition(Vector2<float>(windowWidth * 0.5f, windowHeight * 0.8f));
}

void PauseState::Update() {
	if (_buttons[ButtonType::MainMenu]->ClickedOn()) {
		gameStateHandler->BackToFirstState();
	
	} else if (_buttons[ButtonType::Quit]->ClickedOn()) {
		runningGame = false;
	
	} else if (_buttons[ButtonType::Restart]->ClickedOn()) {
		gameStateHandler->BackToFirstState();
		gameStateHandler->AddState(std::make_shared<GameState>());

	} else if (_buttons[ButtonType::Resume]->ClickedOn() || GetKeyPressed(SDL_SCANCODE_ESCAPE)) {
		gameStateHandler->RemoveCurrentState();
	}
	
}

void PauseState::Render() {
	enemyManager->Render();
	playerCharacter->Render();
	projectileManager->Render();

	_buttons[ButtonType::MainMenu]->Render();
	_buttons[ButtonType::Restart]->Render();
	_buttons[ButtonType::Resume]->Render();
	_buttons[ButtonType::Quit]->Render();
}

void PauseState::RenderText() {}

