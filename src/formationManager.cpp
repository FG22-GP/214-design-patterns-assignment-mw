#include "formationManager.h"
#include "enemyBase.h"
#include <dataStructuresAndMethods.h>

FormationManager::FormationManager(FormationType formationType) {
	switch (formationType) {
	case FormationType::DefensiveCircle:
		_formationPattern = std::make_shared<DefensiveCirclePattern>();
		break;
	case FormationType::SlotRole:
		_formationPattern = std::make_shared<SlotRolePattern>();
		break;
	case FormationType::Count:
		break;
	default:
		break;
	}
}

bool FormationManager::AddCharacter(std::shared_ptr<EnemyBase> enemyCharacter) {
	unsigned int occupiedSlots = _slotAssignments.size();

	if (_formationPattern->SupportsSlots(occupiedSlots + 1)) {
		SlotAssignment slotAssignment;
		slotAssignment.enemyCharacter = enemyCharacter;
		slotAssignment.slotNumber = _slotAssignments.size();
		_numberOfSlots++;
		_slotAssignments.emplace_back(slotAssignment);
		UpdateSlotAssignments();
		return true;
	}
	return false;
}

void FormationManager::UpdateSlots() {
	SlotOffset anchorPoint = GetAnchorPoint();
	Vector2 orientationVector = OrientationAsVector(anchorPoint.orientation);

	for (unsigned int i = 0; i < _slotAssignments.size(); i++) {
		unsigned int slotNumber = _slotAssignments[i].slotNumber;
		SlotOffset slot = _formationPattern->GetSlotLocation(slotNumber, _numberOfSlots);
		SlotOffset location;
		location.position = anchorPoint.position + slot.position;
		location.orientation = anchorPoint.orientation + slot.orientation;

		location.position -= _driftOffset.position;
		location.orientation -= _driftOffset.orientation;

		_slotAssignments[i].enemyCharacter->SetTargetPosition(location.position);
		_slotAssignments[i].enemyCharacter->SetTargetOrientation(location.orientation);
	}
}

void FormationManager::UpdateSlotAssignments() {
	std::vector<CharacterAndSlots> characterAndSlots;
	for (unsigned int i = 0; i < _slotAssignments.size(); ++i) {
		CharacterAndSlots datum;
		datum.enemyCharacter = _slotAssignments[i].enemyCharacter;
		for (unsigned int k = 0; k < _formationPattern->GetNumberOfSlots(); ++k) {
			float slotCost = _formationPattern->GetSlotCost(_slotAssignments[i].enemyCharacter->GetSlotClass(), k);
			if (slotCost >= _costLimit) {
				continue;
			}
			CostAndSlot slotDatum;
			slotDatum.slotNumber = k;
			slotDatum.cost = slotCost;
			datum.costAndSlots.emplace_back(slotDatum);

			datum.assignmentEase += 1 / (1 + slotCost);
		}
		datum.costAndSlots = SortByCost(datum.costAndSlots);
		characterAndSlots.emplace_back(datum);
	}
	characterAndSlots = SortByAssignmentEase(characterAndSlots);
	std::vector<bool> filledSlots;
	for (unsigned int i = 0; i < _formationPattern->GetNumberOfSlots(); i++) {
		filledSlots.emplace_back(false);
	}
	std::vector<SlotAssignment> slotAssignments;
	for (unsigned int i = 0; i < characterAndSlots.size(); i++) {
		for (unsigned int k = 0; k < characterAndSlots[i].costAndSlots.size(); k++) {
			if (!filledSlots[characterAndSlots[i].costAndSlots[k].slotNumber]) {
				SlotAssignment assignment;
				assignment.enemyCharacter = characterAndSlots[i].enemyCharacter;
				assignment.slotNumber = characterAndSlots[i].costAndSlots[k].slotNumber;

				slotAssignments.emplace_back(assignment);
				filledSlots[characterAndSlots[i].costAndSlots[k].slotNumber] = true;
				break;
			}
		}
	}
	_slotAssignments = slotAssignments;
	_driftOffset = _formationPattern->GetDriftOffset(_slotAssignments, _numberOfSlots);
}

void FormationManager::RemoveCharacter(std::shared_ptr<EnemyBase> enemyCharacter) {
	SlotAssignment slotAssignment;
	for (unsigned int i = 0; i < _slotAssignments.size(); i++) {
		if (_slotAssignments[i].enemyCharacter->GetObjectID() == enemyCharacter->GetObjectID()) {
			_slotAssignments[i] = _slotAssignments.back();
			_slotAssignments.pop_back();
			break;
		}
	}
	UpdateSlotAssignments();
}

