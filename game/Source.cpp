#define _CRT_SECURE_NO_WARNINGS 1
#define MAX_INDEX_LEADERBOARD 5

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include "Menu.h"
#include <math.h>
#include <vector>
#include <string.h>
#include <stdio.h>

int main()
{
	//Declear variables
	srand(time(NULL));
	int shootTimer = 0;
	int enemyTimer = 0;
	int enemyCounter = 0;
	int itemTimer = 0;
	int eventTimer = 0;
	int bulletTimer = 20;
	int lightingState = 0;
	int bulletDMG = 2;
	int bulletStatus = 0;
	int gameStatus = 0;
	int dodgeStatus = 0;
	int dodgeTimer = 300;
	float pie = 3.14159265359;
	bool gameSave = false;
	Vector2f position;

	//Window
	RenderWindow window(VideoMode(1920, 1080), "Gunner Knight", Style::Fullscreen);
	window.setFramerateLimit(60);

	//Font
	Font fontIG;

	if (!fontIG.loadFromFile("Font/yoster.ttf"))
	{
		printf("FAILED TO LOAD FONT\n");
	}

	//Background In game
	Texture backgroundTex;
	Sprite background;
	Sprite background2;

	backgroundTex.loadFromFile("Texture/background1.png");
	background.setTexture(backgroundTex);
	background.setScale(1.f, 1.f);

	//Menu
	Text menu[4];

	Texture menuBG_Tex;
	Sprite menuBG[2];

	menuBG_Tex.loadFromFile("Texture/menuBG.png");

	for (int i = 0; i < 2; i++)
	{
		menuBG[i].setTexture(menuBG_Tex);
		menuBG[i].setScale(1.f, 1.f);
	}

	menuBG[0].setPosition(0,0);
	menuBG[1].setPosition(0-menuBG[0].getGlobalBounds().width,0);

	menu[0].setString("Gunner Knight");
	menu[1].setString("Play");
	menu[2].setString("Scoreboard");
	menu[3].setString("Exit");

	for (int i = 0; i < max_menu_choice; i++)
	{
		menu[i].setFont(fontIG);
		menu[i].setOutlineThickness(7.f);
		menu[i].setOutlineColor(Color::Black);
		menu[i].setPosition(Vector2f((window.getSize().x / 2) / 2 - (menu[i].getGlobalBounds().width / 2), window.getSize().x / 2 / (max_menu_choice + 1) * (i + 1)));
	}
	menu[0].setCharacterSize(120);
	menu[0].setFillColor(Color(255,255,215));
	menu[0].setPosition(Vector2f((window.getSize().x / 2) - menu[0].getGlobalBounds().width / 2, window.getSize().y / (max_menu_choice + 1) * 0.7));

	//Score
	FILE* fp;

	struct saveData {
		char name[11];
		int score;
	};
	saveData topScore[5];
	saveData tempScore;
	saveData currentScore;
	currentScore.score = 0;

	string outputName[5];

	//Leaderboard
	RectangleShape leaderboardBehide;
	Texture crownTex;
	Sprite crown;
	Text leaderboard[7];
	leaderboard[0].setString("Leaderboard");
	leaderboard[6].setString("Go back");

	for (int i = 0; i < 7; i++)
	{
		leaderboard[i].setFont(fontIG);
		leaderboard[i].setFillColor(Color::White);
		leaderboard[i].setOutlineThickness(7.f);
		leaderboard[i].setOutlineColor(Color::Black);
		leaderboard[i].setCharacterSize(80);
		leaderboard[i].setPosition(Vector2f((window.getSize().x / 2) - 530, window.getSize().y / (8) * (i+1)));
	}

	leaderboard[0].setCharacterSize(120);
	leaderboard[0].setFillColor(Color(255, 255, 215));
	leaderboard[0].setPosition(Vector2f((window.getSize().x / 2) - leaderboard[0].getGlobalBounds().width / 2, window.getSize().y -1000));

	leaderboard[1].setFillColor(Color(255, 255, 153));
	leaderboard[2].setFillColor(Color(190, 194, 203));
	leaderboard[3].setFillColor(Color(205, 127, 50));

	crownTex.loadFromFile("Texture/1st_nobg.png");
	crown.setTexture(crownTex);
	crown.setScale(0.15f, 0.15f);
	crown.setPosition(Vector2f((window.getSize().x / 2) - 640, window.getSize().y / (8)*2+10));

	leaderboardBehide.setFillColor(Color(255, 255, 215,100));
	leaderboardBehide.setOutlineThickness(5.f);
	leaderboardBehide.setOutlineColor(Color(255, 255, 170));
	leaderboardBehide.setSize(Vector2f(2000, 700));
	leaderboardBehide.setOrigin(leaderboardBehide.getSize().x / 2, leaderboardBehide.getSize().y / 2);
	leaderboardBehide.setPosition(Vector2f(window.getSize().x/2, window.getSize().y/2+50));

	//Game Over
	bool gameOverReady = false;
	int gameOverNameCount = 0;
	Text gameOverText;
	gameOverText.setFont(fontIG);
	gameOverText.setFillColor(Color::Red);
	gameOverText.setCharacterSize(200);
	gameOverText.setString("GAME OVER");
	gameOverText.setOrigin(Vector2f(gameOverText.getGlobalBounds().width/2, gameOverText.getGlobalBounds().height/2));
	gameOverText.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2 - 300));

	RectangleShape gameOverScreen;
	gameOverScreen.setFillColor(Color::Black);
	gameOverScreen.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	String gameOverInput;
	Text gameOverOutput[4]; //[0] = score [1] = name [2] = continue [3] = nameInput

	for (int i = 0; i < 4; i++)
	{
		gameOverOutput[i].setFont(fontIG);
		gameOverOutput[i].setFillColor(Color::White);
		gameOverOutput[i].setCharacterSize(80);
		gameOverOutput[i].setPosition(Vector2f(window.getSize().x / 2 - 600, window.getSize().y /2 + 150*i));
	}

	gameOverOutput[2].setString("Confirm");
	gameOverOutput[2].setPosition(Vector2f(window.getSize().x / 2 - gameOverOutput[2].getGlobalBounds().width/2, window.getSize().y / 2 + 350));
	gameOverOutput[3].setPosition((Vector2f(window.getSize().x / 2 + 6, window.getSize().y / 2 + 150)));

	//Game Victory

	//Music
	Music menuMusic;
	if (!menuMusic.openFromFile("Sound/gameMenu.wav"))
	{
		printf("FAILED TO LOAD MUSIC\n");
	}
	menuMusic.setVolume(20);
	menuMusic.play();

	//Sound Effect
	SoundBuffer gunBuffer;
	Sound gunSound;
	if (!gunBuffer.loadFromFile("Sound/gunShot.wav"))
	{
		printf("COULD NOT LOAD GUNSHOT SOUND");
	}
	gunSound.setBuffer(gunBuffer);

	SoundBuffer gameOverBuffer;
	Sound gameOverSound;
	if (!gameOverBuffer.loadFromFile("Sound/gameOver.wav"))
	{
		printf("COULD NOT LOAD GAMEOVER SOUND");
	}
	gameOverSound.setBuffer(gameOverBuffer);

	SoundBuffer interfaceClickBuffer;
	Sound interfaceClickSound;
	if (!interfaceClickBuffer.loadFromFile("Sound/interfaceClick1.wav"))
	{
		printf("COULD NOT LOAD INTERFACE CLICK SOUND");
	}
	interfaceClickSound.setBuffer(interfaceClickBuffer);

	SoundBuffer interfaceClickBuffer2;
	Sound interfaceClickSound2;
	if (!interfaceClickBuffer2.loadFromFile("Sound/interfaceClick2.wav"))
	{
		printf("COULD NOT LOAD INTERFACE CLICK SOUND2");
	}
	interfaceClickSound2.setBuffer(interfaceClickBuffer2);

	SoundBuffer knightAtkedBuffter;
	Sound knightAtkedSound;
	if (!knightAtkedBuffter.loadFromFile("Sound/knightAtked.wav"))
	{
		printf("COULD NOT LOAD KNIGHT ATTACKED SOUND");
	}
	knightAtkedSound.setBuffer(knightAtkedBuffter);

	SoundBuffer enemyAtkedBuffter;
	Sound enemyAtkedSound;
	if (!enemyAtkedBuffter.loadFromFile("Sound/enemyAtked.wav"))
	{
		printf("COULD NOT LOAD ENEMY ATTACKED SOUND");
	}
	enemyAtkedSound.setBuffer(enemyAtkedBuffter);

	SoundBuffer enemyDiedBuffer;
	Sound enemyDiedSound;
	if (!enemyDiedBuffer.loadFromFile("Sound/enemyDie.wav"))
	{
		printf("COULD NOT LOAD ENEMY DIE SOUND");
	}
	enemyDiedSound.setBuffer(enemyDiedBuffer);

	SoundBuffer dashBuffer;
	Sound dashSound;
	if (!dashBuffer.loadFromFile("Sound/dash.wav"))
	{
		printf("COULD NOT LOAD  DASH SOUND");
	}
	dashSound.setBuffer(dashBuffer);

	SoundBuffer itemCollectBuffer;
	Sound itemCollectSound;
	if (!itemCollectBuffer.loadFromFile("Sound/itemCollect.wav"))
	{
		printf("COULD NOT LOAD ITEM COLLECT SOUND");
	}
	itemCollectSound.setBuffer(itemCollectBuffer);

	gunSound.setVolume(10);
	interfaceClickSound.setVolume(40);
	interfaceClickSound2.setVolume(40);
	enemyDiedSound.setVolume(20);
	enemyAtkedSound.setVolume(10);
	itemCollectSound.setVolume(40);

	//Knight
	Texture knightTex;
	Sprite knight;
	int knightHp = 1;
	int knightArmor = 5;
	float knightSpeedNormal[2] = { 8.f, -8.f };
	float knightSpeedBoost[2] = { 16.f, -16.f };
	float knightSpeedCurrent[2] = { 8.f, -8.f };

	knightTex.loadFromFile("Texture/knight.png");
	knight.setTexture(knightTex);
	knight.setScale(Vector2f(0.4f, 0.4f));
	knight.setOrigin(Vector2f(knight.getTexture()->getSize().x / 2, knight.getTexture()->getSize().y / 2));
	knight.setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	//Enemy
	struct Enemy
	{
		Texture enemyTex;
		Sprite enemy;
		Vector2f enemyVelo;
		Vector2f enemyCenter;
		Vector2f enemyDir;
		Vector2f enemyDirNorm;
		Vector2f enemyPos;
		int hp = 5;
		float rotation_enemy;
	};

	Enemy badguy;
	badguy.enemyTex.loadFromFile("Texture/enemy.png");
	badguy.enemy.setTexture(badguy.enemyTex);
	badguy.enemy.setScale(Vector2f(0.4f, 0.4f));
	badguy.enemy.setOrigin(Vector2f(badguy.enemy.getTexture()->getSize().x / 2, badguy.enemy.getTexture()->getSize().y / 2));

	Enemy bigguy;
	bigguy.enemyTex.loadFromFile("Texture/enemyType2.png");
	bigguy.enemy.setTexture(bigguy.enemyTex);
	bigguy.enemy.setScale(Vector2f(0.6f, 0.6f));
	bigguy.enemy.setOrigin(Vector2f(badguy.enemy.getTexture()->getSize().x / 2, badguy.enemy.getTexture()->getSize().y / 2));

	vector<Enemy>enemies; // walk toward the knight
	vector<Enemy>enemies2; // walk around the map, not toward the knight

	//Bullet
	struct Bullet
	{
		CircleShape shape;
		Vector2f bulletVelo;
	};

	Bullet projectile;
	projectile.shape.setRadius(10.f);
	projectile.shape.setFillColor(Color(255, 215, 0));
	projectile.shape.setOutlineThickness(3.f);
	projectile.shape.setOutlineColor(Color::Black);
	projectile.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	vector<Bullet>bullets;

	Bullet projectileDirection_1;
	projectileDirection_1.shape.setRadius(10.f);
	projectileDirection_1.shape.setFillColor(Color(255, 215, 0));
	projectileDirection_1.shape.setOutlineThickness(3.f);
	projectileDirection_1.shape.setOutlineColor(Color::Black);
	projectileDirection_1.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	Bullet projectileDirection_2;
	projectileDirection_2.shape.setRadius(10.f);
	projectileDirection_2.shape.setFillColor(Color(255, 215, 0));
	projectileDirection_2.shape.setOutlineThickness(3.f);
	projectileDirection_2.shape.setOutlineColor(Color::Black);
	projectileDirection_2.shape.setOrigin(Vector2f(projectile.shape.getRadius() / 2, projectile.shape.getRadius() / 2));

	//HP Bar
	RectangleShape HpBar;
	HpBar.setFillColor(Color::Red);
	HpBar.setOutlineThickness(10);
	HpBar.setOutlineColor(Color::Black);
	HpBar.setPosition(Vector2f(1050, 800));

	//Armor Bar
	RectangleShape ArmorBar;
	ArmorBar.setFillColor(Color(192, 192, 192));
	ArmorBar.setOutlineThickness(10);
	ArmorBar.setOutlineColor(Color::Black);
	ArmorBar.setPosition(Vector2f(1050, 700));

	//Dodge Bar
	RectangleShape DodgeBar;
	DodgeBar.setFillColor(Color::Yellow);
	DodgeBar.setOutlineThickness(10);
	DodgeBar.setOutlineColor(Color::Black);
	DodgeBar.setSize(Vector2f(100.f, 100.f));
	DodgeBar.setPosition(Vector2f(900, 750));

	//HP Potion
	CircleShape hpPotion;
	hpPotion.setRadius(20);
	hpPotion.setOutlineThickness(3);
	hpPotion.setOutlineColor(Color::Black);
	hpPotion.setFillColor(Color::Red);
	int hpPlus = 2;

	vector<CircleShape>lifeExtenders;

	//Armor Potion
	CircleShape armorPotion;
	armorPotion.setRadius(20);
	armorPotion.setOutlineThickness(3);
	armorPotion.setOutlineColor(Color::Black);
	armorPotion.setFillColor(Color(192, 192, 192));
	int armorPlus = 1;

	vector<CircleShape>armorExtenders;

	RectangleShape speedBullet; //Speed
	speedBullet.setSize(Vector2f(10.f, 10.f));
	speedBullet.setFillColor(Color::Cyan);
	speedBullet.setPosition(Vector2f(300.f, 100.f));

	RectangleShape sizeBullet; //Speed
	sizeBullet.setSize(Vector2f(10.f, 10.f));
	sizeBullet.setFillColor(Color::Red);
	sizeBullet.setPosition(Vector2f(300.f, 100.f));

	vector<RectangleShape>speedBuff;
	vector<RectangleShape>sizeBuff;

	//Score
	string output;

	Text scoreIG;
	scoreIG.setFont(fontIG);
	scoreIG.setCharacterSize(62);
	scoreIG.setString(output);

	//Lighting
	CircleShape preLighting;
	preLighting.setRadius(200);
	preLighting.setFillColor(Color(255,0,0, 80));
	preLighting.setOrigin(Vector2f(preLighting.getRadius() , preLighting.getRadius() ));

	CircleShape countdownLighting;
	countdownLighting.setRadius(0);
	countdownLighting.setFillColor(Color(255,0,0, 150));
	countdownLighting.setOrigin(Vector2f(countdownLighting.getRadius() , countdownLighting.getRadius() ));

	//Program start running
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;

			case Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case Mouse::Left:
					if (gameStatus == 0)
					{
						if (menu[1].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
						{
							interfaceClickSound.play();
							knightHp = 10;
							currentScore.score = 0;
							gameStatus = 1;
						}
						if (menu[2].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
						{
							interfaceClickSound.play();
							gameStatus = 2;
						}
						if (menu[3].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
						{
							interfaceClickSound.play();
							window.close();
						}
					}
					if (gameStatus == 2)
					{
						if (leaderboard[6].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
						{
							interfaceClickSound2.play();
							gameStatus = 0;
						}
					}
					if (gameStatus == 3)
					{
						if (gameOverOutput[2].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
						{
							interfaceClickSound2.play();
							gameStatus = 0;
						}
					}
				default:
					break;
				}
				break;
			case Event::MouseButtonPressed:
				switch (event.mouseButton.button)
				{
				case Mouse::Right:
					if (dodgeStatus == 0 && dodgeTimer >= 300)
					{
						dodgeStatus = 1;
						dodgeTimer = 0;

						dashSound.setPlayingOffset(seconds(1.f));
						dashSound.play();
					}
				default:
					break;
				}
				break;
			case Event::TextEntered:
			{
				if (gameStatus == 3)
				{
					if (event.text.unicode != 0x00000008 && gameOverNameCount < 10)
					{
						gameOverInput += event.text.unicode;
						gameOverOutput[3].setString(gameOverInput);
						gameOverNameCount += 1;
					}
					else if (event.text.unicode == 0x00000008)
					{
						if (gameOverInput.getSize() != 0)
						{
							gameOverInput.erase(gameOverInput.getSize() - 1, 1);
							gameOverNameCount -= 1;
						}
						gameOverOutput[3].setString(gameOverInput);
					}
				}
				break;
			}
			default:
				break;
			}
		}
		printf("%d\n", gameOverNameCount);
		//Menu
		if (gameStatus == 0)
		{
			//Music on if it's off
			if (menuMusic.getStatus() == Sound::Status::Stopped)
			{
				menuMusic.play();
			}

			//Menu button color changing
			for (int i = 1; i < max_menu_choice; i++)
			{
				if (menu[i].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
				{
					menu[i].setFillColor(Color::Red);
					menu[i].setCharacterSize(85);
					menu[i].setPosition(Vector2f((window.getSize().x / 2) - menu[i].getGlobalBounds().width / 2, window.getSize().y / (max_menu_choice + 1) * (i + 1)));
				}
				else 
				{ 
					menu[i].setFillColor(Color::White); 
					menu[i].setCharacterSize(80);
					menu[i].setPosition(Vector2f((window.getSize().x / 2) - menu[i].getGlobalBounds().width / 2, window.getSize().y / (max_menu_choice + 1) * (i + 1)));
				}
			}
		}

		//Game
		else if (gameStatus == 1)
		{
			//Stop playing menu music
			menuMusic.stop();
			
			//Update knight -> Direction and rotating
			Vector2f knightCenter = Vector2f(knight.getPosition().x - 10, knight.getPosition().y - 10);
			Vector2f mousePos = Vector2f(Mouse::getPosition(window));
			Vector2f aimDir = mousePos - knightCenter;
			Vector2f aimDirNorm = aimDir / float(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

			float dx_knight = mousePos.x - knight.getPosition().x;
			float dy_knight = mousePos.y - knight.getPosition().y;
			float rotation_knight = (atan2(dy_knight, dx_knight)) * 180 / pie;
			knight.setRotation(rotation_knight + 90);

			//Update control
			if (Keyboard::isKeyPressed(Keyboard::A) && knight.getPosition().x - +knight.getGlobalBounds().width / 2 > 0)
			{
				knight.move(knightSpeedCurrent[1], 0.0f);
			}
			if (Keyboard::isKeyPressed(Keyboard::D) && knight.getPosition().x + knight.getGlobalBounds().width / 2 < window.getSize().x)
			{
				knight.move(knightSpeedCurrent[0], 0.0f);
			}
			if (Keyboard::isKeyPressed(Keyboard::W) && knight.getPosition().y - knight.getGlobalBounds().height / 2 > 0)
			{
				knight.move(0.0f, knightSpeedCurrent[1]);
			}
			if (Keyboard::isKeyPressed(Keyboard::S) && knight.getPosition().y + knight.getGlobalBounds().height / 2 < window.getSize().y)
			{
				knight.move(0.0f, knightSpeedCurrent[0]);
			}

			//Update dodge
			if (dodgeTimer < 300)
			{
				DodgeBar.setFillColor(Color(155,135,12));
				dodgeTimer += 1;
			}
			else
			{
				DodgeBar.setFillColor(Color::Yellow);
			}

			if (dodgeStatus == 1 && dodgeTimer <= 25)
			{
				knightSpeedCurrent[0] = knightSpeedBoost[0];
				knightSpeedCurrent[1] = knightSpeedBoost[1];
				knight.setColor(Color(255,255,255,128));
			}
			else if (dodgeStatus == 1 && dodgeTimer > 25)
			{
				knightSpeedCurrent[0] = knightSpeedNormal[0];
				knightSpeedCurrent[1] = knightSpeedNormal[1];
				knight.setColor(Color::White);
				dodgeStatus = 0;
			}

			//Update enemy
			if (enemyTimer < 20 && enemyCounter < 10)
			{
				enemyTimer += 1;
			}

			if (enemyTimer >= 20) // spawn enemy
			{
				badguy.enemy.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
				enemies.push_back(Enemy(badguy));

				badguy.enemy.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
				badguy.enemyPos = Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
				enemies2.push_back(Enemy(badguy));
			
				enemyTimer = 0;
				enemyCounter += 2;
			}

			for (size_t j = 0; j < enemies.size(); j++) //enemies1
			{
				if (enemies[j].hp <= 0) // enemy delete if dies
				{
					enemies.erase(enemies.begin() + j);
					enemyCounter -= 1;
					currentScore.score += 5;
					enemyDiedSound.play();
					break;
				}

				if (enemies[j].enemy.getGlobalBounds().intersects(knight.getGlobalBounds()) && dodgeStatus != 1) // enemy delete if attack(reach) knight
				{
					enemies.erase(enemies.begin() + j);
					if (knightArmor > 0) { knightArmor -= 1; }
					else { knightHp -= 1; }
					enemyCounter -= 1;
					knightAtkedSound.setPlayingOffset(seconds(0.5f));
					enemyDiedSound.play();
					knightAtkedSound.play();
					break;
				}

				// rotating enemy
				float dx_enemy = knight.getPosition().x - enemies[j].enemy.getPosition().x;
				float dy_enemy = knight.getPosition().y - enemies[j].enemy.getPosition().y;
				enemies[j].rotation_enemy = (atan2(dy_enemy, dx_enemy)) * 180 / pie;
				enemies[j].enemy.setRotation(enemies[j].rotation_enemy + 90);

				//moving enemy
				enemies[j].enemyCenter = Vector2f(enemies[j].enemy.getPosition().x - 10, enemies[j].enemy.getPosition().y - 10);
				enemies[j].enemyDir = knightCenter - enemies[j].enemyCenter;
				enemies[j].enemyDirNorm = enemies[j].enemyDir / float(sqrt(pow(enemies[j].enemyDir.x, 2) + pow(enemies[j].enemyDir.y, 2)));
				enemies[j].enemyVelo = enemies[j].enemyDirNorm * 2.f;
				enemies[j].enemy.move(enemies[j].enemyVelo);
			}

			for (size_t j = 0; j < enemies2.size(); j++) //enemies2
			{
				if (enemies2[j].hp <= 0) // enemy2 delete if dies
				{
					enemies2.erase(enemies2.begin() + j);
					enemyCounter -= 1;
					currentScore.score += 5;
					enemyDiedSound.play();
					break;
				}

				if (enemies2[j].enemy.getGlobalBounds().intersects(knight.getGlobalBounds()) && dodgeStatus != 1) // enemy2 delete if attack(reach) knight
				{
					enemies2.erase(enemies2.begin() + j);
					if (knightArmor > 0) { knightArmor -= 1; }
					else { knightHp -= 1; }
					enemyCounter -= 1;
					knightAtkedSound.setPlayingOffset(seconds(0.5f));
					enemyDiedSound.play();
					knightAtkedSound.play();
					break;
				}

				//rotating enemy2				
				float dx_enemy = enemies2[j].enemyPos.x - enemies2[j].enemy.getPosition().x;
				float dy_enemy = enemies2[j].enemyPos.y - enemies2[j].enemy.getPosition().y;
				enemies2[j].rotation_enemy = (atan2(dy_enemy, dx_enemy)) * 180 / pie;
				enemies2[j].enemy.setRotation(enemies2[j].rotation_enemy + 90);

				//moving enemy2
				enemies2[j].enemyCenter = Vector2f(enemies2[j].enemy.getPosition().x - 10, enemies2[j].enemy.getPosition().y - 10);
				enemies2[j].enemyDir = enemies2[j].enemyPos - enemies2[j].enemyCenter;
				enemies2[j].enemyDirNorm = enemies2[j].enemyDir / float(sqrt(pow(enemies2[j].enemyDir.x, 2) + pow(enemies2[j].enemyDir.y, 2)));
				enemies2[j].enemyVelo = enemies2[j].enemyDirNorm * 2.f;
				enemies2[j].enemy.move(enemies2[j].enemyVelo);

				if (enemies2[j].enemy.getGlobalBounds().contains(enemies2[j].enemyPos))
				{
					enemies2[j].enemyPos = Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
				}
			}

			//Update shooting
			if (shootTimer < bulletTimer)
			{
				shootTimer += 1;
			}

			if (Mouse::isButtonPressed(Mouse::Left) && shootTimer >= bulletTimer) // spawn bullet
			{
				projectile.shape.setPosition(knightCenter);
				projectile.bulletVelo = aimDirNorm * 20.f;
				bullets.push_back(Bullet(projectile));
				shootTimer = 0;

				gunSound.play();
			}

			for (size_t i = 0; i < bullets.size(); i++)  //Collision and movement bullet 
			{
				bullets[i].shape.move(bullets[i].bulletVelo);

				if (bullets[i].shape.getPosition().y + 500 < 0 || bullets[i].shape.getPosition().y - 500 > window.getSize().y - bullets[i].shape.getRadius() || bullets[i].shape.getPosition().x + 500 < 0 || bullets[i].shape.getPosition().x - 500 > window.getSize().x - bullets[i].shape.getRadius())
				{
					bullets.erase(bullets.begin() + i);
					break;
				}

				for (size_t j = 0; j < enemies.size(); j++) //Collision for enemies
				{
					if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()))
					{
						enemies[j].hp -= bulletDMG;
						bullets.erase(bullets.begin() + i);

						enemyAtkedSound.play();
						break;
					}
				}
			}

			for (size_t i = 0; i < bullets.size(); i++)  //Collision for enemies2
			{
				bullets[i].shape.move(bullets[i].bulletVelo);
				for (size_t j = 0; j < enemies2.size(); j++)
				{
					if (bullets[i].shape.getGlobalBounds().intersects(enemies2[j].enemy.getGlobalBounds()))
					{
						enemies2[j].hp -= bulletDMG;
						bullets.erase(bullets.begin() + i);

						enemyAtkedSound.play();
						break;
					}
				}
			}

			//Update Hp Armor Bar
			HpBar.setSize(Vector2f(50 * knightHp, 50));
			ArmorBar.setSize(Vector2f(50 * knightArmor, 50));

			//Bullet Status
			switch (bulletStatus)
			{
			case 0: //normal
				bulletTimer = 20;
				break;
			case 1: //speed buff
				bulletTimer = 8;
				break;
			case 2: //dmg size buff
				projectile.shape.setRadius(15.f);
				projectile.shape.setFillColor(Color(160, 108, 56));
				bulletDMG = 3;
				break;
			default:
				bulletTimer = 20;
				break;
			}

			//Update items
			if (itemTimer < 200)
			{
				itemTimer += 1;
			}

			if (itemTimer >= 200)
			{
				int itemChoice = rand() % 4; // 0 = hp pot spawn. 1 = armor pot spawn.
				switch (itemChoice)
				{
				case 0:
					hpPotion.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					lifeExtenders.push_back(CircleShape(hpPotion));
					itemTimer = 0;
					break;

				case 1:
					armorPotion.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					armorExtenders.push_back(CircleShape(armorPotion));
					itemTimer = 0;
					break;
				case 2:
					speedBullet.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					speedBuff.push_back(RectangleShape(speedBullet));
					itemTimer = 0;
					break;
				case 3:
					sizeBullet.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					sizeBuff.push_back(RectangleShape(sizeBullet));
					itemTimer = 0;
					break;
				default:
					break;
				}
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				if (lifeExtenders[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					if (knightHp + hpPlus > 10) { knightHp = 10; }
					else { knightHp += hpPlus; }
					lifeExtenders.erase(lifeExtenders.begin() + i);
					itemCollectSound.play();
					break;
				}
			}

			for (size_t i = 0; i < armorExtenders.size(); i++)
			{
				if (armorExtenders[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					if (knightArmor + armorPlus > 10) { knightArmor = 10; }
					else { knightArmor += armorPlus; }
					armorExtenders.erase(armorExtenders.begin() + i);
					itemCollectSound.play();
					break;
				}
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				if (speedBuff[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					bulletStatus = 1;
					speedBuff.erase(speedBuff.begin() + i);
					itemCollectSound.play();
					break;
				}
			}

			for (size_t i = 0; i < sizeBuff.size(); i++)
			{
				if (sizeBuff[i].getGlobalBounds().intersects(knight.getGlobalBounds()))
				{
					bulletStatus = 2;
					sizeBuff.erase(sizeBuff.begin() + i);
					itemCollectSound.play();
					break;

				}
			}

			//Update event in game (Lighting Strike)
			if (eventTimer < 300)
			{
				eventTimer += 1;
				position = Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100);
			}

			if (eventTimer >= 300)
			{
				lightingState = 1;
				preLighting.setPosition(position);
				countdownLighting.setPosition(position);
				countdownLighting.setRadius(countdownLighting.getRadius() + 1);
				countdownLighting.setOrigin(Vector2f(countdownLighting.getRadius(), countdownLighting.getRadius()));

				if (countdownLighting.getRadius() == preLighting.getRadius())
				{
					if (preLighting.getGlobalBounds().intersects(knight.getGlobalBounds()) && dodgeStatus != 1)
					{
						knightArmor = 0;
						knightHp -= 5;
						knightAtkedSound.play();
					}
					lightingState = 0;
					countdownLighting.setRadius(0);
					eventTimer = 0;

					bigguy.enemy.setPosition(Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					bigguy.enemyPos = Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
					enemies2.push_back(bigguy);
				}
			}

			//Update score
			output = "SCORE : " + to_string(currentScore.score);
			scoreIG.setString(output);

			//Update Game Status
			if (knightHp <= 0)
			{
				gameOverSound.play();
				gameSave = true;
				gameStatus = 3;
			}
		}

		//Leaderboard
		else if (gameStatus == 2)
		{
			//Data load
			fp = fopen("top5_score.txt", "r");

			for (int i = 0; i < MAX_INDEX_LEADERBOARD; i++)
			{
				fscanf(fp, "%s %d", topScore[i].name, &topScore[i].score);
				outputName[i] = topScore[i].name;
			}
			fclose(fp);
			
			//Update leaderboard
			leaderboard[1].setString("1st : " + outputName[0] + " -> " + to_string(topScore[0].score));
			leaderboard[2].setString("2nd : " + outputName[1] + " -> " + to_string(topScore[1].score));
			leaderboard[3].setString("3rd : " + outputName[2] + " -> " + to_string(topScore[2].score));
			leaderboard[4].setString("4th : " + outputName[3] + " -> " + to_string(topScore[3].score));
			leaderboard[5].setString("5th : " + outputName[4] + " -> " + to_string(topScore[4].score));

			//Exit button
			if (leaderboard[6].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
			{
				leaderboard[6].setFillColor(Color::Red);
				leaderboard[6].setCharacterSize(85);
				leaderboard[6].setPosition(Vector2f((window.getSize().x / 2) - leaderboard[6].getGlobalBounds().width / 2, window.getSize().y -120));
			}
			else
			{
				leaderboard[6].setFillColor(Color(255, 255, 215));
				leaderboard[6].setCharacterSize(80);
				leaderboard[6].setPosition(Vector2f((window.getSize().x / 2) - leaderboard[6].getGlobalBounds().width / 2, window.getSize().y -120));
			}
		}

		//Game Over
		else if (gameStatus == 3)
		{
			//Game Over screen increasing size
			gameOverScreen.setOrigin(Vector2f(gameOverScreen.getSize().x/2, gameOverScreen.getSize().y/2));
			if (gameOverScreen.getSize().x <= window.getSize().x)
			{
				gameOverScreen.setSize(Vector2f(gameOverScreen.getSize().x + 18, gameOverScreen.getSize().y));
			}
			if (gameOverScreen.getSize().y <= window.getSize().y)
			{
				gameOverScreen.setSize(Vector2f(gameOverScreen.getSize().x, gameOverScreen.getSize().y+10));
			}

			//Change continue button color
			if (gameOverOutput[2].getGlobalBounds().contains(Vector2f(Mouse::getPosition(window))))
			{
				gameOverOutput[2].setFillColor(Color(255, 255, 215));
				gameOverOutput[2].setCharacterSize(85);
				gameOverOutput[2].setPosition(Vector2f((window.getSize().x / 2) - gameOverOutput[2].getGlobalBounds().width / 2, window.getSize().y / 2 +350));
			}
			else
			{
				gameOverOutput[2].setFillColor(Color::White);
				gameOverOutput[2].setCharacterSize(80);
				gameOverOutput[2].setPosition(Vector2f((window.getSize().x / 2) - gameOverOutput[2].getGlobalBounds().width / 2, window.getSize().y / 2 + 350));
			}
			
			//Data load
			fp = fopen("top5_score.txt", "r");

			for (int i = 0; i < MAX_INDEX_LEADERBOARD; i++)
			{
				fscanf(fp, "%s %d", topScore[i].name, &topScore[i].score);
				outputName[i] = topScore[i].name;
			}

			//Data Replace
			if (currentScore.score > topScore[4].score)
			{
				topScore[4].score = currentScore.score;
				currentScore.score = 0;
				strncpy(topScore[4].name, currentScore.name, sizeof(currentScore.name));
				topScore[4].name[sizeof(currentScore.name - 1)] = '\0';
			}

			//Data Sort
			for (int i = 0; i < MAX_INDEX_LEADERBOARD; ++i)
			{
				for (int j = i + 1; j < MAX_INDEX_LEADERBOARD; ++j)
				{
					if (topScore[i].score < topScore[j].score)
					{
						tempScore.score = topScore[i].score;
						topScore[i].score = topScore[j].score;
						topScore[j].score = tempScore.score;

						strncpy(tempScore.name, topScore[i].name, sizeof(topScore[i].name));
						tempScore.name[sizeof(topScore[i].name)];

						strncpy(topScore[i].name, topScore[j].name, sizeof(topScore[j].name));
						topScore[i].name[sizeof(topScore[j].name)];

						strncpy(topScore[j].name, tempScore.name, sizeof(tempScore.name));
						topScore[j].name[sizeof(tempScore.score)];
					}
				}
			}

			//Data Save
			if (gameSave == true)
			{
				fp = fopen("top5_score.txt", "w");
				for (int i = 0; i < MAX_INDEX_LEADERBOARD; i++)
				{
					fprintf(fp, "%s %d \n", topScore[i].name, topScore[i].score);
				}
				gameSave = false;
			}

			fclose(fp);

			gameOverOutput[0].setString("Total Score : " + to_string(currentScore.score));
			gameOverOutput[1].setString("Insert Name : ");
		}

		//Background stuff (BG moving, BG music)
		if (gameStatus == 0 || gameStatus == 2)
		{
			for (int i = 0; i < 2; i++)
			{
				menuBG[i].move(Vector2f(1.f, 0));
				if (menuBG[i].getGlobalBounds().width - menuBG[i].getPosition().x == 0)
				{
					menuBG[i].setPosition(Vector2f(-1920.f, 0.f));
				}
			}
			menuMusic.setLoop(true);
		}

		//Clear window to draw a new one
		window.clear();

		//Draw window
		if (gameStatus == 0)
		{
			window.draw(menuBG[1]);
			window.draw(menuBG[0]);
			for (int i = 0; i < 4; i++)
			{
				window.draw(menu[i]);
			}
		}
		else if (gameStatus == 1 || gameStatus == 3 || gameStatus == 4)
		{
			window.draw(background);

			if (lightingState == 1)
			{
				window.draw(preLighting);
				window.draw(countdownLighting);
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				window.draw(lifeExtenders[i]);
			}

			for (size_t i = 0; i < armorExtenders.size(); i++)
			{
				window.draw(armorExtenders[i]);
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				window.draw(speedBuff[i]);
			}

			for (size_t i = 0; i < sizeBuff.size(); i++)
			{
				window.draw(sizeBuff[i]);
			}

			for (size_t i = 0; i < bullets.size(); i++)
			{
				window.draw(bullets[i].shape);
			}

			window.draw(knight);

			for (size_t i = 0; i < enemies.size(); i++)
			{
				window.draw(enemies[i].enemy);
			}

			for (size_t i = 0; i < enemies2.size(); i++)
			{
				window.draw(enemies2[i].enemy);
			}

			window.draw(HpBar);

			window.draw(ArmorBar);

			window.draw(DodgeBar);

			window.draw(scoreIG);

			//If game over
			if (gameStatus == 3)
			{
				window.draw(gameOverScreen);

				if (gameOverScreen.getGlobalBounds().width >= window.getSize().x && gameOverScreen.getGlobalBounds().height >= window.getSize().y)
				{
					window.draw(gameOverText);
					window.draw(gameOverOutput[0]);
					window.draw(gameOverOutput[1]);
					window.draw(gameOverOutput[2]);
					window.draw(gameOverOutput[3]);
				}
			}
			else if (gameStatus == 4)
			{

			}
		}
		else if (gameStatus == 2)
		{
			window.draw(menuBG[0]);
			window.draw(menuBG[1]);

			window.draw(leaderboardBehide);

			for (int i = 0; i < 7; i++)
			{
				window.draw(leaderboard[i]);
			}

			window.draw(crown);
		}
		window.display();

	}
	return 0;
}
