#include <easy2d.h>
#include "resource.h"

// 怪兽
class Monster : public Sprite
{
private:
	float speed = 0;

public:
	// 最大速度
	const float maxSpeed = 6;
	// 加速度
	const float accel = 0.25f;
	// 摩擦力
	const float friction = 0.05f;
	// 跳跃高度
	const float jumpHeight = 150;
	// 跳跃时长
	const float jumpDuration = 0.6f;

public:
	Monster() : Sprite(IDB_PNG3, L"PNG")
	{
		this->setAnchor(0.5f, 1);
		this->setScale(0.5f);

		// 关闭 onUpdate 函数
		this->setAutoUpdate(false);
	}

	void start()
	{
		speed = 0;

		// 打开 onUpdate 函数
		this->setAutoUpdate(true);
		this->setScale(0.6f, 0.4f);

		// 创建拉伸动作，并播放跳跃声音
		auto call = gcnew CallFunc([]() { Player::play(IDR_WAVE1, L"WAVE"); });
		auto scale1 = gcnew ScaleTo(jumpDuration / 2, 0.4f, 0.6f);
		auto scale2 = gcnew ScaleTo(jumpDuration / 2, 0.6f, 0.4f);
		auto seq = gcnew Sequence;
		seq->add({ call, scale1, scale2 });

		// 创建跳跃动作
		auto jumpBy = gcnew JumpBy(jumpDuration, Point(), -jumpHeight);
		auto spawn = gcnew Spawn;
		spawn->add({ seq, jumpBy });

		// 循环执行两动作
		this->runAction(gcnew Loop(spawn));
	}

	void onUpdate()
	{
		// 按左右键时，增加指定方向上的速度
		if (Input::isDown(Input::Key::Left))
		{
			speed -= accel;
		}
		else if (Input::isDown(Input::Key::Right))
		{
			speed += accel;
		}
		
		// 限制最大速度
		if (speed > maxSpeed || speed < -maxSpeed)
		{
			speed = (speed > 0) ? maxSpeed : -maxSpeed;
		}

		// 怪兽碰到屏幕边缘时弹开
		float halfWidth = this->getWidth() / 2;
		if (this->getPosX() - halfWidth <= 0)
		{
			speed = -speed;
			// 防止怪兽反复横跳
			this->setPosX(halfWidth + 1);
		}
		else if (this->getPosX() + halfWidth >= Window::getWidth())
		{
			speed = -speed;
			// 防止怪兽反复横跳
			this->setPosX(Window::getWidth() - halfWidth - 1);
		}

		// 受摩擦力
		if (fabs(speed) <= 0.1)
		{
			speed = 0;
		}
		else
		{
			speed -= (speed > 0) ? friction : -friction;
		}

		// 横向移动怪兽位置
		this->movePosX(speed);
	}
};


// 星星
class Star : public Sprite
{
public:
	Star() : Sprite(IDB_PNG4, L"PNG")
	{
		this->setAnchor(0.5f, 0);
		this->setScale(0.5f);
	}
};


// 地面
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


// 游戏场景
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
		// 背景图
		auto background = gcnew Sprite(IDR_JPG1, L"JPG");
		this->add(background);

		// 得分文本
		scoreText = gcnew Text(L"Score: 0");
		scoreText->setAnchor(0.5f, 0);
		scoreText->setPos(Window::getWidth() / 2, 20);
		this->add(scoreText);

		// 地面
		ground = gcnew Ground;
		this->add(ground);

		// 怪兽
		monster = gcnew Monster;
		monster->setPosX(Window::getWidth() / 2);
		monster->setPosY(ground->getPosY() - ground->getHeight());
		this->add(monster);

		// 开始按钮
		auto btnPlayImage = gcnew Sprite(IDB_PNG1, L"PNG");

		playButton = gcnew Button(btnPlayImage);
		playButton->setScale(0.5f);
		playButton->setPos((Window::getSize() - playButton->getSize())/ 2);
		this->add(playButton);

		// 点击开始按钮后的回调函数
		auto func = std::bind(&GameScene::start, this);
		playButton->setClickFunc(func);
	}

	void start()
	{
		// 隐藏开始按钮
		playButton->setVisiable(false);
		// 重置得分
		score = 0;
		scoreText->setText(L"Score: 0");
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
		auto call = gcnew CallFunc(std::bind(&GameScene::end, this));
		// 执行动作
		auto seq = gcnew Sequence;
		seq->add({ fadeOut, call });
		star->runAction(seq);

		this->add(star);
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
		auto call = gcnew CallFunc([=]() { playButton->setVisiable(true); });

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
				Player::play(IDR_WAVE2, L"WAVE");
				// 得分加一
				score++;
				scoreText->setText(L"Score: " + std::to_wstring(score));
				// 产生一个新的星星
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