#include <easy2d.h>
#include "resource.h"

// ����
class Monster : public Sprite
{
private:
	float speed = 0;

public:
	// ����ٶ�
	const float maxSpeed = 6;
	// ���ٶ�
	const float accel = 0.25f;
	// Ħ����
	const float friction = 0.05f;
	// ��Ծ�߶�
	const float jumpHeight = 150;
	// ��Ծʱ��
	const float jumpDuration = 0.6f;

public:
	Monster() : Sprite(IDB_PNG3, L"PNG")
	{
		this->setAnchor(0.5f, 1);
		this->setScale(0.5f);

		// �ر� onUpdate ����
		this->setAutoUpdate(false);
	}

	void start()
	{
		speed = 0;

		// �� onUpdate ����
		this->setAutoUpdate(true);
		this->setScale(0.6f, 0.4f);

		// �������춯������������Ծ����
		auto call = gcnew CallFunc([]() { Player::play(IDR_WAVE1, L"WAVE"); });
		auto scale1 = gcnew ScaleTo(jumpDuration / 2, 0.4f, 0.6f);
		auto scale2 = gcnew ScaleTo(jumpDuration / 2, 0.6f, 0.4f);
		auto seq = gcnew Sequence;
		seq->add({ call, scale1, scale2 });

		// ������Ծ����
		auto jumpBy = gcnew JumpBy(jumpDuration, Point(), -jumpHeight);
		auto spawn = gcnew Spawn;
		spawn->add({ seq, jumpBy });

		// ѭ��ִ��������
		this->runAction(gcnew Loop(spawn));
	}

	void onUpdate()
	{
		// �����Ҽ�ʱ������ָ�������ϵ��ٶ�
		if (Input::isDown(Input::Key::Left))
		{
			speed -= accel;
		}
		else if (Input::isDown(Input::Key::Right))
		{
			speed += accel;
		}
		
		// ��������ٶ�
		if (speed > maxSpeed || speed < -maxSpeed)
		{
			speed = (speed > 0) ? maxSpeed : -maxSpeed;
		}

		// ����������Ļ��Եʱ����
		float halfWidth = this->getWidth() / 2;
		if (this->getPosX() - halfWidth <= 0)
		{
			speed = -speed;
			// ��ֹ���޷�������
			this->setPosX(halfWidth + 1);
		}
		else if (this->getPosX() + halfWidth >= Window::getWidth())
		{
			speed = -speed;
			// ��ֹ���޷�������
			this->setPosX(Window::getWidth() - halfWidth - 1);
		}

		// ��Ħ����
		if (fabs(speed) <= 0.1)
		{
			speed = 0;
		}
		else
		{
			speed -= (speed > 0) ? friction : -friction;
		}

		// �����ƶ�����λ��
		this->movePosX(speed);
	}
};


// ����
class Star : public Sprite
{
public:
	Star() : Sprite(IDB_PNG4, L"PNG")
	{
		this->setAnchor(0.5f, 0);
		this->setScale(0.5f);
	}
};


// ����
class Ground : public Sprite
{
public:
	Ground() : Sprite(IDB_PNG2, L"PNG")
	{
		this->setScaleX(2);
		this->setScaleY(0.4f);
		this->setAnchor(0, 1);
		this->setPosY(Window::getHeight());
	}
};


// ��Ϸ����
class GameScene : public Scene
{
private:
	Text *		scoreText	= nullptr;
	Monster *	monster		= nullptr;
	Button *	playButton	= nullptr;
	Star *		star		= nullptr;
	Ground *	ground		= nullptr;
	int			score		= 0;

public:
	GameScene()
	{
		// ����ͼ
		auto background = gcnew Sprite(IDR_JPG1, L"JPG");
		this->add(background);

		// �÷��ı�
		scoreText = gcnew Text(L"Score: 0");
		scoreText->setAnchor(0.5f, 0);
		scoreText->setPos(Window::getWidth() / 2, 20);
		this->add(scoreText);

		// ����
		ground = gcnew Ground;
		this->add(ground);

		// ����
		monster = gcnew Monster;
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		this->add(monster);

		// ��ʼ��ť
		auto btnPlayImage = gcnew Sprite(IDB_PNG1, L"PNG");

		playButton = gcnew Button(btnPlayImage);
		playButton->setScale(0.5f);
		playButton->setPos((Window::getSize() - playButton->getSize())/ 2);
		this->add(playButton);

		// �����ʼ��ť��Ļص�����
		auto func = std::bind(&GameScene::start, this);
		playButton->setClickFunc(func);
	}

