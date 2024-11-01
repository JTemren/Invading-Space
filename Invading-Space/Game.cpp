#include "Game.hpp"
#include <iostream>

Game::Game(){
	obstacles = CreateObstacles();
	aliens = CreateAliens();
}

Game:: ~Game() {

}

void Game::Update() {
	for (auto& Laser: SpaceShip.lasers)
	{
		Laser.update();
	}
	DeleteInactiveLasers();
}

void Game::Draw() {
	SpaceShip.Draw();

	for (auto& Laser : SpaceShip.lasers) {
		Laser.Draw();
	}

	for (auto& obstacle: obstacles)
	{
		obstacle.Draw();
	}
	for (auto& alien : aliens) {
		alien.Draw();
	}
}

void Game::HandleInput() {
	if (IsKeyDown(KEY_LEFT))
	{
		SpaceShip.MoveLeft();
	}
	else if (IsKeyDown(KEY_RIGHT)) {
		SpaceShip.MoveRight();
	}
	else if (IsKeyDown(KEY_SPACE)) {
		SpaceShip.FireLaser();
	}
}

void Game::DeleteInactiveLasers()
{
	for (auto it = SpaceShip.lasers.begin(); it != SpaceShip.lasers.end();)
	{
		if (!it->active)
		{
			it = SpaceShip.lasers.erase(it);
		}
		else {
			++it;
		}
	}
}

std::vector<Obstacle> Game::CreateObstacles()
{
	int obstaclewigth = Obstacle::grid[0].size() * 3;
	float gap = (GetScreenWidth() - (4 * obstaclewigth)) / 5;

	for (int i = 0; i < 4; i++)
	{
		float offsetX = (i + 1) * gap + i * obstaclewigth;
		obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 100) }));
	}

	return obstacles;
}

std::vector<Alien> Game::CreateAliens()
{
	std::vector<Alien> aliens;
	for (int row = 0; row < 5; row++)
	{
		for (int coloumn = 0; coloumn < 11; coloumn++)
		{
			int alienType;
			if (row == 0)
			{
				alienType = 3;
			}
			else if (row == 1 || row == 2) 
			{
				alienType = 2;
			}
			else {
				alienType = 1;
			}

			float x = 75 + coloumn * 55;
			float y = 110 + row * 55;
			aliens.push_back(Alien(alienType, { x, y }));

		}
	}
	return aliens;
}
