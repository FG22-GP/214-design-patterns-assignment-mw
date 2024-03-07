#pragma once
#include "collision.h"
#include "sprite.h"

#include <array>
#include <memory>
#include <stack>
#include <unordered_map>
#include "vector"

enum class ButtonType {
	MainMenu,
	Play,
	Quit,
	Restart,
	Resume,
	Count
};

class Button {
public:
	Button(const char* spritePath, int height, int width, Vector2<float> position);
	~Button() {}

	bool ClickedOn();
	void Render();
	void SetTargetPosition(Vector2<float> position);

private:
	int _height;
	int _width;
	
	AABB _boxCollider;
	std::shared_ptr<Sprite> _sprite;
	Vector2<float> _position;
};

class State {
public:
	State() {}
	~State() {}

	virtual void SetButtonPositions() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderText() = 0;
};

class GameStateHandler {
public:
	GameStateHandler();
	~GameStateHandler();

	void AddState(std::shared_ptr<State> state);
	void BackToFirstState();
	void ReplaceCurrentState(std::shared_ptr<State> state);
	void RemoveCurrentState();

	void UpdateState();
	void RenderState();
	void RenderStateText();

private:
	const char* _mainMenuButtonSprite = "res/sprites/MainMenuButton.png";
	const char* _playButtonSprite = "res/sprites/PlayButton.png";
	const char* _quitButtonSprite = "res/sprites/QuitButton.png";
	const char* _restartButtonSprite = "res/sprites/RestartButton.png";
	const char* _resumeButtonSprite = "res/sprites/ResumeButton.png";

	std::vector<std::shared_ptr<State>> _states;

};

class GameOverState : public State {
public:
	GameOverState();
	~GameOverState() {}

	void SetButtonPositions() override;
	void Update() override;
	void Render() override;
	void RenderText() override;
};
class GameState : public State {
public:
	GameState();
	~GameState() {}

	void SetButtonPositions() override;
	void Update() override;
	void Render() override;
	void RenderText() override;

};
class MenuState : public State {
public:
	MenuState();
	~MenuState() {}

	void SetButtonPositions() override;
	void Update() override;
	void Render() override;
	void RenderText() override;
};
class PauseState : public State {
public:
	PauseState();
	~PauseState() {}

	void SetButtonPositions() override;
	void Update() override;
	void Render() override;
	void RenderText() override;
};
