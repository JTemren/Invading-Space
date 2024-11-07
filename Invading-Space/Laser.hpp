#pragma once
#include <raylib.h>

class Laser
{
public:
	Laser(Vector2 position, int speed);
	void update();
	void Draw();
	bool active;
	Rectangle getRectangle();

private:
	Vector2 position;
	int speed;
};