
#include "Robert.h"

using namespace std;

class Player : public sf::Sprite
{
public:
	float hp;
	float speed;
	bool hitted;
	float hitResetTimer;
	float deathTimer;

	Player() : hp(100.f), speed(300.f), hitted(false), hitResetTimer(0.f), deathTimer(0.f)
	{

	}
};

class PepsiEnemy : public sf::Sprite
{
public:
	sf::Vector2f direction;
	float speed;
	float damage;

	PepsiEnemy() : speed(700.f), damage(25.f)
	{

	}
};

struct WASD
{
	WASD() :
		wPressed(false),
		aPressed(false),
		sPressed(false),
		dPressed(false)
	{
		
	}

	bool wPressed;
	bool aPressed;
	bool sPressed;
	bool dPressed;
};

void moveSprite(const WASD &wasd, sf::Sprite &sprite, const float &deltaTime)
{
	if (wasd.wPressed)
		sprite.move(0, -300.f * deltaTime);
	if (wasd.sPressed)
		sprite.move(0, 300.f * deltaTime);
	if (wasd.aPressed)
		sprite.move(-300.f * deltaTime, 0);
	if (wasd.dPressed)
		sprite.move(300.f * deltaTime, 0);
}

int main()
{
	bool gameOver = false;

	float enemySpawnTimer = 0.f;

	default_random_engine randomEngine;

	uniform_int_distribution<int> spriteDistX(-100, 1124);

	WASD wasd;
	sf::RenderWindow window(sf::VideoMode(1024, 720), L"вікно");

	sf::Clock clock;

	sf::Texture textureDog;
	textureDog.loadFromFile("dog.jpg");
	sf::Texture texturePepsi;
	texturePepsi.loadFromFile("pepsi.png");

	Player dog;
	dog.setTexture(textureDog);

	dog.setScale(0.3f, 0.3f);
	dog.setOrigin(dog.getTextureRect().width / 2, dog.getTextureRect().height / 2);
	dog.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	vector<PepsiEnemy> pepsis;

	while (window.isOpen()) 
	{
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::W && !gameOver)
					wasd.wPressed = true;
				if (event.key.code == sf::Keyboard::S && !gameOver)
					wasd.sPressed = true;
				if (event.key.code == sf::Keyboard::A && !gameOver)
					wasd.aPressed = true;
				if (event.key.code == sf::Keyboard::D && !gameOver)
					wasd.dPressed = true;
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
			}
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::W)
					wasd.wPressed = false;
				if (event.key.code == sf::Keyboard::S)
					wasd.sPressed = false;
				if (event.key.code == sf::Keyboard::A)
					wasd.aPressed = false;
				if (event.key.code == sf::Keyboard::D)
					wasd.dPressed = false;
			}
		}

		for (size_t i = pepsis.size(); i < 10 && enemySpawnTimer >= 0.6f && !gameOver; i++)
		{
			PepsiEnemy pepsi;
			pepsi.setTexture(texturePepsi);
			pepsi.setScale(0.15f, 0.15f);
			pepsi.setPosition(spriteDistX(randomEngine), -100);
			pepsi.direction = dog.getPosition() - pepsi.getPosition();
			
			while (abs(pepsi.direction.x) > 1.f)
				pepsi.direction = sf::Vector2f(pepsi.direction.x / 10.f, pepsi.direction.y);

			while (abs(pepsi.direction.y) > 1.f)
				pepsi.direction = sf::Vector2f(pepsi.direction.x, pepsi.direction.y / 10.f);

			float rotation = 180.f * asin(pow(pow(pepsi.direction.x, 2) + pow(pepsi.direction.y, 2), 0.5)) / 3.1415;

			pepsi.setRotation((pepsi.direction.x < 0.f ? rotation : rotation * -1));

			pepsis.push_back(pepsi);
			enemySpawnTimer = 0.f;
		}

		enemySpawnTimer += deltaTime;

		moveSprite(wasd, dog, deltaTime);

		for (int i = 0; i < pepsis.size(); i++)
		{
			pepsis[i].move(pepsis[i].direction * pepsis[i].speed * deltaTime);

			if (dog.getGlobalBounds().intersects(pepsis[i].getGlobalBounds()))
			{
				dog.hitted = true;
				dog.setColor(sf::Color::Red);

				dog.hp -= pepsis[i].damage;
				cout << ANSI_COLOR_RED << dog.hp << ANSI_COLOR_RESET << endl;

				pepsis.erase(pepsis.begin() + i);
			}
		}

		if (dog.hitted)
		{
			dog.hitResetTimer += deltaTime;
			dog.setColor(sf::Color(255, 255.f * dog.hitResetTimer / 0.2f, 255.f * dog.hitResetTimer / 0.2f));

			if (dog.hitResetTimer >= 0.2f)
			{
				dog.hitResetTimer = 0.f;
				dog.hitted = false;
			}
		}

		if (dog.hp <= 0.f) 
		{
			if (dog.deathTimer >= 2.f)
			{
				dog.setPosition(-100, -100);
			}
			else
			{
				dog.deathTimer += deltaTime;
				dog.setColor(sf::Color(255, 255, 255, 255 - 255.f * dog.deathTimer / 2.f));
			}

			gameOver = true;
		}

		window.clear(sf::Color(255, 255, 255));

		window.draw(dog);

		for (auto& pepsi : pepsis)
		{
			window.draw(pepsi);
		}

		window.display();
	}

	return 0;
}
