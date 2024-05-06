#include "FuncItem.h"
#include "Player.h"
#include "OItem.h"

FuncItem::FuncItem()
{

}
FuncItem::~FuncItem()
{

}

void FuncItem::CallFunc(int itemType)
{


	switch ((OItem::ItemType)itemType)
	{
	case OItem::R:
		Player::GetInstance()->_playerController->isReload = true;
		break;
	case OItem::M:
		Player::GetInstance()->GunType = itemType;
		Player::GetInstance()->maxBullet = 5;
		break;
	case OItem::S:
		Player::GetInstance()->GunType = itemType;
		Player::GetInstance()->maxBullet = 10;
		break;
	case OItem::F:
		Player::GetInstance()->GunType = itemType;
		Player::GetInstance()->maxBullet = 4;
		break;
	case OItem::L:
		Player::GetInstance()->GunType = itemType;
		Player::GetInstance()->maxBullet = 2;
		break;
	default:
		break;
	}
}
void FuncItem::GetBullet(std::vector<OBullet*>& listBullet)
{
	int maxBullet = Player::GetInstance()->maxBullet;
	vector <OBullet*> playerListBullet = Player::GetInstance()->ListBullet;

	int count = 0;
	for (size_t i = 0; i < playerListBullet.size(); i++)
	{
		if (playerListBullet[i]->IsFire)
		{
			count++;
		}
	}

	if (count >= maxBullet)
	{
		return;
	}

	OItem::ItemType itemType = (OItem::ItemType)Player::GetInstance()->GunType;
	int arrow = Player::GetInstance()->AngleGun;
	int numBullet = FuncItem::GetNumBulletByType(itemType) > (maxBullet - count) 
		? (maxBullet - count) : FuncItem::GetNumBulletByType(itemType);
	for (size_t i = 0; i < playerListBullet.size(); i++)
	{
		int size = listBullet.size();
		if (size >= numBullet)
		{
			break;
		}
		if (!playerListBullet[i]->IsFire)
		{
			InitBullet(playerListBullet[i], arrow, Player::GetInstance()->isFlip, size, itemType);
			listBullet.push_back(playerListBullet[i]);
		}
	}
}

void FuncItem::InitBullet(OBullet* bullet, int arrow, bool isFlip, int index, int itemType)
{
	int angleList[5] = { 0, 1, -1, 2, -2 };
	float acc = isFlip ? -1 : 1;
	int damage = 1;

	int bulletType = GetBulletTypeByItemType(itemType, acc, damage);
	bullet->Init(arrow, acc, bulletType, angleList[index]);
	bullet->Damage = damage;
}

int FuncItem::GetBulletTypeByItemType(int itemType, float& acc, int& damage)
{
	switch ((OItem::ItemType)itemType)
	{
	case OItem::M:
		acc *= 3;
		return OBullet::RedBullet;
	case OItem::S:
		acc *= 1.5;
		return OBullet::RedBullet;
	case OItem::F:
		damage = 2;
		return OBullet::FBullet;
	case OItem::L:
		damage = 4;
		acc *= 2;
		return OBullet::LBullet;
	default:
		acc *= 1.2;
		return OBullet::NormalBullet;
	}
}

int FuncItem::GetNumBulletByType(int itemType)
{
	switch ((OItem::ItemType)itemType)
	{
	case OItem::S:
		return 5;
	default:
		break;
	}
	return 1;
}