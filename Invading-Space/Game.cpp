#include "Game.hpp"
#include <iostream>
#include<fstream>

Game::Game() {
	music = LoadMusicStream("Sounds/music.ogg");
	PlayMusicStream(music);

	explosionSound = LoadSound("Sounds/explosion.ogg");

	InitGame();
}

Game:: ~Game() {
	Alien::UnloadImages;
	UnloadMusicStream(music);
	UnloadSound(explosionSound);
}

void Game::Update() {
	if (run) 
	{

		double currentTime = GetTime();
		if (currentTime - timeLastSpawn > mysteryShipSpawnInterval)
		{
			mysteryShip.Spawn();
			timeLastSpawn = GetTime();
			mysteryShipSpawnInterval = GetRandomValue(10, 20);
		}

		for (auto& Laser : SpaceShip.lasers)
		{
			Laser.update();
		}

		DeleteInactiveLasers();

		MoveAliens();

		AlienShootLaser();

		for (auto& Laser : alienLasers) {
			Laser.update();
		}

		mysteryShip.Update();

		CheckForCollisions();
	}else 
	{
		if (IsKeyPressed(KEY_ENTER)) {
			Reset();
			InitGame();
		}
	}
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
	for (auto& Laser : alienLasers) {
		Laser.Draw();
	}
	mysteryShip.Draw();
}

void Game::HandleInput() {
	if (run) 
	{
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
	for (auto it = alienLasers.begin(); it != alienLasers.end();)
	{
		if (!it->active)
		{
			it = alienLasers.erase(it);
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
		obstacles.push_back(Obstacle({ offsetX, float(GetScreenHeight() - 200) }));
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

void Game::MoveAliens()
{
	for (auto& alien: aliens)
	{
		if (alien.position.x+alien.alienImages[alien.type-1].width > GetScreenWidth() - 25)
		{
			AliensDirection = -1;
			MoveDownAliens(4);
		}
		else if (alien.position.x < 25) 
		{
			MoveDownAliens(4);
			AliensDirection = 1;
		}
		alien.Update(AliensDirection);
	}
}

void Game::AlienShootLaser()
{
	double currentTime = GetTime();
	if (currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {

		int radnomIndex = GetRandomValue(0, aliens.size() - 1);
		Alien& alien = aliens[radnomIndex];
		alienLasers.push_back(Laser({ alien.position.x + alien.alienImages[alien.type - 1].width / 2,alien.position.y + alien.alienImages[alien.type - 1].height }, 6));

		timeLastAlienFired = GetTime();
	}

	


}

void Game::MoveDownAliens(int discance)
{
	for (auto& alien: aliens)
	{
		alien.position.y += discance;
	}
}

void Game::CheckForCollisions()
{
	//Spaceship Lasers

	for (auto& laser : SpaceShip.lasers) {
		auto it = aliens.begin();
		while (it != aliens.end()) {
			if (CheckCollisionRecs(it -> getRectangle(), laser.getRectangle())) 
			{
				PlaySound(explosionSound);
				if (it -> type == 1) {
					score += 100;
				}
				else if (it->type == 2) {
					score += 200;
				}
				else if (it->type == 3) {
					score += 300;
				}
				CheckForHighScore();

				it = aliens.erase(it);
				laser.active = false;
			}else
			{
				++it;
			}
		}

		//Obstacles

		for (auto& obstacle : obstacles) {
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end()) {
				if (CheckCollisionRecs(it->getRectangle(), laser.getRectangle())) {
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}
				else {
					++it;
				}
			}
		}

		//Mystery Ship

		if (CheckCollisionRecs(mysteryShip.getRectangle(), laser.getRectangle()))
		{
			mysteryShip.alive = false;
			laser.active = false;
			score += 500;
			CheckForHighScore();
			PlaySound(explosionSound);
		}
	}

	//Alien Lasers

	for (auto& laser : alienLasers) {
		if (CheckCollisionRecs(laser.getRectangle(), SpaceShip.getRectangle())) {
			laser.active = false;
			lives--;
			if (lives == 0) {
				GameOver();
			}
		}

		// Obstacles
		
		for (auto& obstacle : obstacles) {
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end()) {
				if (CheckCollisionRecs(it->getRectangle(), laser.getRectangle())) {
					it = obstacle.blocks.erase(it);
					laser.active = false;
				}
				else {
					++it;
				}
			}
		}
	}

	//Alien Collision with Obstacles 

	for (auto& alien : aliens)
	{
		for (auto& obstacle : obstacles)
		{
			auto it = obstacle.blocks.begin();
			while (it != obstacle.blocks.end()) {
				if (CheckCollisionRecs(it -> getRectangle(), alien.getRectangle()))
				{
					it = obstacle.blocks.erase(it);
				}
				else 
				{
					it++;
				}
			}
		}

		if (CheckCollisionRecs(alien.getRectangle(), SpaceShip.getRectangle())) {
			GameOver();
		}

	}
}

void Game::GameOver()
{
	run = false;
}

void Game::Reset()
{
	SpaceShip.Reset();
	aliens.clear();
	alienLasers.clear();
	obstacles.clear();
}

void Game::InitGame()
{
	obstacles = CreateObstacles();
	aliens = CreateAliens();
	AliensDirection = 1;
	timeLastAlienFired = 0.0;
	timeLastSpawn = 0.0;
	mysteryShipSpawnInterval = GetRandomValue(10, 20);
	score = 0;
	lives = 3;
	run = true;
	highScore = LoadHighScoreFromFile();
}

void Game::CheckForHighScore()
{
	if (score > highScore) {
		highScore = score;
		SaveHighScoreToFile(highScore);
	}
}

void Game::SaveHighScoreToFile(int highscore)
{
	std::ofstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open()) {
		highscoreFile << highscore;
		highscoreFile.close();
	}
	else 
	{
		std::cerr << "Failed to save highscore to file" << std::endl;
	}
}

int Game::LoadHighScoreFromFile()
{
	int loadedHighScore = 0;
	std::ifstream highscoreFile("highscore.txt");
	if (highscoreFile.is_open())
	{
		highscoreFile >> loadedHighScore;
		highscoreFile.close();
	}
	else 
	{
		std::cerr << "Failed to load highscore from file" << std::endl;
	}
	return loadedHighScore;
}
