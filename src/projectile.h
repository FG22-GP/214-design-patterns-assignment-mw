#pragma once
#include "collision.h"
#include "objectBase.h"
#include "sprite.h"
#include "vector2.h"

enum class ProjectileType {
	EnemyProjectile,
	PlayerProjectile,
	Count
};

class Projectile : public ObjectBase {
public:
	Projectile(ProjectileType damageType, const char* spritePath, unsigned int projectileDamage, unsigned int objectID);
	~Projectile();

	void Init();
	void Update();
	void Render();
	
	const Circle GetCollider() const;
	const ProjectileType GetProjectileType() const;
	const unsigned int GetProjectileDamage() const;
	
	const unsigned int GetObjectID() const;
	const float GetOrientation() const;
	const std::shared_ptr<Sprite> GetSprite() const;
	const Vector2<float> GetPosition() const;
	
	void SetDirection(Vector2<float> direction);
	void SetTargetOrientation(float orientation);
	void SetTargetPosition(Vector2<float> position);

	void ActivateProjectile(float orientation, Vector2<float> direction, Vector2<float> position);
	void DeactivateProjectile();

private:
	Circle _circleCollider;

	const float _projectileSpeed = 200.f;
	const float _spriteCollisionOffset = 8.f;

	ProjectileType _projectileType = ProjectileType::Count;	

	unsigned int _projectileDamage;

	Vector2<float> _direction = Vector2<float>(0.f, 0.f);
};

