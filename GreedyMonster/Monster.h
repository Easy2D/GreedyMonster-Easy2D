#pragma once
#include <easy2d.h>
#include "resource.h"

using namespace easy2d;

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
		auto call = gcnew CallFunc([]() { MusicPlayer::play(IDR_WAVE1, L"WAVE"); });
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
		if (Input::isDown(Key::Left))
		{
			speed -= accel;
		}
		else if (Input::isDown(Key::Right))
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
