#pragma once
#include <easy2d/easy2d.h>
#include "resource.h"
#include "Monster.h"
#include "Star.h"
#include "Ground.h"

using namespace easy2d;

// ��Ϸ����
class MainScene : public Scene
{
private:
	Text*		scoreText = nullptr;
	Monster*	monster = nullptr;
	Button*		playButton = nullptr;
	Star*		star = nullptr;
	Ground*		ground = nullptr;
	int			score = 0;

public:
	MainScene()
	{
		// ����ͼ
		auto background = gcnew Sprite(IDR_JPG1, "JPG");
		this->addChild(background);

		// �÷��ı�
		scoreText = gcnew Text("Score: 0");
		scoreText->setAnchor(0.5f, 0);
		scoreText->setPos(Window::getWidth() / 2, 20);
		this->addChild(scoreText);

		// ����
		ground = gcnew Ground;
		this->addChild(ground);

		// ����
		monster = gcnew Monster;
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		this->addChild(monster);

		// ��ʼ��ť
		auto btnPlayImage = gcnew Sprite(IDB_PNG1, "PNG");

		playButton = gcnew Button(btnPlayImage);
		playButton->setScale(0.5f);
		playButton->setPos((Window::getSize() - playButton->getSize()) / 2);
		this->addChild(playButton);

		// �����ʼ��ť��Ļص�����
		auto func = std::bind(&MainScene::start, this);
		playButton->setClickFunc(func);
	}

	void start()
	{
		// ���ؿ�ʼ��ť
		playButton->setVisible(false);
		// ���õ÷�
		score = 0;
		scoreText->setText("Score: 0");
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
		auto call = gcnew CallFunc(std::bind(&MainScene::end, this));
		// ִ�ж���
		auto seq = gcnew Sequence;
		seq->add({ fadeOut, call });
		star->runAction(seq);

		this->addChild(star);
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
		auto call = gcnew CallFunc([=]()
			{
				playButton->setVisible(true);
			});

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
				MusicPlayer::play(IDR_WAVE2, "WAVE");
				// �÷ּ�һ
				score++;
				scoreText->setText("Score: " + std::to_string(score));
				// ����һ���µ�����
				this->newStar();
			}
		}
	}
};
