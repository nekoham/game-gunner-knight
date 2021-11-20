#include "Menu.h"

Menu::Menu(float width, float height)
{
	if (!fontMenu.loadFromFile("Font/yoster.ttf"))
	{
		printf("FAILED TO LOAD FONT");
	}

	

}


Menu::~Menu()
{
}

void Menu::draw(RenderWindow& window)
{
	window.draw(menuBG);

	for (int i = 0; i < max_menu_choice; i++)
	{
		window.draw(menu[i]);
	}
}
