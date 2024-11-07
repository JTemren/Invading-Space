#pragma once
#include "SpaceShip.hpp"
#include "Obstacle.hpp"
#include "Alien.hpp"
#include "MysteryShip.hpp"

class Game {

	public:
		Game();
		~Game();

		void Draw();
		void Update();
		void HandleInput();
		bool run;
		int lives;
		int score;
		int highScore;
		Music music;


	private:
		void DeleteInactiveLasers();

		SpaceShip SpaceShip;

		std::vector<Obstacle> obstacles;
		std::vector<Obstacle> CreateObstacles();

		std::vector<Alien> aliens;
		std::vector<Alien> CreateAliens();
		void MoveAliens();
		void AlienShootLaser();
		void MoveDownAliens(int discance);
		int AliensDirection;
		std::vector<Laser> alienLasers;
		constexpr static float alienLaserShootInterval = 0.35;
		float timeLastAlienFired;

		Mysteryship mysteryShip;
		float mysteryShipSpawnInterval;
		float timeLastSpawn;

		void CheckForCollisions();

		void GameOver();
		void Reset();
		void InitGame();

		void CheckForHighScore();

		void SaveHighScoreToFile(int highscore);
		int LoadHighScoreFromFile();

		Sound explosionSound;
};