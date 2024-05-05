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
	int arrow = Player::GetInstance()->ArrowGun;
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
			InitBullet(playerListBullet[i], arrow, Player::GetInstance()->isFlip, size);
			listBullet.push_back(playerListBullet[i]);
		}
	}
}

void FuncItem::InitBullet(OBullet* bullet, int arrow, bool isFlip, int index, int itemType)
{
	int	acc = isFlip ? -1 : 1;
	int angleList[5] = { 0, 1, -1, 2, -2 };
	int arrowList[10] = { 0, 0, 0, 90, 0, 45, 0, -45 };

	bullet->Init(arrowList[arrow], acc, GetBulletTypeByItemType(itemType), angleList[index]);
}

int FuncItem::GetBulletTypeByItemType(int itemType)
{
	switch ((OItem::ItemType)itemType)
	{
	case OItem::M:
	case OItem::S:
		return OBullet::RedBullet;
	case OItem::F:
		return OBullet::FBullet;
	case OItem::L:
		return OBullet::LBullet;
	default:
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