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
	std::vector<Laser> lasers;

private:
	Texture2D image;
	Vector2 position;
	double lastFireTime;
};
