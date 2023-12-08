#define _CRT_SECURE_NO_WARNINGS 1
#define MAX_INDEX_LEADERBOARD 5
#define MAX_MENU_CHOICE 4
#define pie 3.14159265359

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Network.hpp>
#include <math.h>
#include <vector>
#include <stdio.h>

int main()
{
	//Declear variables
	srand(time(NULL));
	int phrase = 0;
	int phraseTimer[4][2] = { {5,5},{5,5},{4,4},{4,3} }; //[0] = enemy [1] = thunder
	int enemyNum[2];
	int enemyTimer = 5;
	int eventTimer = 5;
	int bulletTimer = 300;
	int HeatState = 0;
	int bulletDMG = 2;
	int gameStatus = 0;
	int dodgeStatus = 0;
	int exitCount = 0;
	bool gameSave = false;
	bool nameReplace = false;
	bool bulletBuffAtk = false;
	bool bulletBuffSpd = false;
	sf::Vector2f position;
	sf::Clock clockShoot, clockBullet1, clockBullet2, clockItem, clockEvent, clockPhrase, clockDodge, clockEnemy, clockExplode;
	sf::Time timeShoot, timeBullet1, timeBullet2, timeItem, timeEvent, timePhrase, timeDodge, timeEnemy, timeExplode;
	double Tshoot, Tbullet1, Tbullet2, Titem, Tevent, Tphrase, Tdodge, Tenemy, Texplode;

	//Window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Gunner Knight", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	//Font
	sf::Font fontIG;

	if (!fontIG.loadFromFile("Font/yoster.ttf"))
	{
		printf("FAILED TO LOAD FONT\n");
	}

	//Background In game
	sf::Texture backgroundTex;
	sf::Sprite background;

	backgroundTex.loadFromFile("Texture/gameBG.png");
	background.setTexture(backgroundTex);
	background.setScale(1.f, 1.f);
	background.setColor(sf::Color(255, 255, 255, 200));

	//Menu
	sf::Text menu[4];
	sf::Text studentName;
	sf::Texture menuBG_Tex;
	sf::Sprite menuBG[2];

	studentName.setCharacterSize(60);
	studentName.setFont(fontIG);
	studentName.setFillColor(sf::Color(255, 255, 215));
	studentName.setOutlineThickness(7.f);
	studentName.setOutlineColor(sf::Color::Black);
	studentName.setString("Woramet Thammajariyawat 64010765");
	studentName.setPosition(sf::Vector2f(30,1000));

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

	for (int i = 0; i < MAX_MENU_CHOICE; i++)
	{
		menu[i].setFont(fontIG);
		menu[i].setOutlineThickness(7.f);
		menu[i].setOutlineColor(sf::Color::Black);
		menu[i].setPosition(sf::Vector2f((window.getSize().x / 2) / 2 - (menu[i].getGlobalBounds().width / 2), window.getSize().x / 2 / (MAX_MENU_CHOICE + 1) * (i + 1)));
	}
	menu[0].setCharacterSize(120);
	menu[0].setFillColor(sf::Color(255,255,215));
	menu[0].setPosition(sf::Vector2f((window.getSize().x / 2) - menu[0].getGlobalBounds().width / 2, window.getSize().y / (MAX_MENU_CHOICE + 1) * 0.7));

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
	
	std::string outputName[5];

	//Leaderboard
	sf::RectangleShape leaderboardBehide;
	sf::Texture crownTex;
	sf::Sprite crown;
	sf::Text leaderboard[7];
	leaderboard[0].setString("Leaderboard");
	leaderboard[6].setString("Go back");

	for (int i = 0; i < 7; i++)
	{
		leaderboard[i].setFont(fontIG);
		leaderboard[i].setFillColor(sf::Color::White);
		leaderboard[i].setOutlineThickness(7.f);
		leaderboard[i].setOutlineColor(sf::Color::Black);
		leaderboard[i].setCharacterSize(80);
		leaderboard[i].setPosition(sf::Vector2f((window.getSize().x / 2) - 530, window.getSize().y / (8) * (i+1)));
	}

	leaderboard[0].setCharacterSize(120);
	leaderboard[0].setFillColor(sf::Color(255, 255, 215));
	leaderboard[0].setPosition(sf::Vector2f((window.getSize().x / 2) - leaderboard[0].getGlobalBounds().width / 2, window.getSize().y -1000));

	leaderboard[1].setFillColor(sf::Color(255, 255, 153));
	leaderboard[2].setFillColor(sf::Color(190, 194, 203));
	leaderboard[3].setFillColor(sf::Color(205, 127, 50));

	crownTex.loadFromFile("Texture/1st_nobg.png");
	crown.setTexture(crownTex);
	crown.setScale(0.15f, 0.15f);
	crown.setPosition(sf::Vector2f((window.getSize().x / 2) - 640, window.getSize().y / (8)*2+10));

	leaderboardBehide.setFillColor(sf::Color(255, 255, 215,100));
	leaderboardBehide.setOutlineThickness(5.f);
	leaderboardBehide.setOutlineColor(sf::Color(255, 255, 170));
	leaderboardBehide.setSize(sf::Vector2f(2000, 700));
	leaderboardBehide.setOrigin(leaderboardBehide.getSize().x / 2, leaderboardBehide.getSize().y / 2);
	leaderboardBehide.setPosition(sf::Vector2f(window.getSize().x/2, window.getSize().y/2+50));

	//Game Over
	bool gameOverReady = false;
	sf::Text gameOverText;
	gameOverText.setFont(fontIG);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setCharacterSize(200);
	gameOverText.setString("GAME OVER");
	gameOverText.setOrigin(sf::Vector2f(gameOverText.getGlobalBounds().width/2, gameOverText.getGlobalBounds().height/2));
	gameOverText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2 - 300));

	sf::RectangleShape gameOverScreen;
	gameOverScreen.setFillColor(sf::Color::Black);
	gameOverScreen.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	std::string gameOverInput;
	sf::Text gameOverOutput[4]; //[0] = score [1] = name [2] = continue [3] = nameInput

	for (int i = 0; i < 4; i++)
	{
		gameOverOutput[i].setFont(fontIG);
		gameOverOutput[i].setFillColor(sf::Color::White);
		gameOverOutput[i].setCharacterSize(80);
		gameOverOutput[i].setPosition(sf::Vector2f(window.getSize().x / 2 - 600, window.getSize().y /2 + 150*i));
	}

	gameOverOutput[2].setString("Confirm");
	gameOverOutput[2].setPosition(sf::Vector2f(window.getSize().x / 2 - gameOverOutput[2].getGlobalBounds().width/2, window.getSize().y / 2 + 350));
	gameOverOutput[3].setPosition((sf::Vector2f(window.getSize().x / 2 + 6, window.getSize().y / 2 + 150)));

	//Music
	sf::Music menuMusic;
	if (!menuMusic.openFromFile("Sound/gameMenu.wav"))
	{
		printf("FAILED TO LOAD MUSIC\n");
	}
	menuMusic.setVolume(20);
	menuMusic.play();

	sf::Music gameMusic;
	if (!gameMusic.openFromFile("Sound/gameIG.wav"))
	{
		printf("FAILED TO LOAD MUSIC\n");
	}
	gameMusic.setVolume(10);

	//Sound Effect
	sf::SoundBuffer gunBuffer;
	sf::Sound gunSound;
	if (!gunBuffer.loadFromFile("Sound/gunShot.wav"))
	{
		printf("COULD NOT LOAD GUNSHOT SOUND");
	}
	gunSound.setBuffer(gunBuffer);

	sf::SoundBuffer gameOverBuffer;
	sf::Sound gameOverSound;
	if (!gameOverBuffer.loadFromFile("Sound/gameOver.wav"))
	{
		printf("COULD NOT LOAD GAMEOVER SOUND");
	}
	gameOverSound.setBuffer(gameOverBuffer);

	sf::SoundBuffer interfaceClickBuffer;
	sf::Sound interfaceClickSound;
	if (!interfaceClickBuffer.loadFromFile("Sound/interfaceClick1.wav"))
	{
		printf("COULD NOT LOAD INTERFACE CLICK SOUND");
	}
	interfaceClickSound.setBuffer(interfaceClickBuffer);

	sf::SoundBuffer interfaceClickBuffer2;
	sf::Sound interfaceClickSound2;
	if (!interfaceClickBuffer2.loadFromFile("Sound/interfaceClick2.wav"))
	{
		printf("COULD NOT LOAD INTERFACE CLICK SOUND2");
	}
	interfaceClickSound2.setBuffer(interfaceClickBuffer2);

	sf::SoundBuffer knightAtkedBuffter;
	sf::Sound knightAtkedSound;
	if (!knightAtkedBuffter.loadFromFile("Sound/knightAtked.wav"))
	{
		printf("COULD NOT LOAD KNIGHT ATTACKED SOUND");
	}
	knightAtkedSound.setBuffer(knightAtkedBuffter);

	sf::SoundBuffer enemyAtkedBuffter;
	sf::Sound enemyAtkedSound;
	if (!enemyAtkedBuffter.loadFromFile("Sound/enemyAtked.wav"))
	{
		printf("COULD NOT LOAD ENEMY ATTACKED SOUND");
	}
	enemyAtkedSound.setBuffer(enemyAtkedBuffter);

	sf::SoundBuffer enemyDiedBuffer;
	sf::Sound enemyDiedSound;
	if (!enemyDiedBuffer.loadFromFile("Sound/enemyDie.wav"))
	{
		printf("COULD NOT LOAD ENEMY DIE SOUND");
	}
	enemyDiedSound.setBuffer(enemyDiedBuffer);

	sf::SoundBuffer dashBuffer;
	sf::Sound dashSound;
	if (!dashBuffer.loadFromFile("Sound/dash.wav"))
	{
		printf("COULD NOT LOAD  DASH SOUND");
	}
	dashSound.setBuffer(dashBuffer);

	sf::SoundBuffer itemCollectBuffer;
	sf::Sound itemCollectSound;
	if (!itemCollectBuffer.loadFromFile("Sound/itemCollect.wav"))
	{
		printf("COULD NOT LOAD ITEM COLLECT SOUND");
	}
	itemCollectSound.setBuffer(itemCollectBuffer);

	sf::SoundBuffer nameErrorBuffer;
	sf::Sound nameErrorSound;
	if (!nameErrorBuffer.loadFromFile("Sound/nameError.wav"))
	{
		printf("COULD NOT LOAD NAME ERROR SOUND");
	}
	nameErrorSound.setBuffer(nameErrorBuffer);

	gunSound.setVolume(10);
	interfaceClickSound.setVolume(40);
	interfaceClickSound2.setVolume(40);
	enemyDiedSound.setVolume(20);
	enemyAtkedSound.setVolume(10);
	knightAtkedSound.setVolume(20);
	itemCollectSound.setVolume(40);
	nameErrorSound.setVolume(30);

	//Knight
	sf::Texture knightTex;
	sf::Sprite knight;
	int knightHp = 1;
	int knightArmor = 5;
	float knightSpeedNormal[2] = { 8.f, -8.f };
	float knightSpeedBoost[2] = { 16.f, -16.f };
	float knightSpeedCurrent[2] = { 8.f, -8.f };

	knightTex.loadFromFile("Texture/knight2.png");
	knight.setTexture(knightTex);
	knight.setScale(sf::Vector2f(0.8f, 0.8f));
	knight.setOrigin(sf::Vector2f(knight.getTexture()->getSize().x / 2, knight.getTexture()->getSize().y / 2));
	knight.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	//Hitbox
	sf::RectangleShape hitbox;
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setSize(sf::Vector2f(knight.getGlobalBounds().width-60,knight.getGlobalBounds().height-60));
	hitbox.setOrigin(sf::Vector2f(knight.getTexture()->getSize().x / 2 - 40, knight.getTexture()->getSize().y / 2 - 50));
	hitbox.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	//Enemy
	struct Enemy
	{
		sf::Texture enemyTex;
		sf::Sprite enemy;
		sf::Vector2f enemyVelo;
		sf::Vector2f enemyCenter;
		sf::Vector2f enemyDir;
		sf::Vector2f enemyDirNorm;
		sf::Vector2f enemyPos;
		int hp;
		int score;
		int dmg;
		float rotation_enemy;
	};

	Enemy badguy;
	badguy.enemyTex.loadFromFile("Texture/enemyType1.png");
	badguy.enemy.setTexture(badguy.enemyTex);
	badguy.enemy.setScale(sf::Vector2f(0.4f, 0.4f));
	badguy.enemy.setOrigin(sf::Vector2f(badguy.enemy.getTexture()->getSize().x / 2, badguy.enemy.getTexture()->getSize().y / 2));
	badguy.hp = 5;
	badguy.dmg = 1;
	badguy.score = 50;

	Enemy bigguy;
	bigguy.enemyTex.loadFromFile("Texture/enemyType2.png");
	bigguy.enemy.setTexture(bigguy.enemyTex);
	bigguy.enemy.setScale(sf::Vector2f(0.7f, 0.7f));
	bigguy.enemy.setOrigin(sf::Vector2f(bigguy.enemy.getTexture()->getSize().x / 2, bigguy.enemy.getTexture()->getSize().y / 2));
	bigguy.hp = 10;
	bigguy.dmg = 1;
	bigguy.score = 100;

	Enemy strongguy;
	strongguy.enemyTex.loadFromFile("Texture/enemyType3.png");
	strongguy.enemy.setTexture(strongguy.enemyTex);
	strongguy.enemy.setScale(sf::Vector2f(0.4f, 0.4f));
	strongguy.enemy.setOrigin(sf::Vector2f(strongguy.enemy.getTexture()->getSize().x / 2, strongguy.enemy.getTexture()->getSize().y / 2));
	strongguy.hp = 7;
	strongguy.dmg = 2;
	strongguy.score = 300;

	std::vector<Enemy>enemies; // walk toward the knight
	std::vector<Enemy>enemies2; // walk around the map, not toward the knight

	//Bullet
	struct Bullet
	{
		sf::RectangleShape shape;
		sf::Vector2f bulletVelo;
		float rotation_bullet;
	};

	Bullet projectile;
	projectile.shape.setSize(sf::Vector2f(10.f, 10.f));
	projectile.shape.setFillColor(sf::Color(255, 215, 0));
	projectile.shape.setOutlineThickness(3.f);
	projectile.shape.setOutlineColor(sf::Color::Black);
	projectile.shape.setOrigin(sf::Vector2f(projectile.shape.getGlobalBounds().width / 2, projectile.shape.getGlobalBounds().height / 2));

	std::vector<Bullet>bullets;
	
	//HP Bar
	sf::Texture HpBarGraphicTex;
	sf::Sprite HpBarGraphic;
	sf::RectangleShape HpBar;

	HpBarGraphicTex.loadFromFile("Texture/HpBar.png");
	HpBarGraphic.setTexture(HpBarGraphicTex);
	HpBarGraphic.setScale(0.7f, 0.7f);
	HpBarGraphic.setPosition(sf::Vector2f(50, 50));
	HpBar.setFillColor(sf::Color::Red);
	HpBar.setPosition(sf::Vector2f(100, 60));

	//Armor Bar
	sf::Texture ArmorBarGraphicTex;
	sf::Sprite ArmorBarGraphic;
	sf::RectangleShape ArmorBar;

	ArmorBarGraphicTex.loadFromFile("Texture/armorBar.png");
	ArmorBarGraphic.setTexture(ArmorBarGraphicTex);
	ArmorBarGraphic.setScale(0.5f, 0.5f);
	ArmorBarGraphic.setPosition(sf::Vector2f(50,150));
	ArmorBar.setFillColor(sf::Color(192, 192, 192));
	ArmorBar.setPosition(sf::Vector2f(105, 165));

	//Dodge Bar
	sf::Texture DodgeBarTex;
	sf::Sprite DodgeBar;

	DodgeBarTex.loadFromFile("Texture/dash.png");
	DodgeBar.setTexture(DodgeBarTex);
	DodgeBar.setScale(1.f,1.f);
	DodgeBar.setPosition(sf::Vector2f(375, 155));

	//HP Potion
	sf::Texture hpPotionTex;
	sf::Sprite hpPotion;
	hpPotionTex.loadFromFile("Texture/heal.png");
	hpPotion.setTexture(hpPotionTex);
	int hpPlus = 2;

	std::vector<sf::Sprite>lifeExtenders;

	//Armor Potion
	sf::Texture armorPotionTex;
	sf::Sprite armorPotion;
	armorPotionTex.loadFromFile("Texture/armor.png");
	armorPotion.setTexture(armorPotionTex);
	armorPotion.setScale(1.f, 1.f);
	int armorPlus = 1;

	std::vector<sf::Sprite>armorExtenders;

	//Buff Bullet
	sf::Texture speedBulletTex;
	sf::Sprite speedBullet; //Speed
	speedBulletTex.loadFromFile("Texture/fast.png");
	speedBullet.setTexture(speedBulletTex);

	sf::Texture dmgBulletTex;
	sf::Sprite dmgBullet; //Dmg
	dmgBulletTex.loadFromFile("Texture/dmg.png");
	dmgBullet.setTexture(dmgBulletTex);

	std::vector<sf::Sprite>speedBuff;
	std::vector<sf::Sprite>dmgBuff;

	//Score
	std::string output;

	sf::Text scoreIG;
	scoreIG.setFont(fontIG);
	scoreIG.setCharacterSize(50);
	scoreIG.setOutlineColor(sf::Color::Black);
	scoreIG.setOutlineThickness(7.f);
	scoreIG.setString(output);
	scoreIG.setPosition(60,240);

	//Heat
	sf::CircleShape preHeat;
	preHeat.setRadius(200);
	preHeat.setFillColor(sf::Color(255,0,0, 80));
	preHeat.setOrigin(sf::Vector2f(preHeat.getRadius() , preHeat.getRadius() ));

	sf::CircleShape countdownHeat;
	countdownHeat.setRadius(0);
	countdownHeat.setFillColor(sf::Color(255,0,0, 150));
	countdownHeat.setOrigin(sf::Vector2f(countdownHeat.getRadius() , countdownHeat.getRadius() ));

	//Program start running
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonReleased:
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					if (gameStatus == 0)
					{
						if (menu[1].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))  //click play -> set zero everything
						{
							interfaceClickSound.play();
							knightHp = 10;
							knightArmor = 5;
							HeatState = 0;
							countdownHeat.setRadius(0.f);
							knight.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
							clockEnemy.restart();
							clockPhrase.restart();
							clockEvent.restart();
							gameStatus = 1;
						}
						if (menu[2].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
						{
							interfaceClickSound.play();
							gameStatus = 2;
						}
						if (menu[3].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
						{
							interfaceClickSound.play();
							window.close();
						}
					}
					if (gameStatus == 2)
					{
						if (leaderboard[6].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
						{
							interfaceClickSound2.play();
							gameStatus = 0;
						}
					}
					if (gameStatus == 3)
					{
						if (gameOverOutput[2].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && gameOverInput.size() != 0)
						{
							interfaceClickSound2.play();
							nameReplace = true;
							gameOverScreen.setSize(sf::Vector2f(0,0));
							gameStatus = 0;
						}
						else if ((gameOverOutput[2].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))) && gameOverInput.size() == 0))
						{
							nameErrorSound.play();
						}
					}
				default:
					break;
				}
				break;
			case sf::Event::TextEntered:
			{
				if (gameStatus == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					interfaceClickSound2.play();
					exitCount += 1;
				}
				if (gameStatus == 3 && gameOverScreen.getGlobalBounds().width >= window.getSize().x && gameOverScreen.getGlobalBounds().height >= window.getSize().y)
				{
					if (event.text.unicode != 0x000008 && event.text.unicode != 0x00000020 && gameOverInput.size() < 10)
					{
						gameOverInput += event.text.unicode;
					}
					else if (event.text.unicode == 0x000008)
					{
						if (gameOverInput.size() != 0)
						{
							gameOverInput.erase(gameOverInput.size() - 1, 1);
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}

		//Menu
		if (gameStatus == 0)
		{
			//Music on if it's off
			if (menuMusic.getStatus() == sf::Sound::Status::Stopped)
			{
				menuMusic.play();
			}

			//Turn off ingame music
			if (gameMusic.getStatus() == sf::Music::Status::Playing)
			{
				gameMusic.stop();
			}

			//Menu button color changing
			for (int i = 1; i < MAX_MENU_CHOICE; i++)
			{
				if (menu[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
				{
					menu[i].setFillColor(sf::Color::Red);
					menu[i].setCharacterSize(85);
					menu[i].setPosition(sf::Vector2f((window.getSize().x / 2) - menu[i].getGlobalBounds().width / 2, window.getSize().y / (MAX_MENU_CHOICE + 1) * (i + 1)));
				}
				else 
				{ 
					menu[i].setFillColor(sf::Color::White);
					menu[i].setCharacterSize(80);
					menu[i].setPosition(sf::Vector2f((window.getSize().x / 2) - menu[i].getGlobalBounds().width / 2, window.getSize().y / (MAX_MENU_CHOICE + 1) * (i + 1)));
				}
			}

			if (nameReplace)
			{
				strcpy(currentScore.name, gameOverInput.c_str());
				nameReplace = false;
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
			gameOverInput = "";
		}

		//Game
		else if (gameStatus == 1)
		{
			//Stop playing menu music
			menuMusic.stop();

			//Play ingame music
			if (gameMusic.getStatus() == sf::Music::Status::Stopped)
			{
				gameMusic.play();
				gameMusic.setLoop(true);
			}
			
			//Exit key
			if (exitCount >= 2)
			{
				gameOverSound.play();
				exitCount = 0;
				gameStatus = 0;
			}

			//Get and store time
			timeShoot = clockShoot.getElapsedTime();
			timeBullet1 = clockBullet1.getElapsedTime();
			timeBullet2 = clockBullet2.getElapsedTime();
			timeItem = clockItem.getElapsedTime();
			timeEvent = clockEvent.getElapsedTime();
			timePhrase = clockPhrase.getElapsedTime();
			timeDodge = clockDodge.getElapsedTime();
			timeEnemy = clockEnemy.getElapsedTime();
			timeExplode = clockExplode.getElapsedTime();

			Tshoot = timeShoot.asMilliseconds();
			Tbullet1 = timeBullet1.asSeconds();
			Tbullet2 = timeBullet2.asSeconds();
			Titem = timeItem.asSeconds();
			Tevent = timeEvent.asSeconds();
			Tphrase = timePhrase.asSeconds();
			Tdodge = timeDodge.asSeconds();
			Tenemy = timeEnemy.asSeconds();
			Texplode = timeExplode.asMilliseconds();

			//Phrase phraseTimer[4][2] = { {4,5},{4,5},{3,4},{3,3} }; //[0] = enemy [1] = thunder
			if (Tphrase <= 20) //phrase 0
			{
				phrase = 0;
				enemyTimer = phraseTimer[phrase][0];
				eventTimer = phraseTimer[phrase][1];
				enemyNum[0] = 4;
				enemyNum[1] = 2;
			}
			else if (Tphrase <= 40) //phrase 1
			{
				phrase = 1;
				enemyTimer = phraseTimer[phrase][0];
				eventTimer = phraseTimer[phrase][1];
				enemyNum[0] = 3;
				enemyNum[1] = 2;
			}
			else if (Tphrase <= 60) //phrase 2
			{
				phrase = 2;
				enemyTimer = phraseTimer[phrase][0];
				eventTimer = phraseTimer[phrase][1];
				enemyNum[0] = 1;
				enemyNum[1] = 2;
			}
			else //phrase 3
			{
				phrase = 3;
				enemyTimer = phraseTimer[phrase][0];
				eventTimer = phraseTimer[phrase][1];
				enemyNum[0] = 2;
				enemyNum[1] = 2;
			}
			
			//Update knight -> Direction and rotating
			sf::Vector2f knightCenter = sf::Vector2f(hitbox.getPosition().x, hitbox.getPosition().y);
			sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
			sf::Vector2f aimDir = mousePos - knightCenter;
			sf::Vector2f aimDirNorm = aimDir / float(sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2)));

			float dx_knight = mousePos.x - hitbox.getPosition().x;
			float dy_knight = mousePos.y - hitbox.getPosition().y;
			float rotation_knight = (atan2(dy_knight, dx_knight)) * 180 / pie;
			knight.setRotation(rotation_knight + 90);
			hitbox.setPosition(sf::Vector2f(knight.getPosition().x, knight.getPosition().y));
			//Update control
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && knight.getPosition().x - +hitbox.getGlobalBounds().width / 2 > 0)
			{
				knight.move(knightSpeedCurrent[1], 0.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && knight.getPosition().x + hitbox.getGlobalBounds().width / 2 < window.getSize().x)
			{
				knight.move(knightSpeedCurrent[0], 0.0f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && knight.getPosition().y - hitbox.getGlobalBounds().height / 2 > 0)
			{
				knight.move(0.0f, knightSpeedCurrent[1]);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && knight.getPosition().y + hitbox.getGlobalBounds().height / 2 < window.getSize().y)
			{
				knight.move(0.0f, knightSpeedCurrent[0]);
			}

			//Update dodge
			if (Tdodge < 3)
			{
				DodgeBar.setColor(sf::Color(255, 255, 255, 122));
			}
			else
			{
				DodgeBar.setColor(sf::Color(255,255,255,255));
			}

			if (dodgeStatus == 1 && Tdodge <= 0.5)
			{
				knightSpeedCurrent[0] = knightSpeedBoost[0];
				knightSpeedCurrent[1] = knightSpeedBoost[1];
				knight.setColor(sf::Color(255,255,255,128));
			}
			else if (dodgeStatus == 1 && Tdodge > 0.5)
			{
				knightSpeedCurrent[0] = knightSpeedNormal[0];
				knightSpeedCurrent[1] = knightSpeedNormal[1];
				knight.setColor(sf::Color::White);
				dodgeStatus = 0;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && dodgeStatus == 0 && Tdodge >= 3)
			{
				dodgeStatus = 1;
				clockDodge.restart();

				dashSound.setPlayingOffset(sf::seconds(1.f));
				dashSound.play();
			}

			//Update enemy
			sf::Vector2f spawnPosition[4] = {(sf::Vector2f(2000,rand()%1080)),(sf::Vector2f(-200,rand() % 1080)),(sf::Vector2f(rand()%1920,-200),(sf::Vector2f(rand()%1920,1200)))}; //East , West , North , South

			if (Tenemy >= enemyTimer) // spawn enemy
			{
				for (int i = 0; i < enemyNum[0]; i++)
				{
					badguy.enemy.setPosition(spawnPosition[rand()%4]);
					enemies.push_back(Enemy(badguy));
				}

				for (int i = 0; i < enemyNum[1]; i++)
				{
					badguy.enemy.setPosition(spawnPosition[rand() % 4]);
					badguy.enemyPos = sf::Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
					enemies2.push_back(Enemy(badguy));
				}

				if (phrase >= 1)
				{
					bigguy.enemy.setPosition(spawnPosition[rand() % 4]);
					bigguy.enemyPos = sf::Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
					enemies2.push_back(bigguy);
				}
				
				if (phrase >= 2)
				{
					bigguy.enemy.setPosition(spawnPosition[rand() % 4]);
					enemies.push_back(bigguy);
				}

				if (phrase >= 3)
				{
					strongguy.enemy.setPosition(spawnPosition[rand() % 4]);
					enemies.push_back(strongguy);
				}

				clockEnemy.restart();
			}

			for (size_t j = 0; j < enemies.size(); j++) //enemies1
			{
				if (enemies[j].hp <= 0) // enemy delete if dies
				{
					currentScore.score += enemies[j].score;
					enemies.erase(enemies.begin() + j);
					enemyDiedSound.play();
					break;
				}

				if (enemies[j].enemy.getGlobalBounds().intersects(hitbox.getGlobalBounds()) && dodgeStatus != 1) // enemy delete if attack(reach) knight
				{
					if (knightArmor > 0) { knightArmor -= enemies[j].dmg; }
					else { knightHp -= enemies[j].dmg; }
					enemies.erase(enemies.begin() + j);
					enemyDiedSound.play();
					knightAtkedSound.play();
					break;
				}

				// rotating enemy
				float dx_enemy = hitbox.getPosition().x - enemies[j].enemy.getPosition().x;
				float dy_enemy = hitbox.getPosition().y - enemies[j].enemy.getPosition().y;
				enemies[j].rotation_enemy = (atan2(dy_enemy, dx_enemy)) * 180 / pie;
				enemies[j].enemy.setRotation(enemies[j].rotation_enemy + 90);

				//moving enemy
				enemies[j].enemyCenter = sf::Vector2f(enemies[j].enemy.getPosition().x - 10, enemies[j].enemy.getPosition().y - 10);
				enemies[j].enemyDir = knightCenter - enemies[j].enemyCenter;
				enemies[j].enemyDirNorm = enemies[j].enemyDir / float(sqrt(pow(enemies[j].enemyDir.x, 2) + pow(enemies[j].enemyDir.y, 2)));
				enemies[j].enemyVelo = enemies[j].enemyDirNorm * 2.f;
				enemies[j].enemy.move(enemies[j].enemyVelo);
			}

			for (size_t j = 0; j < enemies2.size(); j++) //enemies2
			{
				if (enemies2[j].hp <= 0) // enemy2 delete if dies
				{
					currentScore.score += enemies2[j].score;
					enemies2.erase(enemies2.begin() + j);
					enemyDiedSound.play();
					break;
				}

				if (enemies2[j].enemy.getGlobalBounds().intersects(hitbox.getGlobalBounds()) && dodgeStatus != 1) // enemy2 delete if attack(reach) knight
				{
					if (knightArmor > 0) { knightArmor -= enemies2[j].dmg; }
					else { knightHp -= enemies2[j].dmg; }
					enemies2.erase(enemies2.begin() + j);
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
				enemies2[j].enemyCenter = sf::Vector2f(enemies2[j].enemy.getPosition().x - 10, enemies2[j].enemy.getPosition().y - 10);
				enemies2[j].enemyDir = enemies2[j].enemyPos - enemies2[j].enemyCenter;
				enemies2[j].enemyDirNorm = enemies2[j].enemyDir / float(sqrt(pow(enemies2[j].enemyDir.x, 2) + pow(enemies2[j].enemyDir.y, 2)));
				enemies2[j].enemyVelo = enemies2[j].enemyDirNorm * 2.f;
				enemies2[j].enemy.move(enemies2[j].enemyVelo);

				if (enemies2[j].enemy.getGlobalBounds().contains(enemies2[j].enemyPos))
				{
					enemies2[j].enemyPos = sf::Vector2f(rand() % window.getSize().x - 100, rand() % window.getSize().y - 100);
				}
			}

			//Update shooting
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && Tshoot >= bulletTimer) // spawn bullet
			{
				projectile.shape.setPosition(knightCenter);
				projectile.bulletVelo = aimDirNorm * 20.f;
				float dx_bullet = mousePos.x - hitbox.getPosition().x;
				float dy_bullet = mousePos.y - hitbox.getPosition().y;
				projectile.rotation_bullet = (atan2(dx_bullet, dy_bullet)) * 180 / pie;
				bullets.push_back(Bullet(projectile));
				clockShoot.restart();

				gunSound.play();
			}

			for (size_t i = 0; i < bullets.size(); i++)  //Collision and movement bullet 
			{
				bullets[i].shape.move(bullets[i].bulletVelo);
				bullets[i].shape.setRotation(bullets[i].rotation_bullet);

				if (bullets[i].shape.getPosition().y + 500 < 0 || bullets[i].shape.getPosition().y - 500 > window.getSize().y - bullets[i].shape.getGlobalBounds().width || bullets[i].shape.getPosition().x + 500 < 0 || bullets[i].shape.getPosition().x - 500 > window.getSize().x - bullets[i].shape.getGlobalBounds().width)
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
			HpBar.setSize(sf::Vector2f(48.9 * knightHp, 65));
			ArmorBar.setSize(sf::Vector2f(49 * knightArmor, 50));

			//Bullet Effect
			if (Tbullet1 <= 5 && bulletBuffSpd == true)
			{
				bulletTimer = 150;
			}
			else
			{
				bulletTimer = 300;
				bulletBuffSpd = false;
			}

			if (Tbullet2 <= 5 && bulletBuffAtk == true)
			{
				bulletDMG = 3;
				projectile.shape.setScale(sf::Vector2f(2.f, 2.f));
				projectile.shape.setFillColor(sf::Color::Red);
			}
			else
			{
				bulletDMG = 2;
				projectile.shape.setScale(sf::Vector2f(1.f, 1.f));
				projectile.shape.setFillColor(sf::Color(255, 215, 0));
				bulletBuffAtk = false;
			}
			

			//Update items
			if (Titem >= 5)
			{
				int itemChoice = rand() % 4; // 0 = hp pot spawn. 1 = armor pot spawn. 2 = speed buff 3 = atk buff
				switch (itemChoice)
				{
				case 0:
					hpPotion.setPosition(sf::Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					lifeExtenders.push_back(sf::Sprite(hpPotion));
					break;

				case 1:
					armorPotion.setPosition(sf::Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					armorExtenders.push_back(sf::Sprite(armorPotion));
					break;
				case 2:
					speedBullet.setPosition(sf::Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					speedBuff.push_back(sf::Sprite(speedBullet));
					break;
				case 3:
					dmgBullet.setPosition(sf::Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100));
					dmgBuff.push_back(sf::Sprite(dmgBullet));
					break;
				default:
					break;
				}
				clockItem.restart();
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				if (lifeExtenders[i].getGlobalBounds().intersects(hitbox.getGlobalBounds()))
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
				if (armorExtenders[i].getGlobalBounds().intersects(hitbox.getGlobalBounds()))
				{
					if (knightArmor + armorPlus > 5) { knightArmor = 5; }
					else { knightArmor += armorPlus; }
					armorExtenders.erase(armorExtenders.begin() + i);
					itemCollectSound.play();
					break;
				}
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				if (speedBuff[i].getGlobalBounds().intersects(hitbox.getGlobalBounds()))
				{
					bulletBuffSpd = true;
					speedBuff.erase(speedBuff.begin() + i);
					itemCollectSound.play();
					clockBullet1.restart();
					break;
				}
			}

			for (size_t i = 0; i < dmgBuff.size(); i++)
			{
				if (dmgBuff[i].getGlobalBounds().intersects(hitbox.getGlobalBounds()))
				{
					bulletBuffAtk = true;
					dmgBuff.erase(dmgBuff.begin() + i);
					itemCollectSound.play();
					clockBullet2.restart();
					break;

				}
			}

			//Update event in game (Heat Strike)
			if (Tevent < eventTimer)
			{
				position = sf::Vector2f(rand() % (window.getSize().x - 100), rand() % (window.getSize().y) - 100);
			}

			if (Tevent >= eventTimer)
			{
				HeatState = 1;
				preHeat.setPosition(position);
				countdownHeat.setPosition(position);
				countdownHeat.setRadius(countdownHeat.getRadius() + 1);
				countdownHeat.setOrigin(sf::Vector2f(countdownHeat.getRadius(), countdownHeat.getRadius()));

				if (countdownHeat.getRadius() == preHeat.getRadius())
				{
					if (preHeat.getGlobalBounds().intersects(hitbox.getGlobalBounds()) && dodgeStatus != 1)
					{
						knightArmor = 0;
						knightHp -= 5;
						knightAtkedSound.play();
					}
					HeatState = 0;
					countdownHeat.setRadius(0);
					clockEvent.restart();
				}
			}

			//Update score
			output = "SCORE : " + std::to_string(currentScore.score);
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
			leaderboard[1].setString("1st : " + outputName[0] + " -> " + std::to_string(topScore[0].score));
			leaderboard[2].setString("2nd : " + outputName[1] + " -> " + std::to_string(topScore[1].score));
			leaderboard[3].setString("3rd : " + outputName[2] + " -> " + std::to_string(topScore[2].score));
			leaderboard[4].setString("4th : " + outputName[3] + " -> " + std::to_string(topScore[3].score));
			leaderboard[5].setString("5th : " + outputName[4] + " -> " + std::to_string(topScore[4].score));

			//Exit button
			if (leaderboard[6].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
			{
				leaderboard[6].setFillColor(sf::Color::Red);
				leaderboard[6].setCharacterSize(85);
				leaderboard[6].setPosition(sf::Vector2f((window.getSize().x / 2) - leaderboard[6].getGlobalBounds().width / 2, window.getSize().y -120));
			}
			else
			{
				leaderboard[6].setFillColor(sf::Color(255, 255, 215));
				leaderboard[6].setCharacterSize(80);
				leaderboard[6].setPosition(sf::Vector2f((window.getSize().x / 2) - leaderboard[6].getGlobalBounds().width / 2, window.getSize().y -120));
			}
		}

		//Game Over
		else if (gameStatus == 3)
		{
			//Turn off ingame music
			if (gameMusic.getStatus() == sf::Music::Status::Playing)
			{
				gameMusic.stop();
			}

			//Game Over screen increasing size
			gameOverScreen.setOrigin(sf::Vector2f(gameOverScreen.getSize().x/2, gameOverScreen.getSize().y/2));
			if (gameOverScreen.getSize().x <= window.getSize().x)
			{
				gameOverScreen.setSize(sf::Vector2f(gameOverScreen.getSize().x + 18, gameOverScreen.getSize().y));
			}
			if (gameOverScreen.getSize().y <= window.getSize().y)
			{
				gameOverScreen.setSize(sf::Vector2f(gameOverScreen.getSize().x, gameOverScreen.getSize().y+10));
			}

			//Change continue button color
			if (gameOverOutput[2].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
			{
				gameOverOutput[2].setFillColor(sf::Color(255, 255, 215));
				gameOverOutput[2].setCharacterSize(85);
				gameOverOutput[2].setPosition(sf::Vector2f((window.getSize().x / 2) - gameOverOutput[2].getGlobalBounds().width / 2, window.getSize().y / 2 +350));
			}
			else
			{
				gameOverOutput[2].setFillColor(sf::Color::White);
				gameOverOutput[2].setCharacterSize(80);
				gameOverOutput[2].setPosition(sf::Vector2f((window.getSize().x / 2) - gameOverOutput[2].getGlobalBounds().width / 2, window.getSize().y / 2 + 350));
			}
			
			gameOverOutput[0].setString("Total Score : " + std::to_string(currentScore.score));
			gameOverOutput[1].setString("Insert Name : ");
			gameOverOutput[3].setString(gameOverInput);

			//Remove stuff from last game
			for (size_t j = 0; j < enemies.size(); j++)
			{
				enemies.erase(enemies.begin() + j);
			}

			for (size_t j = 0; j < enemies2.size(); j++)
			{
				enemies2.erase(enemies2.begin() + j);
			}

			for (size_t i = 0; i < bullets.size(); i++)
			{
				bullets.erase(bullets.begin() + i);
			}

			for (size_t i = 0; i < lifeExtenders.size(); i++)
			{
				lifeExtenders.erase(lifeExtenders.begin() + i);
			}

			for (size_t i = 0; i < armorExtenders.size(); i++)
			{
				armorExtenders.erase(armorExtenders.begin() + i);
			}

			for (size_t i = 0; i < speedBuff.size(); i++)
			{
				speedBuff.erase(speedBuff.begin() + i);
			}

			for (size_t i = 0; i < dmgBuff.size(); i++)
			{
				dmgBuff.erase(dmgBuff.begin() + i);
			}
		}

		//Background stuff (BG moving, BG music)
		if (gameStatus == 0 || gameStatus == 2)
		{
			for (int i = 0; i < 2; i++)
			{
				menuBG[i].move(sf::Vector2f(1.f, 0));
				if (menuBG[i].getGlobalBounds().width - menuBG[i].getPosition().x == 0)
				{
					menuBG[i].setPosition(sf::Vector2f(-1920.f, 0.f));
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
			window.draw(studentName);
		}
		else if (gameStatus == 1 || gameStatus == 3)
		{
			window.draw(background);

			if (HeatState == 1)
			{
				window.draw(preHeat);
				window.draw(countdownHeat);
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

			for (size_t i = 0; i < dmgBuff.size(); i++)
			{
				window.draw(dmgBuff[i]);
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
			window.draw(HpBarGraphic);

			window.draw(ArmorBar);
			window.draw(ArmorBarGraphic);

			window.draw(DodgeBar);

			window.draw(scoreIG);

			//If game over
			if (gameStatus == 3)
			{
				window.draw(gameOverScreen);

				if (gameOverScreen.getGlobalBounds().width >= window.getSize().x && gameOverScreen.getGlobalBounds().height >= window.getSize().y)
				{
					window.draw(gameOverText);
					for (int i = 0; i < 4; i++)
					{
						window.draw(gameOverOutput[i]);
					}
				}
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
