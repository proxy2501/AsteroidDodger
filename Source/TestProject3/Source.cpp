#include <iostream>
#include <Windows.h>
#include <irrKlang.h>
#include <thread>
#include <vector>
#include <string>
#include "Player.h"
#include "EasyEnemy.h"
#include "MediumEnemy.h"

using namespace std;
using namespace irrklang;

int nScreenWidth = 50; // Width (columns) of console window.
int nScreenHeight = 40; // Height (rows) of console window.

int main()
{
	// Console setup (black magic).
	wchar_t* screen = new wchar_t[nScreenWidth*nScreenHeight];
	for (int i = 0; i < nScreenWidth*nScreenHeight; i++)
		screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SMALL_RECT rectWindow = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	COORD coord = { (short)nScreenWidth, (short)nScreenHeight };
	SetConsoleScreenBufferSize(hConsole, coord);
	SetConsoleActiveScreenBuffer(hConsole);
	rectWindow = { 0, 0, (short)nScreenWidth - 1, (short)nScreenHeight - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);
	DWORD dwBytesWritten = 0;
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// Sound.
	ISoundEngine* soundEngine = createIrrKlangDevice();
	soundEngine->setSoundVolume(0.3f);

	// Input.
	bool bKeyLeft = false;
	bool bKeyRight = false;
	bool bKeyEnter = false;

	// Game variables.
	float elapsedTime;
	int enemySpawnRate;
	Player player(nScreenWidth / 2, nScreenHeight - 2);
	vector<Enemy*> enemies;

	// Program loop.
	while (true)
	{
		elapsedTime = 0;
		enemySpawnRate = 20; // Initial chance to spawn: 20%

		player.SetIsAlive(true);
		player.SetX(nScreenWidth / 2);
		player.SetY(nScreenHeight - 2);

		soundEngine->play2D("theme.mp3", true); // Start playing theme.

		// Game loop.
		while (player.GetIsAlive())
		{
			//////////////////////////
			// ---Timing & Input--- //
			//////////////////////////

			this_thread::sleep_for(90ms); // 1 frame = 90 milliseconds.

			bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0; // Left arrow key detection.
			bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0; // Right arrow key detection.



			//////////////////////
			// ---Game Logic--- //
			//////////////////////

			// Spawn enemies at top of screen according to their spawn rate.
			if ((rand() % 100) + 1 < enemySpawnRate)
			{
				switch (rand() % 2) // TODO: Add more enemy types here.
				{
				case 0:
					enemies.push_back(new EasyEnemy(rand() % (nScreenWidth - 1), 2));
					break;
				case 1:
					enemies.push_back(new MediumEnemy(rand() % (nScreenWidth - 1), 2));
					break;
				default:
					break;
				}
			}

			// Move player according to input.
			if (bKeyLeft && !bKeyRight) // Player move left.
			{
				if (player.GetX() != 1)
				{
					player.SetX(player.GetX() - 1);
				}
			}
			else if (bKeyRight && !bKeyLeft) // Player move right.
			{
				if (player.GetX() != nScreenWidth - 2)
				{
					player.SetX(player.GetX() + 1);
				}
			}

			// Move enemies, detect collision with player. Delete if off-screen.
			for (vector<Enemy*>::iterator it = enemies.begin(); it < enemies.end();)
			{
				// Move enemy.
				(*it)->Move();

				// Player collision.
				if (player.GetX() == (*it)->GetX() && player.GetY() == (*it)->GetY())
				{
					player.SetIsAlive(false);
					soundEngine->stopAllSounds();
					soundEngine->play2D("explosion.wav");
				}

				// Off screen.
				if ((*it)->GetY() >= nScreenHeight)
				{
					delete (*it);
					it = enemies.erase(it);
				}
				else
				{
					++it;
				}
			}

			// Update game time.
			elapsedTime += 0.1f; // Theoretically a frame is 0.09, but under runtime is closer to 0.1

			// Increase spawn rate at 10 second intervals.
			if (enemySpawnRate < 100)
			{
				enemySpawnRate = 20 + (int)(elapsedTime / 10) * 10;
			}



			///////////////////
			// ---Display--- //
			///////////////////

			// Clear screen.
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
				screen[i] = L' ';

			// Draw UI
			for (int i = 0; i < nScreenWidth; i++)
				screen[i] = L'=';
			for (int i = 0; i < nScreenWidth; i++)
				screen[2 * nScreenWidth + i] = L'=';
			wsprintf(&screen[nScreenWidth + 16], L"EVADE THE ENEMIES!");

			// Draw player.
			screen[nScreenWidth * player.GetY() + player.GetX()] = L'A';

			// Draw Enemies.
			for (vector<Enemy*>::iterator e = enemies.begin(); e < enemies.end(); e++)
			{
				screen[nScreenWidth * (*e)->GetY() + (*e)->GetX()] = L'¤';
			}

			// Write contents of buffer to console.
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

		} // Game loop end.

		// Clean up.
		for (vector<Enemy*>::iterator it = enemies.begin(); it < enemies.end();)
		{
			delete (*it);
			it = enemies.erase(it);
		}

		// Display end-game screen.
		for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
			screen[i] = L' ';
		wsprintf(&screen[nScreenWidth * (nScreenHeight / 2 - 5) + 21], L"YOU DIED");
		wsprintf(&screen[nScreenWidth * (nScreenHeight / 2 - 3) + 15], L"SECONDS SURVIVED: %d", (int)elapsedTime);
		wsprintf(&screen[nScreenWidth * (nScreenHeight / 2) + 11], L"<press 'ENTER' to try again>");
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten);

		// Await enter key pressed.
		bKeyEnter = false;
		while (!bKeyEnter)
		{
			bKeyEnter = (0x8000 & GetAsyncKeyState((unsigned char)('\x0D'))) != 0; // Enter key detection.
		}

	} // Program loop end.

	return 0;
}