#include "playerCharacter.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "enemyManager.h"
#include "gameEngine.h"
#include "obstacleManager.h"
#include "obstacleWall.h"
#include "projectile.h"
#include "projectileManager.h"
#include "stateStack.h"
#include "timerManager.h"
#include "quadTree.h"

#include <string>

PlayerCharacter::PlayerCharacter(float characterOrientation, Vector2<float> characterPosition) {
	_characterSprite = std::make_shared<Sprite>();
	_characterSprite->Load(_kingSprite);

	_orientation = characterOrientation;
	_position = characterPosition;
	_oldPosition = _position;

	_currentHealth = _maxHealth;

	_circleCollider.position = characterPosition;
	_circleCollider.radius = 16.f;

	_healthTextSprite = std::make_shared<TextSprite>();
	_healthTextSprite->SetTargetPosition(Vector2<float>(windowWidth * 0.05f, windowHeight * 0.9f));
}

PlayerCharacter::~PlayerCharacter() {}

void PlayerCharacter::Init() {
	_healthTextSprite->Init("res/roboto.ttf", 24, std::to_string(_currentHealth).c_str(), { 255, 255, 255, 255 });
	_attackTimer = timerManager->CreateTimer(0.05f);
	_regenerationTimer = timerManager->CreateTimer(0.5f);
}

void PlayerCharacter::Update() {
	UpdateCollision();
	UpdateHealthRegen();
	UpdateInput();
	UpdateMovement();
	UpdateTarget();
}

void PlayerCharacter::Render() {

	_characterSprite->RenderWithOrientation(_position, _orientation);
}

void PlayerCharacter::RenderText() {
	_healthTextSprite->Render();
}

void PlayerCharacter::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	
	if (_currentHealth <= 0) {
		_currentHealth = 0;
		ExecuteDeath();
	}
	_healthTextSprite->ChangeText(std::to_string(_currentHealth).c_str(), { 255, 255, 255, 255 });
}

void PlayerCharacter::ExecuteDeath() {
	gameStateHandler->ReplaceCurrentState(std::make_shared<GameOverState>());
}

void PlayerCharacter::FireProjectile() {	
	projectileManager->SpawnProjectile(ProjectileType::PlayerProjectile, projectileManager->GetPlayerProjectileSprite(), _orientation, _attackDamage, _direction, _position);
}

void PlayerCharacter::Respawn() {
	_position = Vector2<float>(windowWidth * 0.5f, windowHeight * 0.5f);
	_orientation = 0.f;
	_attackTimer->ResetTimer();

	_currentHealth = _maxHealth;
	_healthTextSprite->ChangeText(std::to_string(_currentHealth).c_str(), { 255, 255, 255, 255 });

	enemyManager->RemoveAllEnemies();
	projectileManager->RemoveAllProjectiles();
}

void PlayerCharacter::UpdateCollision() {
	std::vector<std::shared_ptr<Projectile>> porjectilesHit = projectileManager->GetProjectileQuadTree()->Query(_circleCollider);
	for (unsigned int i = 0; i < porjectilesHit.size(); i++) {
		if (porjectilesHit[i]->GetProjectileType() == ProjectileType::PlayerProjectile) {
			continue;
		}
		TakeDamage(porjectilesHit[i]->GetProjectileDamage());
		projectileManager->RemoveProjectile(porjectilesHit[i]->GetProjectileType(), porjectilesHit[i]->GetObjectID());
	}
}

void PlayerCharacter::UpdateHealthRegen() {
	if (_currentHealth < _maxHealth) {
		if (_regenerationTimer->GetTimerFinished()) {
			_currentHealth += 1;
			if (_currentHealth > _maxHealth) {
				_currentHealth = _maxHealth;
			}
			_healthTextSprite->ChangeText(std::to_string(_currentHealth).c_str(), { 255, 255, 255, 255 });
			_regenerationTimer->ResetTimer();
		}
	}
}

void PlayerCharacter::UpdateInput() {
	if (GetMouseButton(SDL_BUTTON_LEFT)) {
		if (_attackTimer->GetTimerFinished()) {
			FireProjectile();
			_attackTimer->ResetTimer();
		}
	}
	if (GetKeyPressed(SDL_SCANCODE_ESCAPE)) {
		gameStateHandler->AddState(std::make_shared<PauseState>());
	}
}

void PlayerCharacter::UpdateMovement() { 
	_oldPosition = _position;
	if (GetKey(SDL_SCANCODE_W)) {
		_position.y -= _movementSpeed * deltaTime;

	} else if (GetKey(SDL_SCANCODE_S)) {
		_position.y += _movementSpeed * deltaTime;
	}	
	
	if (GetKey(SDL_SCANCODE_A)) {
		_position.x -= _movementSpeed * deltaTime;

	} else if (GetKey(SDL_SCANCODE_D)) {
		_position.x += _movementSpeed * deltaTime;
	}

	if (OutOfBorderX(_position.x)) {
		_position.x = _oldPosition.x;
	}
	if (OutOfBorderY(_position.y)) {
		_position.y = _oldPosition.y;
	}
	_circleCollider.position = _position;
}

void PlayerCharacter::UpdateTarget() {
	_direction = (GetCursorPosition() - _position).normalized();
	_orientation = VectorAsOrientation(_direction);
}

const Circle PlayerCharacter::GetCircleCollider() const {
	return _circleCollider;
}

std::shared_ptr<Sprite> PlayerCharacter::GetSprite() {
	return _characterSprite;
}

const float PlayerCharacter::GetOrientation() const {
	return _orientation;
}

const int PlayerCharacter::GetCurrentHealth() const {
	return _currentHealth;
}

const Vector2<float> PlayerCharacter::GetPosition() const {
	return _position;
}
