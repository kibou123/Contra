#pragma once
#include "OBullet.h"

class FuncItem
{
public:
	FuncItem();
	~FuncItem();

	static void CallFunc(int itemType);
	static void GetBullet(std::vector<OBullet*>& listBullet);
	static int GetNumBulletByType(int itemType);
	static int GetBulletTypeByItemType(int itemType, float& acc, int& damage);
	static void InitBullet(OBullet* bullet, int arrow, bool isFlip, int index, int itemType = 0);
};

