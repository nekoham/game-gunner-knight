#pragma once
#include <SFML/Graphics.hpp>

#define max_menu_choice 4

using namespace sf;
using namespace std;

class Menu
{
public:
	Menu(float width, float height);
	~Menu();

	void draw(RenderWindow& window);
	int GetPressedItem() { return selectedChoice; }
	int selectedChoice;
	Font fontMenu;
	Text menu[4];

	Texture menuBG_Tex;
	Sprite menuBG;
};
