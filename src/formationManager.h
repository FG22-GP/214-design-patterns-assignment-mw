#pragma once
#include "vector2.h"

#include <climits>
#include <vector>
#include <memory>

class EnemyBase;

enum class FormationType {
	DefensiveCircle,
	SlotRole,
	Count
};

enum class SlotClass {
	Archer,
	Fighter,
	Mage,
	Count
};

enum class SlotAttackType {
	Magic,
	Melee,
	Missle,
	Count
};

struct SlotOffset {
	Vector2<float> position = Vector2<float>(0.f, 0.f);
	float orientation = 0.f;
};

struct CostAndSlot {
	float cost = 0.f;
	unsigned int slotNumber = 0;
};

struct SlotPositionAndType {
	unsigned int slotNumber = 0;
	SlotOffset positionAndOrientation;
	SlotAttackType slotAttackType;
};

struct CharacterAndSlots {
	std::shared_ptr<EnemyBase> enemyCharacter = nullptr;
	float assignmentEase = 0.f;
	std::vector<CostAndSlot> costAndSlots;
};

struct SlotAssignment {
	std::shared_ptr<EnemyBase> enemyCharacter;
	unsigned int slotNumber = INT_MAX;
};

std::vector<CostAndSlot> SortByCost(std::vector<CostAndSlot> slotsAndCosts);
std::vector<CharacterAndSlots> SortByAssignmentEase(std::vector<CharacterAndSlots> characterAndSlots);

class FormationPattern {
public:
	virtual unsigned int CalculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments) = 0;

	virtual SlotOffset GetDriftOffset(std::vector<SlotAssignment> slotAssignments, unsigned int numberOfSlots) = 0;
	virtual SlotOffset GetSlotLocation(float slotNumber, unsigned int numberOfSlots) = 0;

	virtual float GetSlotCost(SlotClass slotClass, unsigned int index) = 0;

	virtual const unsigned int GetNumberOfSlots() const = 0;

	virtual bool SupportsSlots(unsigned int slotCount) = 0;
};

class DefensiveCirclePattern : public FormationPattern {
public:
	DefensiveCirclePattern() {}
	~DefensiveCirclePattern() {}

	unsigned int CalculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments) override;

	SlotOffset GetDriftOffset(std::vector<SlotAssignment> slotAssignments, unsigned int numberOfSlots) override;
	SlotOffset GetSlotLocation(float slotNumber, unsigned int numberOfSlots) override;
	float GetSlotCost(SlotClass slotClass, unsigned int index) override;

	const unsigned int GetNumberOfSlots() const override;

	bool SupportsSlots(unsigned int slotCount) override;

private:
	unsigned int _numberOfSlots = 9;
	float _characterRadius = 25.f;
};

class SlotRolePattern : public FormationPattern {
public:
	SlotRolePattern();
	~SlotRolePattern() {}
	unsigned int CalculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments) override;

	SlotOffset GetDriftOffset(std::vector<SlotAssignment> slotAssignments, unsigned int numberOfSlots) override;
	SlotOffset GetSlotLocation(float slotNumber, unsigned int numberOfSlots) override;

	float GetSlotCost(SlotClass slotClass, unsigned int index) override;

	const unsigned int GetNumberOfSlots() const override;

	bool SupportsSlots(unsigned int slotCount) override;

private:
	unsigned int _numberOfSlots = 9;
	std::vector<SlotPositionAndType> _slotPositionAndType;
	float _characterRadius = 25.f;
};

class FormationManager {
public:
	FormationManager(FormationType formationType);
	~FormationManager() {}

	bool AddCharacter(std::shared_ptr<EnemyBase> enemyCharacter);
	void UpdateSlots();
	void UpdateSlotAssignments();
	void RemoveCharacter(std::shared_ptr<EnemyBase> enemyCharacter);

	std::vector<SlotAssignment> GetSlotAssignments();
	SlotOffset GetAnchorPoint();

private:
	SlotOffset _driftOffset;
	std::shared_ptr<FormationPattern> _formationPattern;
	std::vector<SlotAssignment> _slotAssignments;
	float _costLimit = 1500.f;
	float _orientation = 0.f;
	unsigned int _numberOfSlots = 0;
};