	void start()
	{
		// ���ؿ�ʼ��ť
		playButton->setVisiable(false);
		// ���õ÷�
		score = 0;
		scoreText->setText(L"Score: 0");
		// ���ù���λ��
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		// ���޿�ʼ��Ծ
		monster->start();
		// ��������
		newStar();
	}

	// ����������
	void newStar()
	{
		// �������λ��
		Point pos = randomStarPos();

		// �Ƴ�ԭ����
		if (star) star->removeFromParent();

		// ����������
		star = gcnew Star;
		star->setPos(pos);
		// ������ 2 �� 3 ������ʧ
		auto fadeOut = gcnew FadeOut(Random::range(2.f, 3.f));
		// ������ʧ��ִ�� GameScene::end ����
		auto call = gcnew CallFunc(std::bind(&GameScene::end, this));
		// ִ�ж���
		auto seq = gcnew Sequence;
		seq->add({ fadeOut, call });
		star->runAction(seq);

		this->add(star);
	}

	// �������λ��
	Point randomStarPos()
	{
		float randX = 0;
		// ���ݵ�ƽ��λ�ú�������Ծ�߶ȣ�����õ�һ�����ǵ� y ����
		float randY = Window::getHeight() - ground->getHeight() - Random::range(0.f, 1.f) * monster->jumpHeight - 50;
		// ������Ļ��Ⱥ���һ�����ǵ� x ���꣬����õ�һ������������ x ����
		float maxX = Window::getWidth() / 2;
		float currentX = star == nullptr ? Random::range(-1.f, 1.f) * maxX : star->getPosX() - maxX;
		if (currentX >= 0) 
		{
			randX = -Random::range(0.f, 1.f) * (maxX - 50);
		}
		else
		{
			randX = Random::range(0.f, 1.f) * (maxX - 50);
		}
		return Point(randX + maxX, randY);
	}

	// ��Ϸ����
	void end()
	{
		// ֹͣ���޵��ƶ�
		monster->setAutoUpdate(false);
		monster->stopAllActions();
		// �ù������������·�
		auto jumpBy = gcnew JumpBy(0.6f, Point(0, Window::getHeight()), -150);
		auto call = gcnew CallFunc([=]() { playButton->setVisiable(true); });

		auto seq = gcnew Sequence;
		seq->add({ jumpBy, call });
		monster->runAction(seq);
	}

	void onUpdate()
	{
		if (star && monster)
		{
			// �жϹ����Ƿ���������ײ
			if (monster->getBoundingBox().intersects(star->getBoundingBox()))
			{
				// ������Ч
				Player::play(IDR_WAVE2, L"WAVE");
				// �÷ּ�һ
				score++;
				scoreText->setText(L"Score: " + std::to_wstring(score));
				// ����һ���µ�����
				this->newStar();
			}
		}
	}
};

int main()
{
	if (Game::init())
	{
		Window::setTitle(L"Greedy Monster");
		Window::setSize(512, 362);
		Logger::showConsole(false);

		Path::add(L"res/images/");
		Path::add(L"res/audio/");

		Player::preload(IDR_WAVE1, L"WAVE");
		Player::preload(IDR_WAVE2, L"WAVE");

		auto scene = gcnew GameScene;
		SceneManager::enter(scene);

		Game::start();
	}
	Game::destroy();
	return 0;
}