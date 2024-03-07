#include "weaponComponent.h"

#include "dataStructuresAndMethods.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "sprite.h"
#include "timerManager.h"

SwordComponent::SwordComponent() {
	_sprite = std::make_shared<Sprite>();
	_sprite->Load("res/sprites/Sword.png");
	_attackTimer = timerManager->CreateTimer(1.f);
	_attackDamage = 2;
	_attackRange = 25.f;
}

void SwordComponent::Render(Vector2<float> position, float orientation) {
	_sprite->RenderWithOrientation(position, orientation);
}
//If the weapon is a sword it damages the player if its close enough
void SwordComponent::Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) {
	if (IsInDistance(playerCharacter->GetPosition(), position, _attackRange) && _attackTimer->GetTimerFinished()) {
		playerCharacter->TakeDamage(_attackDamage);
		_attackTimer->ResetTimer();
	}
}

const float SwordComponent::GetAttackDamage() const {
	return _attackDamage;
}

const float SwordComponent::GetAttackRange() const {
	return _attackRange;
}

StaffComponent::StaffComponent() {
	_sprite = std::make_shared<Sprite>();
	_sprite->Load("res/sprites/Staff.png");
	_attackTimer = timerManager->CreateTimer(1.5f);
	_attackDamage = 1;
	_attackRange = 300.f;
}

void StaffComponent::Render(Vector2<float> position, float orientation) {
	_sprite->RenderWithOrientation(position, orientation);
}
//If the weapon is a staff it shoots a fireball towards the player
void StaffComponent::Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) {
	if (IsInDistance(playerCharacter->GetPosition(), position, _attackRange) && _attackTimer->GetTimerFinished()) {
		projectileManager->SpawnProjectile(ProjectileType::EnemyProjectile, 
			projectileManager->GetEnemyProjectileSprite(), orientation, _attackDamage,
			Vector2<float>(targetPosition - position).normalized(), position);
		_attackTimer->ResetTimer();
	}
}

const float StaffComponent::GetAttackDamage() const {
	return _attackDamage;
}

const float StaffComponent::GetAttackRange() const {
	return _attackRange;
}
