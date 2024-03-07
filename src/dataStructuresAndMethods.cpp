#include "dataStructuresAndMethods.h"
#include "gameEngine.h"
#include "objectBase.h"


bool IsInDistance(Vector2<float> positionA, Vector2<float> positionB, float distance) {
	return Vector2<float>::distanceBetweenVectors(positionA, positionB) <= distance;
}

bool OutOfBorderX(float positionX) {
	return positionX < 0.f || positionX > windowWidth;
}

bool OutOfBorderY(float positionY) {
	return positionY < 0.f || positionY > windowHeight;
}

Vector2<float> GetCursorPosition() {
	int mousePositionX;
	int mousePositionY;
	SDL_GetMouseState(&mousePositionX, &mousePositionY);
	Vector2<float> mousePosition = Vector2<float>(mousePositionX, mousePositionY);
	return mousePosition;

}

Vector2<float> OrientationAsVector(float orientation) {
	return  Vector2<float>(-sinf(orientation), cosf(orientation)).normalized();
}

Vector2<float> RotateVector(float degree, Vector2<float> startPoint, Vector2<float> endPoint) {
	Vector2 direction = endPoint - startPoint;
	Vector2 rotatedVector =
		Vector2(direction.x * cos(degree) - direction.y * sin(degree),
			direction.x * sin(degree) + direction.y * cos(degree));
	return rotatedVector;
}

float RandomBinomal(float a, float b) {
	std::uniform_real_distribution<float> dist(a, b);
	float randomNumber = dist(randomDevice);
	return randomNumber;
}

float VectorAsOrientation(Vector2<float> direction) {
	direction.normalize();
	return atan2f(direction.x, -direction.y);
}

float WrapMax(float rotation, float maxValue) {
	return fmod(maxValue + fmod(rotation, maxValue), maxValue);
}

float WrapMinMax(float rotation, float minValue, float maxValue) {
	return minValue + WrapMax(rotation - minValue, maxValue - minValue);
}

/*

int BinarySearch(std::vector<std::shared_ptr<ObjectBase>> objectVector, int low, int high, int targetID) {
	while (low <= high) {
		int mid = low + (high - low) / 2;

		if (objectVector[mid]->GetObjectID() == targetID) {
			return mid;
		}

		if (objectVector[mid]->GetObjectID() < targetID) {
			low = mid + 1;
		}
		else {
			high = mid - 1;
		}
	}
	return -1;
}

int Partition(std::vector<std::shared_ptr<ObjectBase>> objectVector, int start, int end) {
	int pivot = objectVector[start]->GetObjectID();

	int count = 0;
	for (int i = start + 1; i <= end; i++) {
		if (objectVector[i]->GetObjectID() <= pivot) {
			count++;
		}
	}

	int pivotIndex = start + count;
	std::swap(objectVector[pivotIndex], objectVector[start]);

	int i = start, k = end;

	while (i < pivotIndex && k > pivotIndex) {

		while (objectVector[i]->GetObjectID() <= pivot) {
			i++;
		}

		while (objectVector[k]->GetObjectID() > pivot) {
			k--;
		}

		if (i < pivotIndex && k > pivotIndex) {
			std::swap(objectVector[i++], objectVector[k--]);
		}
	}
	return pivotIndex;
}
void QuickSort(std::vector<std::shared_ptr<ObjectBase>> objectVector, int start, int end) {
	if (start >= end) {
		return;
	}
	int p = Partition(objectVector, start, end);

	QuickSort(objectVector, start, p - 1);
	QuickSort(objectVector, p + 1, end);
}
*/