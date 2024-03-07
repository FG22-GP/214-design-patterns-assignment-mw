#pragma once
#include "sprite.h"
#include "vector2.h"

class Timer;

class ObjectBase {
public:
	ObjectBase(unsigned int objectID) : _objectID(objectID) {}
	~ObjectBase() {}
	
protected:
	float _orientation = 0.f;
	
	const int _objectID;

	std::shared_ptr<Sprite> _sprite = nullptr;

	Vector2<float> _position = Vector2<float>(-10000.f, -10000.f);
};