std::vector<SlotAssignment> FormationManager::GetSlotAssignments() {
	return _slotAssignments;
}

SlotOffset FormationManager::GetAnchorPoint() {
	SlotOffset temp;
	
	temp.position = GetCursorPosition();
	temp.orientation = 0.f;
	return temp;
}

unsigned int DefensiveCirclePattern::CalculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments) {
	return slotAssignments.size();
}

SlotOffset DefensiveCirclePattern::GetDriftOffset(std::vector<SlotAssignment> slotAssignments, unsigned int numberOfSlots) {
	SlotOffset result;
	for (unsigned int i = 0; i < slotAssignments.size(); i++) {
		SlotOffset location = GetSlotLocation(slotAssignments[i].slotNumber, numberOfSlots);
		result.position += location.position;
		result.orientation += location.orientation;
	}
	result.position /= numberOfSlots;
	result.orientation /= numberOfSlots;
	return result;
}

SlotOffset DefensiveCirclePattern::GetSlotLocation(float slotNumber, unsigned int numberOfSlots) {
	float angleAroundCircle = slotNumber / numberOfSlots * PI * 2;
	float radius = _characterRadius / sin(PI / numberOfSlots);

	SlotOffset result;
	result.position.x = radius * sin(angleAroundCircle);
	result.position.y = radius * -cos(angleAroundCircle);
	result.orientation = WrapMinMax(angleAroundCircle, -PI, PI);
	return result;
}

float DefensiveCirclePattern::GetSlotCost(SlotClass slotClass, unsigned int index) {
	return 0.0f;
}

const unsigned int DefensiveCirclePattern::GetNumberOfSlots() const {
	return _numberOfSlots;
}

bool DefensiveCirclePattern::SupportsSlots(unsigned int slotCount) {
	return true;
}

SlotRolePattern::SlotRolePattern() {}

unsigned int SlotRolePattern::CalculateNumberOfSlots(std::vector<SlotAssignment> slotAssignments) {
	return slotAssignments.size();
}

SlotOffset SlotRolePattern::GetDriftOffset(std::vector<SlotAssignment> slotAssignments, unsigned int numberOfSlots) {
	SlotOffset result;
	for (unsigned int i = 0; i < slotAssignments.size(); i++) {
		SlotOffset location = GetSlotLocation(slotAssignments[i].slotNumber, numberOfSlots);
		result.position += location.position;
		result.orientation += location.orientation;
	}
	result.position /= numberOfSlots;
	result.orientation /= numberOfSlots;
	return result;
}

SlotOffset SlotRolePattern::GetSlotLocation(float slotNumber, unsigned int numberOfSlots) {
	SlotOffset result;
	result.position = _slotPositionAndType[slotNumber].positionAndOrientation.position;
	result.orientation = _slotPositionAndType[slotNumber].positionAndOrientation.orientation;
	result.orientation = WrapMinMax(result.orientation, -PI, PI);
	return result;
}

float SlotRolePattern::GetSlotCost(SlotClass slotClass, unsigned int index) {
	switch (slotClass) {
	case SlotClass::Archer:
	{
		switch (_slotPositionAndType[index].slotAttackType) {
		case SlotAttackType::Magic:
			return 1000.f;
		case SlotAttackType::Melee:
			return 1500.f;
		case SlotAttackType::Missle:
			return 0.f;
		default:
			break;
		}
		break;
	}
	case SlotClass::Fighter:
	{
		switch (_slotPositionAndType[index].slotAttackType) {
		case SlotAttackType::Magic:
			return 2000.f;
		case SlotAttackType::Melee:
			return 0.f;
		case SlotAttackType::Missle:
			return 1500.f;
		default:
			break;
		}
		break;
	}
	case SlotClass::Mage:
		switch (_slotPositionAndType[index].slotAttackType) {
		case SlotAttackType::Magic:
			return 0.f;
		case SlotAttackType::Melee:
			return 2000.f;
		case SlotAttackType::Missle:
			return 500.f;
		default:
			break;
		}
	default:
		break;
	}
}

const unsigned int SlotRolePattern::GetNumberOfSlots() const {
	return _numberOfSlots;
}

bool SlotRolePattern::SupportsSlots(unsigned int slotCount) {
	return true;
}
