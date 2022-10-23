#pragma once
#include <easy2d/easy2d.h>
#include "resource.h"
#include "Monster.h"
#include "Star.h"
#include "Ground.h"

using namespace easy2d;

// 游戏场景
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
		// 背景图
		auto background = gcnew Sprite(IDR_JPG1, "JPG");
		this->addChild(background);

		// 得分文本
		scoreText = gcnew Text("Score: 0");
		scoreText->setAnchor(0.5f, 0);
		scoreText->setPos(Window::getWidth() / 2, 20);
		this->addChild(scoreText);

		// 地面
		ground = gcnew Ground;
		this->addChild(ground);

		// 怪兽
		monster = gcnew Monster;
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		this->addChild(monster);

		// 开始按钮
		auto btnPlayImage = gcnew Sprite(IDB_PNG1, "PNG");

		playButton = gcnew Button(btnPlayImage);
		playButton->setScale(0.5f);
		playButton->setPos((Window::getSize() - playButton->getSize()) / 2);
		this->addChild(playButton);

		// 点击开始按钮后的回调函数
		auto func = std::bind(&MainScene::start, this);
		playButton->setClickFunc(func);
	}

	void start()
	{
		// 隐藏开始按钮
		playButton->setVisible(false);
		// 重置得分
		score = 0;
		scoreText->setText("Score: 0");
		// 重置怪兽位置
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		// 怪兽开始跳跃
		monster->start();
		// 生成星星
		newStar();
	}

	// 生成新星星
	void newStar()
	{
		// 随机星星位置
		Point pos = randomStarPos();

		// 移除原星星
		if (star) star->removeFromParent();

		// 生成新星星
		star = gcnew Star;
		star->setPos(pos);
		// 星星在 2 至 3 秒内消失
		auto fadeOut = gcnew FadeOut(Random::range(2.f, 3.f));
		// 星星消失后执行 GameScene::end 函数
		auto call = gcnew CallFunc(std::bind(&MainScene::end, this));
		// 执行动作
		auto seq = gcnew Sequence;
		seq->add({ fadeOut, call });
		star->runAction(seq);

		this->addChild(star);
	}

	// 随机星星位置
	Point randomStarPos()
	{
		float randX = 0;
		// 根据地平面位置和主角跳跃高度，随机得到一个星星的 y 坐标
		float randY = Window::getHeight() - ground->getHeight() - Random::range(0.f, 1.f) * monster->jumpHeight - 50;
		// 根据屏幕宽度和上一个星星的 x 坐标，随机得到一个新生成星星 x 坐标
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

	// 游戏结束
	void end()
	{
		// 停止怪兽的移动
		monster->setAutoUpdate(false);
		monster->stopAllActions();
		// 让怪兽跳到地面下方
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
			// 判断怪兽是否与星星碰撞
			if (monster->getBoundingBox().intersects(star->getBoundingBox()))
			{
				// 播放音效
				MusicPlayer::play(IDR_WAVE2, "WAVE");
				// 得分加一
				score++;
				scoreText->setText("Score: " + std::to_string(score));
				// 产生一个新的星星
				this->newStar();
			}
		}
	}
};
