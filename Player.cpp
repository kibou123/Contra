﻿#include "Player.h"
#include "GameDefine.h"
#include <math.h>
#include "GUI.h"
#include "ObjectManager.h"
#include "Obullet.h"
#pragma warning(disable : 4996)

Player* Player::_player = nullptr;
Player::Player()
{
}

Player* Player::GetInstance()
{
	if (nullptr == _player) {
		_player = new Player();
		_player->_life = StartLive;
		_player->Init();
	}
	return _player;
}

Player::~Player()
{
	delete _playerController;
	delete _playerCollision;
	delete _player;
}

Animation::DataAnimMap dataM()
{

	// 1: bắn bình thường
	// 2: giơ súng lên
	// 3: giơ súng lên rồi bắn
	// 4: giơ súng chéo lên
	// 5: giơ súng chéo lên rồi bắn
	// 6: giơ súng chéo xuống
	// 7: giơ súng chéo xuống rồi bắn
	// 
	//Tạo Animation
	Animation::DataAnimMap data;
	//Small
	data[Player::Blue + Object::Standing] = { 48, 48 };
	data[Player::Blue + Object::Standing + 1] = { 49, 49 }; // đứng + bắn
	data[Player::Blue + Object::Standing + 2] = { 12, 12 }; // đứng + up
	data[Player::Blue + Object::Standing + 3] = { 13, 13 }; // đứng + up + bắn

	data[Player::Blue + Object::Running] = { 0, 5 }; // // Không súng
	data[Player::Blue + Object::Running + 1] = { 6, 11 }; // súng + ngang
	data[Player::Blue + Object::Running + 4] = { 16, 18 }; //  súng + chéo + up
	data[Player::Blue + Object::Running + 5] = { 19, 21 }; //  súng + chéo + up + bắn
	data[Player::Blue + Object::Running + 6] = { 22, 24 }; //  súng + chéo + down
	data[Player::Blue + Object::Running + 7] = { 25, 27 }; //  súng + chéo + down + bắn


	data[Player::Blue + Object::Jumping] = { 28, 31 };
	data[Player::Blue + Object::Jumping + 1] = { 28, 28 }; // nhảy + bắn

	data[Player::Blue + Object::Sitting] = { 14, 14 }; // nằm
	data[Player::Blue + Object::Sitting + 1] = { 15, 15 }; // nằm bắn


	data[Player::Blue + Object::Dying] = { 44, 47  };
	data[Player::Blue + Object::Diving ] = { 41, 42 }; // lặn 


	data[Player::Blue + Object::Swimming] = { 33, 34 }; // Bơi
	data[Player::Blue + Object::Swimming + 1] = { 35, 36 };// Bơi + súng
	data[Player::Blue + Object::Swimming + 2] = { 39, 39 };// Bơi + súng + thẳng
	data[Player::Blue + Object::Swimming + 3] = { 40, 40 };// Bơi + súng + thẳng + bắn
	data[Player::Blue + Object::Swimming + 4] = { 37, 37 };// Bơi + súng + chéo 
	data[Player::Blue + Object::Swimming + 5] = { 38, 38 };// Bơi + súng + chéo + bắn

	data[Player::Blue + Object::Falling] = {3, 3 };

	return data;
}

void Player::Init()
{
	//Tạo class xử lý va chạm
	_playerController = new PlayerController();
	_playerCollision = new PlayerCollision();

	AllowDraw = true;
	Tag = Object::Player;
	_playerType = Player::Blue;
	position = positionStart;
	velocity = D3DXVECTOR2(0, 0);
	StartJump(-10);
	HP = 1;
	maxBullet = 4;
	GunType = 0;

	type = _playerType;
	Animation::DataAnimMap data = dataM();
	_anim = new Animation(PlayerXML, PlayerPNG);
	_anim->SetDataAnimation(data);
	SetBound(38, 48);
}

void Player::BeforeUpdate(float gameTime, Keyboard* key)
{
	_playerCollision->isGround = false;

	this->SetBound(Width, Height);
	//Check handler controller
	_playerController->Update(gameTime, key);
}

void Player::OnCollision(Object* obj, float gameTime)
{
	Object::OnCollision(obj, gameTime);
	if (obj->State == Object::Dying || this->State == Object::Dying)
		return;
}

D3DXVECTOR2 Player::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	_playerCollision->_obj = obj;
	_playerCollision->_side = side;
	_playerCollision->OnCollision();
	return _playerCollision->_side;
}

void Player::OnCollision(Object* obj)
{
	
}

void Player::Update(float gameTime, Keyboard* key)
{
	if (!_playerCollision->isGround && State != Object::Jumping) State = Object::Falling;

	//Update Animation
	UpdateAnimation(gameTime);

	Object::Update(gameTime, key);

	for (size_t i = 0; i < ListBullet.size(); i++)
	{
		if (abs(ListBullet.at(i)->positionStart.x - ListBullet.at(i)->GetPosition().x) > GameWidth)
		{
			ListBullet.at(i)->Reset();
		}
	}
}

void Player::UpdateAnimation(float gameTime)
{
	_anim->NewAnimationByIndex(_playerType + this->State + (_playerController->isAttack ? ArrowGun : 0));
	_anim->SetPosition(D3DXVECTOR2(position.x, position.y + Height / 2));
	_anim->SetFlipFlag(isFlip);
	_anim->Update(gameTime);
}

void Player::SetImmortal(float time)
{
	immortalTime = time;
	isImmortal = true;
}

void Player::SetBound(float width, float height)
{
	//InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();
	Width = width;
	Height = height;
	bound.left = position.x - Width / 2;
	bound.right = bound.left + Width;
	bound.top = position.y + Height;
	bound.bottom = position.y;
}

void Player::Render(Viewport* viewport)
{
	//Vẽ Player
	if (AllowDraw)
	{
		_anim->Render(viewport);
	}

	GUI::GetInstance()->Render("State: ", { 150, 200, 200, 235 });
	GUI::GetInstance()->Render(State, { 210, 200, 230, 235 });
}
