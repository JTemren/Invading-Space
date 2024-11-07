// Invading-Space.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <raylib.h>
#include "Game.hpp"
#include <string>

std::string FormatWithLeadingZeroes(int number, int Width) {
    std::string numberText = std::to_string(number);
    int leadingZeros = Width - numberText.length();
    return numberText = std::string(leadingZeros, '0') + numberText;
}

int main()
{
    Color grey = { 29,29,27,255 };
    Color yellow = { 243,216,63,255 };
    int offset = 50;
    int windowWWidth = 750;
    int windowHight = 700;

    InitWindow(windowWWidth + offset, windowHight + 2 * offset, "Ivading Space C++");
    InitAudioDevice();
    
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");



    SetTargetFPS(60);


    Game game;

    while (WindowShouldClose() == false) {

        game.HandleInput();
        UpdateMusicStream(game.music);


        game.Update();
        BeginDrawing();
        ClearBackground(grey);
        DrawRectangleRoundedLines({ 10,10,780,780 }, 0.18f, 20, 2, yellow);

        DrawLineEx({ 25,730 }, { 775,730 }, 3, yellow);
        if (game.run) {
            DrawTextEx(font, "Level 01", { 570,740 }, 34, 2, yellow);

        }
        else {
            DrawTextEx(font, "GAME OVER", { 570,740 }, 34, 2, yellow);
        }
        float x = 50;
        for (int i = 0; i < game.lives; i++)
        {
            DrawTextureV(spaceshipImage, { x,745 }, WHITE);
            x += 50;
        }
        DrawTextEx(font, "SCORE", { 50,15 }, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZeroes(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), { 50,40 }, 34, 2, yellow);

        DrawTextEx(font, "HIGH-SCORE", { 570,15 }, 34, 2, yellow);
        std::string highScoreText = FormatWithLeadingZeroes(game.highScore, 5);
        DrawTextEx(font, highScoreText.c_str(), { 655,40 }, 34, 2, yellow);


        game.Draw();

        EndDrawing();
    }

    CloseWindow;
    CloseAudioDevice();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
