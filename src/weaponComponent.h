#pragma once
#include "vector2.h"

#include <memory>

class Sprite;
class Timer;

class WeaponComponent {
public:
	WeaponComponent() {}
	~WeaponComponent() {}

	virtual void Render(Vector2<float> position, float orientation) = 0;

	virtual void Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) = 0;

	const virtual float GetAttackDamage() const = 0;
	const virtual float GetAttackRange() const = 0;

protected:
	std::shared_ptr<Sprite> _sprite = nullptr;
	std::shared_ptr<Timer> _attackTimer = nullptr;

	int _attackDamage = 0;
	float _attackRange = 0.f;
};

class SwordComponent : public WeaponComponent {
public:
	SwordComponent();
	~SwordComponent() {}

	void Render(Vector2<float> position, float orientation) override;

	void Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) override;

	const float GetAttackDamage() const;
	const float GetAttackRange() const;

};

class StaffComponent : public WeaponComponent {
public:
	StaffComponent();
	~StaffComponent() {}

	void Render(Vector2<float> position, float orientation) override;

	void Attack(Vector2<float> position, Vector2<float> targetPosition, float orientation) override;

	const float GetAttackDamage() const;
	const float GetAttackRange() const;
};