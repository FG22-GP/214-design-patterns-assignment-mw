#pragma once
#include "vector2.h"

#include <vector>

bool IsInDistance(Vector2<float> positionA, Vector2<float> positionB, float distance);
bool OutOfBorderX(float positionX);
bool OutOfBorderY(float positionY);

Vector2<float> GetCursorPosition();
Vector2<float>  OrientationAsVector(float orientation);
Vector2<float> RotateVector(float degree, Vector2<float> startPoint, Vector2<float> endPoint);

float RandomBinomal(float a, float b);

float VectorAsOrientation(Vector2<float> direction);

float WrapMax(float rotation, float maxValue);
float WrapMinMax(float rotation, float minValue, float maxValue);