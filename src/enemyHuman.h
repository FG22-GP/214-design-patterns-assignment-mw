#pragma once
#include "collision.h"
#include "enemyBase.h"
#include "sprite.h"
#include "vector2.h"

class WeaponComponent;

class EnemyHuman : public EnemyBase {
public:
	EnemyHuman(unsigned int objectID, int maxHealth, float movementSpeed);
	~EnemyHuman();

	void Init() override;
	void Update() override;
	void Render() override;

	bool TakeDamage(unsigned int damageAmount) override;

	const Circle GetCollider() const override;
	const EnemyType GetEnemyType() const override;
	
	const float GetAttackDamage() const override;
	const float GetAttackRange() const override;
	const float GetOrientation() const override;
	const float GetRotation() const override;
	
	const int GetCurrentHealth() const override;
	const unsigned int GetObjectID() const override;

	const SlotClass GetSlotClass() const override;
	const std::shared_ptr<Sprite> GetSprite() const override;
	const std::shared_ptr<Timer> GetAttackTimer() const override;
	
	const Vector2<float> GetPosition() const override;
	const Vector2<float> GetTargetPosition() const override;
	const Vector2<float> GetVelocity() const override;
	
	const std::vector<std::shared_ptr<EnemyBase>> GetQueriedEnemies() const override;

	void ActivateEnemy(float orienation, Vector2<float> direction, Vector2<float> position) override;
	void DeactivateEnemy() override;
	void HandleAttack() override;

	void SetTargetPosition(Vector2<float> targetPosition) override;
	void SetTargetOrientation(float targetOrientation) override;

private:
	void UpdateMovement();
	void UpdateTarget();
	void PickWeapon();
	const char* _humanSprite = "res/sprites/Human.png";
	std::shared_ptr<WeaponComponent> _weaponComponent = nullptr;
};

