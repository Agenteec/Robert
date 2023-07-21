
#include "Robert.h"

using namespace std;



class Player : public sf::Sprite
{
public:
	// Скорость следования камеры за персонажем
	float cameraSpeed;
	float hp;
	float maxHp;
	float speed;
	bool hitted;
	float hitResetTimer;
	float deathTimer;

	Player() :
		hp(100.f),
		maxHp(100.f),
		speed(300.f),
		hitted(false), 
		hitResetTimer(0.f), 
		deathTimer(0.f),
		cameraSpeed(0.8f)
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
class Rocket : public PepsiEnemy
{
public:
	b2BodyDef bodyDef;
	b2Body* body;
	b2Fixture* fixture;
	float liveTime;

	void initBody(b2World * world, sf::Vector2f pos,sf::Vector2f size)
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(pos.x/SCALE, pos.y/SCALE);
		body = world->CreateBody(&bodyDef);
		b2PolygonShape boxShape;
		boxShape.SetAsBox(size.x / 2.f / SCALE, size.y / 2.f / SCALE);
		fixture = body->CreateFixture(&boxShape, 1.0f);
	}
	
	Rocket() :PepsiEnemy(), liveTime(5.f)
	{

	}

	void update()
	{
		b2Vec2 pos = body->GetPosition();
		setPosition(pos.x*SCALE, pos.y*SCALE);
		setRotation(body->GetAngle() * 180.f / 3.14);
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
// Функция для нормализации вектора
sf::Vector2f normalize(const sf::Vector2f& vector) {
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length != 0.0f) {
		return sf::Vector2f(vector.x / length, vector.y / length);
	}
	return vector;
}
void setHpText(sf::Text& text, const float& hp, const float& maxHp)
{
	if (hp < 0)
	{
		return;
	}
	float col = 255.f * hp / maxHp;
	text.setFillColor(sf::Color(255 -col, col, 0));
	text.setString("HP: " + to_string(static_cast<int>(hp)));
}
int main()
{

#pragma region b2d
	// Создание мира Box2D без гравитации
	b2Vec2 gravity(0.0f, 9.8f-9.8f);
	b2World* world = new b2World(gravity);

	// Создание тела и формы первого прямоугольника
	b2BodyDef bodyDef1;
	bodyDef1.type = b2_dynamicBody;
	bodyDef1.position.Set(2.0f, 10.0f);
	b2Body* body1 = world->CreateBody(&bodyDef1);

	b2PolygonShape boxShape1;
	boxShape1.SetAsBox(1.0f, 1.0f);
	b2Fixture* fixture1 = body1->CreateFixture(&boxShape1, 1.0f);

	// Создание тела и формы второго прямоугольника
	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(3.5f, 14.0f);
	b2Body* body2 = world->CreateBody(&bodyDef2);

	b2PolygonShape boxShape2;
	boxShape2.SetAsBox(1.0f, 1.0f);
	b2Fixture* fixture2 = body2->CreateFixture(&boxShape2, 1.0f);

#pragma endregion

#pragma region b2dWalls
	// Определение статических тел для стен
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// Определение форм границ мира (стены)
	b2EdgeShape groundBox;

	// Верхняя граница
	groundBox.SetTwoSided(b2Vec2(-100.0f, 0.0f), b2Vec2(100.0f, 0.0f));
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Нижняя граница
	groundBox.SetTwoSided(b2Vec2(-100.0f, 50.0f), b2Vec2(100.0f, 50.0f));
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Левая граница
	groundBox.SetTwoSided(b2Vec2(-100.0f, 0.0f), b2Vec2(-100.0f, 50.0f));
	groundBody->CreateFixture(&groundBox, 0.0f);

	// Правая граница
	groundBox.SetTwoSided(b2Vec2(100.0f, 0.0f), b2Vec2(100.0f, 50.0f));
	groundBody->CreateFixture(&groundBox, 0.0f);
#pragma endregion
	sf::Font font;
	font.loadFromFile("Resources/fonts/impact.ttf");
	sf::Text hpText;
	hpText.setFont(font);
	hpText.setCharacterSize(20);
	

	bool gameOver = false;

	float enemySpawnTimer = 0.f;

	default_random_engine randomEngine;

	uniform_int_distribution<int> spriteDistX(-100, 1124);

	WASD wasd;
	sf::RenderWindow window(sf::VideoMode(1024, 720), L"вікно");
	hpText.setPosition(window.getSize().x-100,20);


	sf::Texture textureDog;
	textureDog.loadFromFile("Resources/png/entitys/dog.png");
	sf::Texture texturePepsi;
	texturePepsi.loadFromFile("Resources/png/entitys/pepsi.png");

	Player dog;
	dog.setTexture(textureDog);

	dog.setScale(0.3f, 0.3f);
	dog.setOrigin(dog.getTextureRect().width / 2, dog.getTextureRect().height / 2);
	dog.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	vector<PepsiEnemy> pepsis;
	sf::Clock clock;
	setHpText(hpText,dog.hp,dog.maxHp);
	Rocket rocketPepsi;
	rocketPepsi.setTexture(texturePepsi);
	rocketPepsi.setScale(0.15f, 0.15f);
	rocketPepsi.initBody(world, sf::Vector2f(1000, 500), sf::Vector2f(rocketPepsi.getLocalBounds().width * rocketPepsi.getScale().x, rocketPepsi.getLocalBounds().height * rocketPepsi.getScale().y));
	printf("%f", rocketPepsi.getLocalBounds().height);
	// Создаем вид (камеру) для окна
	sf::View view = window.getDefaultView();
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

		// Проверка столкновения двух прямоугольников
		b2ContactEdge* contactEdge = body1->GetContactList();
		while (contactEdge) {
			if (contactEdge->contact->IsTouching()) {
				//std::cout << "Collision detected!" << std::endl;
			}
			contactEdge = contactEdge->next;
		}


		for (size_t i = pepsis.size(); 0&&i < 30 && enemySpawnTimer >= 0.6f && !gameOver; i++)
		{
			PepsiEnemy pepsi;
			pepsi.setTexture(texturePepsi);
			pepsi.setScale(0.15f, 0.15f);
			pepsi.setPosition(spriteDistX(randomEngine), -100);
			if(i % 7 == 0)
			{
				pepsi.setColor(sf::Color::Green);
				pepsi.damage = -25;
				pepsi.speed = 500;
			}

			sf::Vector2f direction = normalize(dog.getPosition() - pepsi.getPosition());

			float rotation = std::atan2(direction.y, direction.x) * 180.f / 3.1415f;
			pepsi.setRotation(rotation+90.f);

			pepsi.direction = direction;

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
				if (dog.hp>dog.maxHp)
				{
					dog.hp = dog.maxHp;
				}
				setHpText(hpText, dog.hp, dog.maxHp);
				pepsis.erase(pepsis.begin() + i);
			}
		}
		b2Vec2 bodyPos = rocketPepsi.body->GetPosition();
		//cout << bodyPos.x << " " << bodyPos.y << endl;
		sf::Vector2f direction = normalize(sf::Vector2f(dog.getPosition().x - bodyPos.x*SCALE, dog.getPosition().y - bodyPos.y* SCALE));
		rocketPepsi.body->ApplyLinearImpulseToCenter(b2Vec2(direction.x*deltaTime*100.f, direction.y*deltaTime*100.f),true);
		float rotation = std::atan2(direction.y, direction.x);
		rocketPepsi.body->SetTransform(rocketPepsi.body->GetPosition(), rotation - 3.14f / 2.f);
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
				dog.move(sf::Vector2f(0,-10)*deltaTime);
				dog.setColor(sf::Color(255, 255, 255, 255 - 255.f * dog.deathTimer / 2.f));
			}

			gameOver = true;
		}

		rocketPepsi.update();
		// Обновление мира Box2D
		world->Step(deltaTime*0.1f, 6, 2);
