#include "OEnemy.h"
#include "Player.h"

Animation* OEnemy::GetAnimationEnemy()
{
	//Tạo Animation
	Animation::DataAnimMap data;
	// Soldier
	data[OEnemy::Soldier + Object::Running] = { 0 , 6};
	data[OEnemy::Soldier + Object::Dying] = { 7 , 8 };
	data[OEnemy::rifleman + Object::Running] = { 9 , 9 };


	Animation* _animEnemy = new Animation(EnemyXML, EnemyPNG);
	_animEnemy->SetDataAnimation(data);
	return _animEnemy;
}

OEnemy::OEnemy()
{
}

OEnemy::~OEnemy()
{
	delete _anim;
}

void OEnemy::Init(D3DXVECTOR2 pos, int _type, int kind)
{
	AllowDraw = true;
	Tag = Object::Enemy;

	_anim = GetAnimationEnemy();
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
	default:
		return D3DXVECTOR2(Collision::NONE, Collision::NONE);
	}
}

//void OEnemy::OnCollision(Object* obj)
//{
//}

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
	_anim->SetPosition(D3DXVECTOR2(position.x, position.y + Height / 2));
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