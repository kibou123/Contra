﻿#include "PlayerCollision.h"
#include "Player.h"
#include "OWall.h"

PlayerCollision::PlayerCollision()
{
	player = Player::GetInstance();
	_functionMap[Object::Standing] = &PlayerCollision::StandCollision;
	_functionMap[Object::Running] = &PlayerCollision::RunCollision;
	_functionMap[Object::Sitting] = &PlayerCollision::StandCollision;
}

PlayerCollision::~PlayerCollision()
{
}

void PlayerCollision::OnCollision()
{
	//Check player collision by state
	PlayCollisionF();

	//CheckAllState
	switch (_obj->Tag)
	{
	case Object::Wall:
		CheckCollisionWall();
		break;
	default:

		break;
	}

	//Kiểm tra player rơi
	isGround = isGround || _side.y == Collision::BOTTOM;
	isCollisionTop = isCollisionTop || _side.y == Collision::TOP;
}


//Va chạm với Tường
void PlayerCollision::CheckCollisionWall(Object* _wall)
{
	if (_wall == nullptr) _wall = _obj;
	OWall* wall = dynamic_cast<OWall*>(_wall);
	switch (wall->_walltype)
	{
	default:
		break;
	}
}

//Chạy function ứng với state player
void PlayerCollision::PlayCollisionF()
{
	//Lấy Function từ player state
	FunctionMap::iterator it = _functionMap.find(player->State);
	//(it->second) là con trỏ hàm lấy từ <map> = &StandCollision
	(this->*(it->second))();	//	(this->*(it->second)) = this->StandCollision
}

//Va chạm khi đứng
void PlayerCollision::StandCollision()
{
}

//Va chạm khi chạy
void PlayerCollision::RunCollision()
{
	if (_side.x != Collision::NONE)
	{
		player->State = Object::Standing;
	}
}