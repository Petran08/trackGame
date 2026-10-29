#pragma once
#include <raylib.h>
#include <raymath.h>
#include<cmath>
#include <vector>
#include <string>

class menu;

class button {
public:

	Texture2D buttonTexture;
	Vector2 position = Vector2Zero();
	Vector2 topLeftCorner = Vector2Zero(), bottomRightCorner = Vector2Zero();
	std::string action, trackName;
	menu* menuToOpen = nullptr;

	void drawButton()
	{
		DrawTexture(buttonTexture, position.x, position.y, WHITE);
	}
};

class menu
{
public:

	Texture2D background;
	std::vector <button> menuButtons;

	void drawMenu()
	{
		DrawTexture(background, 0.0, 0.0, WHITE);

		for (auto btn : menuButtons)
		{
			btn.drawButton();
		}
	}

	bool updateButtonHit(Vector2 mousePos, std::string& action, menu& toOpen, std::string& trackName)
	{
		for (auto btn : menuButtons)
		{
			if (btn.topLeftCorner.x <= mousePos.x && btn.topLeftCorner.y <= mousePos.y && btn.bottomRightCorner.x >= mousePos.x && btn.bottomRightCorner.y >= mousePos.y)
			{
				action = btn.action;
				if (action == "openMenu")
				{
					toOpen = *btn.menuToOpen;
				}
				else if (action == "playTrack" || action == "editTrack")
				{
					trackName = btn.trackName;
				}
				return true;
			}
		}
		return false;
	}
};

