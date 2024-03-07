#include "debugDrawer.h"
#include "gameEngine.h"

#include <SDL2/SDL.h>

void DebugDrawer::AddDebugBox(Vector2<float> position, Vector2<float> min, Vector2<float> max, std::array<int, 4> color) {
	DebugBox debugBox;
	debugBox.min = min;
	debugBox.max = max;
	debugBox.position = position;
	debugBox.color = color;
	_debugBox.emplace_back(debugBox);
}

void DebugDrawer::AddDebugCircle(Vector2<float> position, float radius, std::array<int, 4> color) {
	DebugCircle debugCircle;
	debugCircle.position = position;
	debugCircle.radius = radius;
	debugCircle.color = color;
	_debugCircles.emplace_back(debugCircle);
}

void DebugDrawer::AddDebugCross(Vector2<float> position, float length, std::array<int, 4> color) {
	DebugLine debugLineHorizontal;
	DebugLine debugLineVertical;

	debugLineHorizontal.startPosition = Vector2<float>(position.x - (length * 0.5f), position.y);
	debugLineHorizontal.endPosition = Vector2<float>(position.x + (length * 0.5f), position.y);
	debugLineHorizontal.color = color;

	debugLineVertical.startPosition = Vector2<float>(position.x, position.y - (length * 0.5f));
	debugLineVertical.endPosition = Vector2<float>(position.x, position.y + (length * 0.5f));
	debugLineVertical.color = color;

	_debugLines.emplace_back(debugLineHorizontal);
	_debugLines.emplace_back(debugLineVertical);
}

void DebugDrawer::AddDebugLine(Vector2<float> startPosition, Vector2<float> endPosition, std::array<int, 4> color) {
	DebugLine debugLine;
	debugLine.startPosition = startPosition;
	debugLine.endPosition = endPosition;
	debugLine.color = color;
	_debugLines.emplace_back(debugLine);
}

void DebugDrawer::DrawBoxes() {
	for (int i = 0; i < _debugBox.size(); i++) {
		SDL_SetRenderDrawColor(renderer, _debugBox[i].color[0], _debugBox[i].color[1], _debugBox[i].color[2], _debugBox[i].color[3]);
		SDL_Rect rect = {
			_debugBox[i].position.x - ((_debugBox[i].max.x - _debugBox[i].min.x) * 0.5f),
			_debugBox[i].position.y - ((_debugBox[i].max.y - _debugBox[i].min.y) * 0.5f),
			_debugBox[i].max.x - _debugBox[i].min.x,
			_debugBox[i].max.y - _debugBox[i].min.y			
		};
		SDL_RenderDrawRect(renderer, &rect);
	}
	_debugBox.clear();
}

void DebugDrawer::DrawCircles() {
	for (int i = 0; i < _debugCircles.size(); i++) {
		SDL_SetRenderDrawColor(renderer, _debugBox[i].color[0], _debugBox[i].color[1], _debugBox[i].color[2], _debugBox[i].color[3]);

		int resolution = 24;
		float step = (2 * PI) / resolution;

		for (int k = 0; k < resolution; k++) {

			float angle = step * k;
			float x1 = cos(angle);
			float y1 = sin(angle);

			float nextAngle = step * (k + 1);
			float x2 = cos(nextAngle);
			float y2 = sin(nextAngle);

			SDL_RenderDrawLine(
				renderer,
				x1 * _debugCircles[i].radius + _debugCircles[i].position.x,
				y1 * _debugCircles[i].radius + _debugCircles[i].position.y,
				x2 * _debugCircles[i].radius + _debugCircles[i].position.x,
				y2 * _debugCircles[i].radius + _debugCircles[i].position.y);

		}
	}
	_debugCircles.clear();
}

void DebugDrawer::DrawLines() {
	for (int i = 0; i < _debugLines.size(); i++) {
	SDL_SetRenderDrawColor(renderer, _debugLines[i].color[0], _debugLines[i].color[1], _debugLines[i].color[2], _debugLines[i].color[3]);
		SDL_RenderDrawLineF(renderer, _debugLines[i].startPosition.x, _debugLines[i].startPosition.y, 
			_debugLines[i].endPosition.x, _debugLines[i].endPosition.y);
	}
	_debugLines.clear();
}
