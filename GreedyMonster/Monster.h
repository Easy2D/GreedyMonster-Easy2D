#pragma once
#include <easy2d.h>
#include "resource.h"

using namespace easy2d;

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
		auto call = gcnew CallFunc([]() { MusicPlayer::play(IDR_WAVE1, L"WAVE"); });
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
		if (Input::isDown(Key::Left))
		{
			speed -= accel;
		}
		else if (Input::isDown(Key::Right))
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
