#include "enemyHuman.h"

#include "dataStructuresAndMethods.h"
#include "debugDrawer.h"
#include "gameEngine.h"
#include "playerCharacter.h"
#include "projectileManager.h"
#include "quadTree.h"
#include "steeringBehavior.h"
#include "timerManager.h"
#include "weaponComponent.h"

EnemyHuman::EnemyHuman(unsigned int objectID, int maxHealth, float movementSpeed) :
	EnemyBase(objectID) {
	_sprite = std::make_shared<Sprite>();
	_sprite->Load(_humanSprite);

	_position = Vector2<float>(-10000.f, -10000.f);

	_circleCollider.position = _position;
	_circleCollider.radius = 12.f;
	
	_movementSpeed = movementSpeed;

	_maxHealth = maxHealth;
	_currentHealth = _maxHealth;

	_behaviorData.angularSlowDownRadius = PI * 0.5f;
	_behaviorData.angularTargetRadius = PI * 0.005f;
	_behaviorData.maxAngularAcceleration = PI;
	_behaviorData.maxRotation = PI;
	_behaviorData.timeToTarget = 0.1f;

	_enemyType = EnemyType::Human;

}

EnemyHuman::~EnemyHuman() {}

void EnemyHuman::Init() {
	_targetPosition = playerCharacter->GetPosition();
	_direction = Vector2<float>(_targetPosition - _position).normalized();

	_orientation = VectorAsOrientation(_direction);

	_attackTimer = timerManager->CreateTimer(1.f);

	PickWeapon();
}

void EnemyHuman::Update() {
	UpdateTarget();
	UpdateMovement();

	_queriedEnemies = enemyManager->GetEnemyQuadTree()->Query(_circleCollider);
	HandleAttack();
}

void EnemyHuman::Render() {
	_sprite->RenderWithOrientation(_position, _orientation);
	_weaponComponent->Render(_position, _orientation);
}

const Circle EnemyHuman::GetCollider() const {
	return _circleCollider;
}

const EnemyType EnemyHuman::GetEnemyType() const {
	return _enemyType;
}

const float EnemyHuman::GetAttackDamage() const {
	return _weaponComponent->GetAttackDamage();
}

const float EnemyHuman::GetAttackRange() const {
	return _weaponComponent->GetAttackRange();
}

const float EnemyHuman::GetOrientation() const {
	return _orientation;
}

const float EnemyHuman::GetRotation() const {
	return _rotation;
}

const int EnemyHuman::GetCurrentHealth() const {
	return _currentHealth;
}

const unsigned int EnemyHuman::GetObjectID() const {
	return _objectID;
}

const SlotClass EnemyHuman::GetSlotClass() const {
	return _slotClass;
}

const std::shared_ptr<Sprite> EnemyHuman::GetSprite() const {
	return _sprite;
}

const std::shared_ptr<Timer> EnemyHuman::GetAttackTimer() const {
	return _attackTimer;
}

const Vector2<float> EnemyHuman::GetPosition() const {
	return _position;
}

const Vector2<float> EnemyHuman::GetTargetPosition() const {
	return _targetPosition;
}

const Vector2<float> EnemyHuman::GetVelocity() const {
	return _velocity;
}

const std::vector<std::shared_ptr<EnemyBase>> EnemyHuman::GetQueriedEnemies() const {
	return _queriedEnemies;
}

void EnemyHuman::ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) {
	_orientation = orienation;
	_direction = direction;
	_position = position;
	_circleCollider.position = position;
	Init();
}

void EnemyHuman::DeactivateEnemy() {
	_orientation = 0.f;
	_direction = Vector2<float>(0.f, 0.f);
	_position = Vector2<float>(-10000.f, -10000.f);
	_circleCollider.position = _position;
	_attackTimer->DeactivateTimer();
}

void EnemyHuman::HandleAttack() {
	//Depending on the weapon, the attack works differently
	_weaponComponent->Attack(_position, _targetPosition, _orientation);
}

void EnemyHuman::SetTargetPosition(Vector2<float> targetPosition) {
	_targetPosition = targetPosition;
}

void EnemyHuman::SetTargetOrientation(float targetOrientation) {
	_targetPosition = targetOrientation;
}

bool EnemyHuman::TakeDamage(unsigned int damageAmount) {
	_currentHealth -= damageAmount;
	if (_currentHealth <= 0) {
		return true;
	}
	return false;
}

void EnemyHuman::UpdateMovement() {
	//The enemy walks towards the player until it is in range
	_direction = Vector2<float>(_targetPosition - _position).normalized();
	if (!IsInDistance(_position, playerCharacter->GetPosition(), _weaponComponent->GetAttackRange() * 0.5f)) {
		_position += _direction * _movementSpeed * deltaTime;
		_circleCollider.position = _position;
	}
	//I'm using separation behavior to prevent the enemies from stacking ontop of each other
	//enemyManager->GetSteeringBehaviors()[SteeringBehaviorType::FaceBehavior]->Steering(_behaviorData, this).angularVelocity;
	
	_position += separationBehavior->Steering(_behaviorData, this).linearVelocity * deltaTime;
	_orientation = VectorAsOrientation(_direction);
}

void EnemyHuman::UpdateTarget() {
	_targetPosition = playerCharacter->GetPosition();
}

void EnemyHuman::PickWeapon() {
	/*Human enemy picks a weapon component at spawn
	to either fight in melee range or shoot fireballs*/
	std::uniform_int_distribution dist{ 0, 1 };
	int temp = dist(randomEngine);
	if (temp == 0) {
		_weaponComponent = std::make_shared<SwordComponent>();

	} else {
		_weaponComponent = std::make_shared<StaffComponent>();
	}
}
