#pragma once
#include "vector2.h"

#include <array>
#include <vector>

struct DebugBox {
	Vector2<float> min = Vector2<float>(0.f, 0.f);
	Vector2<float> max = Vector2<float>(0.f, 0.f);
	Vector2<float> position = Vector2<float>(0.f, 0.f);
	std::array<int, 4> color = { 255, 255, 255, 255 };
};
struct DebugCircle {
	Vector2<float> position = Vector2<float>(0.f, 0.f);
	float radius = 0.f;
	std::array<int, 4> color = { 255, 255, 255, 255 };
};
struct DebugLine {
	Vector2<float> startPosition = Vector2<float>(0.f, 0.f);
	Vector2<float> endPosition = Vector2<float>(0, 0.f);
	std::array<int, 4> color = { 255, 255, 255, 255 };
};


class DebugDrawer {
public:
	DebugDrawer() {}
	~DebugDrawer() {}

	void AddDebugBox(Vector2<float> position, Vector2<float> min, Vector2<float> max, std::array<int, 4> color);
	void AddDebugCircle(Vector2<float> position, float radius, std::array<int, 4> color);
	void AddDebugCross(Vector2<float> position, float length, std::array<int, 4> color);
	void AddDebugLine(Vector2<float> startPosition, Vector2<float> endPosition, std::array<int, 4> color);

	void DrawBoxes();
	void DrawCircles();
	void DrawLines();

private:

	std::vector<DebugBox> _debugBox;
	std::vector<DebugLine> _debugLines;
	std::vector<DebugCircle> _debugCircles;

};

