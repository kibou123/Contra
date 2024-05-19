﻿#include "Map.h"
#include"TinyXML\tinyxml.h"
#include "Player.h"
#include "ObjectManager.h"
#include "Water.h"
#include "Bridge.h"

#pragma warning(disable : 4996)

Map::Map()
{
	info = new InfoMap(MapXML);

	this->tileset = new TileSet(info->tileCount, info->tileColumns, info->tileWidth, info->tileHeight);
	position = D3DXVECTOR2(0, 0);
	WidthMap = info->width * info->tileWidth;
	HeightMap = info->height * info->tileHeight;

	RECT boundMap = { 0, HeightMap, WidthMap, 0 };

	//Thêm vùng di chuyển của view
	Viewport* viewport = ObjectManager::GetInstance()->GetViewPort();
	viewport->RectView = {0, HeightMap, WidthMap, 0};

	ObjectManager::GetInstance()->GetViewPort()->RectView = boundMap;;

	//Tạo cây nhị phân theo bound map
	this->Tree = new BinaryTree(boundMap, HeightMap - WidthMap > 0);

	// Tạo object
	objectTag["Wall"] = OWall::Wall;
	objectTag["Water"] = OWall::Water;
	objectTag["Soldier"] = OEnemy::Soldier;
	objectTag["RifleMan"] = OEnemy::Rifleman;
	objectTag["R"] = OItem::R;
	objectTag["M"] = OItem::M;
	objectTag["S"] = OItem::S;
	objectTag["F"] = OItem::F;
	objectTag["L"] = OItem::L;
	objectTag["T"] = OItem::T;
	objectTag["Tank"] = OEnemy::Tank;
	objectTag["Cannon"] = OEnemy::Cannon;
	objectTag["Bridge"] = OWall::Brigde;

	for (int i = 0; i < info->numObjectGroups; i++)
	{
		for (int j = 0; j < info->ObjectGroups.at(i)->NumOnjects; j++)
		{
			MapObject* mapObject = info->ObjectGroups.at(i)->Objects.at(j);
			Object* obj = CreateObject(mapObject);
			//Thêm object vào cây nhị phân
			if (obj != NULL)
				this->Tree->insertObject(obj);
		}
	}


	this->Tree->LogAllObject();
}

Map::~Map()
{
	delete tileset;
	delete info;
	int size = ListObject.size();
	for (size_t i = 0; i < size; i++)
	{
		Object* obj = ListObject.at(i);
		if (obj->Tag == Object::Player) continue; //Không xóa player
		delete ListObject.at(i);
		obj = NULL;
	}
}


Object* Map::CreateObject(MapObject* _mapobject)
{
	D3DXVECTOR2 pos;
	pos.x = _mapobject->x + _mapobject->width / 2;
	pos.y = info->height * info->tileHeight - _mapobject->y - _mapobject->height / 2;

	Object* obj;
	Object::tag tagg = Object::GetTag(_mapobject->name);
	int objectType = objectTag[_mapobject->name];
	switch (tagg)
	{
	case Object::Player:
		obj = Player::GetInstance();
		obj->SetPositionStart(pos);
		break;
	case Object::Wall:
		switch (objectType)
		{
		case OWall::Wall:
			obj = new OWall();
			break;
		case OWall::Water:
			obj = new Water();
			break;
		case OWall::Brigde:
			obj = new Bridge();
			break;
		default:
			return NULL;
		}
		break;
	case Object::Enemy:
		switch (objectType)
		{
		case OEnemy::Soldier:
			obj = new OEnemy();
			break;
		case OEnemy::Rifleman:
			obj = new RifleMan();
			break;
		case OEnemy::Tank:
			obj = new Tank();
			break;
		case OEnemy::Cannon:
			obj = new Cannon();
			break;
		default:
			return NULL;
		}
		break;
	case Object::Item:
		objectType = objectTag[_mapobject->nameType];
		switch (objectType)
		{
		case OItem::R:
			obj = new OItem();
			break;
		case OItem::M:
			obj = new OItem();
			break;
		case OItem::S:
			obj = new OItem();
			break;
		case OItem::F:
			obj = new OItem();
			break;
		case OItem::L:
			obj = new OItem();
			break;
		default:
			return NULL;
		}
		break;
	default:
		return NULL;
	}
	obj->id = _mapobject->id;
	obj->Init(pos, objectType, _mapobject->kind);
	obj->SetPositionStart(pos);
	obj->GetBound(_mapobject->width, _mapobject->height);

	obj->SetName(_mapobject->name);

	ListObject.push_back(obj);
	return obj;
}

void Map::Render(Viewport * view)
{
	int startH, startW, endH, endW;
	RECT r = view->GetBoundViewport();
	startH = r.bottom / info->tileHeight;
	endH = r.top / info->tileHeight + 1;
	startW = r.left / info->tileWidth;
	endW = r.right / info->tileWidth + 1;

	for (int h = startH; h < endH; h++)
		for (int w = startW; w < endW; w++)
		{
			int id = info->GetData(w, h);
			if (id > 0)
			{
				D3DXVECTOR2 position(w * info->tileWidth, (h + 1) * info->tileHeight);
				tileset->Render(view, id, position);
			}
		}
}
