#include "OEnemy.h"
#include "Player.h"

Animation* OEnemy::GetAnimationEnemy()
{
	//Tạo Animation
	Animation::DataAnimMap data;
	// Soldier
	data[OEnemy::Soldier + Object::Running] = { 0 , 5};
	data[OEnemy::Soldier + Object::Dying] = { 7 , 8 };
	data[OEnemy::Rifleman + Object::Standing] = { 11, 12 };
	data[OEnemy::Rifleman + Object::Attacking ] = { 11, 12 };
	data[OEnemy::Rifleman + Object::Attacking + 5] = { 13, 14 };
	data[OEnemy::Rifleman + Object::Attacking + 7] = { 15, 15 };
	data[OEnemy::Rifleman + Object::Diving] = { 16, 16};
	data[OEnemy::Rifleman + Object::Jumping] = { 17, 18 };
	data[OEnemy::Tank + Object::Standing] = { 21, 23 };
	data[OEnemy::Tank + Object::Attacking] = { 21, 23 };
	data[OEnemy::Tank + Object::Attacking + 3] = { 34, 36 };
	data[OEnemy::Tank + Object::Attacking + 5] = { 24, 26 };
	data[OEnemy::Tank + Object::Attacking + 7] = { 30, 32 };
	data[OEnemy::Tank + Object::Attacking + 9] = { 37, 39 };
	data[OEnemy::Cannon + Object::Standing] = { 75, 83 };
	data[OEnemy::Cannon + Object::Attacking] = { 81, 83 };
	data[OEnemy::Cannon + Object::Attacking + 5] = { 49, 51 };


	Animation* _animEnemy = new Animation(EnemyXML, EnemyPNG);
	_animEnemy->SetDataAnimation(data);
	return _animEnemy;
}

OEnemy::OEnemy()
{
	Tag = Object::Enemy;
	_anim = GetAnimationEnemy();
}

OEnemy::~OEnemy()
{
	delete _anim;
}

void OEnemy::Init(D3DXVECTOR2 pos, int _type, int kind)
{
	AllowDraw = true;

	_enemyType = (Enemytype)_type;
	_kind = kind;
	position = pos;
	velocity = D3DXVECTOR2(-EnemySpeed, Gravity);
	SetState(Object::Running);
	this->SetBound(15, 32);
	HP = 1;
	type = _type;
}

void OEnemy::Controller()
{
	if (State == Object::Jumping)
	{
		JumpState();
		return;
	}
	velocity.y = Gravity;
}

D3DXVECTOR2 OEnemy::OnCollision(Object* obj, D3DXVECTOR2 side)
{
	switch (obj->Tag)
	{
	case Object::Wall:
		if (side.x != Collision::NONE)
		{
			velocity.x = -velocity.x;
			side.x = Collision::NONE;
		}
		if (side.y == Collision::BOTTOM)
		{
			SetState(Object::Running);
			if (position.x < obj->GetBound().left || position.x > obj->GetBound().right)
			{
				StartJump(-Gravity/2, 16, Gravity);
			}
		}
		return side;
	case Object::Bullet:
		obj->OnCollision(this);
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	default:
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	}
}

void OEnemy::OnCollision(Object* obj)
{
	switch (obj->Tag)
	{
	case Object::Player:
		Player::GetInstance()->_playerController->DeadState();
		return;
	default:
		return;
	}
}

void OEnemy::BeforeUpdate(float gameTime, Keyboard* key)
{
	this->SetBound(15, 32);
	this->Controller();
}

void OEnemy::Update(float gameTime, Keyboard* key)
{
	if (HP <= 0 && State != Object::Dying)
	{
		velocity.x = (Player::GetInstance()->GetFlipFlag()?-1:1)*EnemySpeed;
		StartJump(-Gravity / 2, 16, Gravity);
		State = Object::Dying;
	}
	//Update Animation
	if (State == Object::Dying)
	{
		this->SetBound(0, 0);
		JumpState();
		timeDead += gameTime;
		if (timeDead > 0.2)
			AllowDraw = false;
	}

	UpdateAnimation(gameTime);

	Object::Update(gameTime, key);
}

void OEnemy::UpdateAnimation(float gameTime)
{
	_anim->NewAnimationByIndex(_enemyType + this->State + _kind);
	int h = _anim->GetCurrentFrameInfo().h;
	_anim->SetPosition(D3DXVECTOR2(position.x, position.y + h / 2));
	_anim->SetFlipFlag(velocity.x > 0);
	_anim->Update(gameTime);
}

void OEnemy::SetBound(float width, float height)
{
	InfoSprite::Infoframe info = _anim->GetCurrentFrameInfo();
	Width = info.w;
	Height = info.h;

	float w = width >= 0 ? width : Width;
	float h = height >= 0 ? height : Height;
	bound.left = position.x - w / 2;
	bound.right = bound.left + w;
	bound.top = position.y + h;
	bound.bottom = position.y;
}

void OEnemy::Render(Viewport* viewport)
{
	//Vẽ OEnemy
	if (AllowDraw)
	{
		_anim->Render(viewport);
	}
}