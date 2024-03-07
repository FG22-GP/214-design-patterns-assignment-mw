#pragma once
#include "quadTree.h"
#include "vector2.h"

#include <vector>
#include <unordered_map>
#include <memory>

class EnemyBase;
class SteeringBehavior;
class Timer;
template<typename T> class ObjectPool;
template<typename T> class QuadTree;

enum class EnemyType;

enum class SteeringBehaviorType {
	Align,
	Arrive,
	CollisionAbvoidance,
	FaceBehavior,
	LookAtDirection,
	Pursue,
	Seek,
	Separation,
	Velocity,
	Wander,
	Count
};


enum class EnemyType {
	Boar,
	Human,
	Count
};

struct BehaviorData {
	float targetOrientation = 0.f;

	float timeToTarget = 0.f;

	Vector2<float> targetPosition = Vector2<float>{ 0.f, 0.f };

	//align data
	float angularSlowDownRadius = 0.f;
	float angularTargetRadius = 0.f;
	float maxAngularAcceleration = 0.f;
	float maxRotation = 0.f;

	//seek data
	float maxLinearAcceleration = 0.f;
	float maxSpeed = 0.f;
	float linearSlowDownRadius = 0.f;
	float linearTargetRadius = 0.f;

	//pursue data
	float maxPrediction = 0.f;
	Vector2<float> targetVelocity = Vector2<float>{ 0.f, 0.f };

	//collision avoidance data
	float characterRadius = 0.f;

	//Obstacle avoidance data
	float avoidDistance = 0.f;
	float lookAhead = 0.f;

	//separation data
	float separationThreshold = 0.f;
	float attractionThreshold = 0.f;
	float decayCoefficient = 0.f;

	//wander data
	float wanderOffset = 0.f;
	float wanderRadius = 0.f;
	float wanderRate = 0.f;


};

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void Update();
	void Render();

	std::vector<std::shared_ptr<EnemyBase>> GetActiveEnemies();
	std::shared_ptr<QuadTree<std::shared_ptr<EnemyBase>>> GetEnemyQuadTree();
	std::unordered_map<SteeringBehaviorType, std::shared_ptr<SteeringBehavior>> GetSteeringBehaviors();

	void ClearEnemyQuadTree();

	void CreateNewEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void EnemySpawner();

	void SpawnEnemy(EnemyType enemyType, float orientation,
		Vector2<float> direction, Vector2<float> position);

	void RemoveAllEnemies();
	void RemoveEnemy(EnemyType enemyType, unsigned int objectID);

	void TakeDamage(unsigned int enemyIndex, unsigned int damageAmount);

	void UpdateQuadTree();

	int BinarySearch(int low, int high, int objectID);

	int Partition(int start, int end);
	void QuickSort( int start, int end);

private:
	std::shared_ptr<QuadTree<std::shared_ptr<EnemyBase>>> _enemyQuadTree;

	std::vector<std::shared_ptr<EnemyBase>> _activeEnemies;

	std::shared_ptr<Timer> _spawnTimer = nullptr;

	std::unordered_map<EnemyType, std::shared_ptr<ObjectPool<std::shared_ptr<EnemyBase>>>> _enemyPools;

	std::unordered_map<SteeringBehaviorType, std::shared_ptr<SteeringBehavior>> _steeringBehaviors;

	int _lastEnemyID = 0;
	int _latestEnemyIndex = -1;

	unsigned int _enemyAmountLimit = 1000;
	unsigned int _numberOfEnemyTypes = 0;
	unsigned int _spawnNumberOfEnemies = 25;
};