#pragma region Camera
		// Вычисляем новую позицию камеры с интерполяцией
		sf::Vector2f targetPosition = dog.getPosition();
		sf::Vector2f currentPosition = view.getCenter();
		sf::Vector2f newPosition = currentPosition + (targetPosition - currentPosition) * dog.cameraSpeed*deltaTime;

		view.setCenter(newPosition);
		window.setView(view);
#pragma endregion

		
		////////////
		window.clear(sf::Color(255, 255, 255));
		
		window.draw(dog);
		#pragma region box2d
		sf::RectangleShape rect1(sf::Vector2f(2.0f * SCALE, 2.0f * SCALE));
		rect1.setOrigin(rect1.getSize()/2.f);
		rect1.setRotation(body1->GetAngle()/3.14f*180.f);
		rect1.setPosition(body1-> GetPosition().x* SCALE, body1->GetPosition().y* SCALE);
		rect1.setFillColor(sf::Color::Red);

		sf::RectangleShape rect2(sf::Vector2f(2.0f * SCALE, 2.0f * SCALE));
		rect2.setOrigin(rect2.getSize() / 2.f);
		rect2.setRotation(body2->GetAngle() / 3.14f * 180.f);
		rect2.setPosition(body2->GetPosition().x * SCALE, body2->GetPosition().y * SCALE);
		rect2.setFillColor(sf::Color::Blue);

		sf::RectangleShape wallShape1(sf::Vector2f(200.0f*SCALE, 5.0f));
		wallShape1.setPosition(-100.0f * SCALE, 0.0f);
		wallShape1.setFillColor(sf::Color::Green);
		window.draw(wallShape1);

		sf::RectangleShape wallShape2(sf::Vector2f(200.0f * SCALE, 5.0f));
		wallShape2.setPosition(-100.0f * SCALE, 50.0f * SCALE);
		wallShape2.setFillColor(sf::Color::Green);
		window.draw(wallShape2);

		sf::RectangleShape wallShape3(sf::Vector2f(5.0f, 50.0f * SCALE));
		wallShape3.setPosition(-100.0f * SCALE, 0.0f);
		wallShape3.setFillColor(sf::Color::Green);
		window.draw(wallShape3);

		sf::RectangleShape wallShape4(sf::Vector2f(5.0f, 50.0f * SCALE));
		wallShape4.setPosition(100.0f * SCALE - 5.0f, 0.0f);
		wallShape4.setFillColor(sf::Color::Green);
		window.draw(wallShape4);
		#pragma endregion
		window.draw(rocketPepsi);
		//window.draw(rect1);
		//window.draw(rect2);
		window.draw(hpText);
		for (auto& pepsi : pepsis)
		{
			window.draw(pepsi);
		}

		window.display();
		//////////
	}

	return 0;
}
