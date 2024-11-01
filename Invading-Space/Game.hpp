#pragma once
#include "SpaceShip.hpp"
#include "Obstacle.hpp"
#include "Alien.hpp"

class Game {

	public:
		Game();
		~Game();

		void Draw();
		void Update();
		void HandleInput();



	private:
		void DeleteInactiveLasers();
		SpaceShip SpaceShip;

		std::vector<Obstacle> obstacles;
		std::vector<Obstacle> CreateObstacles();
		std::vector<Alien> aliens;
		std::vector<Alien> CreateAliens();




};