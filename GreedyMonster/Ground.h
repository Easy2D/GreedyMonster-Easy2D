#pragma once
#include <easy2d/easy2d.h>
#include "resource.h"

using namespace easy2d;

// µØÃæ
class Ground : public Sprite
{
public:
	Ground() : Sprite(IDB_PNG2, "PNG")
	{
		this->setScaleX(2);
		this->setScaleY(0.4f);
		this->setAnchor(0, 1);
		this->setPosY(Window::getHeight());
	}
};
