#pragma once
#include <easy2d/easy2d.h>
#include "resource.h"

using namespace easy2d;

// ����
class Star : public Sprite
{
public:
	Star() : Sprite(IDB_PNG4, "PNG")
	{
		this->setAnchor(0.5f, 0);
		this->setScale(0.5f);
	}
};
