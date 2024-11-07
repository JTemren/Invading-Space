#pragma once 
#include <raylib.h>
#include "laser.hpp"
#include <vector>

class SpaceShip {
public:
	SpaceShip();
	~SpaceShip();
	void Draw();
	void MoveLeft();
	void MoveRight();
	void FireLaser();
	Rectangle getRectangle();
	std::vector<Laser> lasers;
	void Reset();

private:
	Texture2D image;
	Vector2 position;
	double lastFireTime;
	Sound laserSound;
};
