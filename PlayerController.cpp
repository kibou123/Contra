#include "PlayerController.h"
#include "Player.h"
#include "ObjectManager.h"
#include "FuncItem.h"

PlayerController::PlayerController()
{
	player = Player::GetInstance();
	player->SetState(Object::Standing);
	isAttack = false;
	isReload = false;

	_functionMap[Object::Standing] = &PlayerController::StandState;
	_functionMap[Object::Running] = &PlayerController::StandState;
	_functionMap[Object::Jumping] = &PlayerController::JumpState;
	_functionMap[Object::Dying] = &PlayerController::DeadState;
	_functionMap[Object::Sitting] = &PlayerController::SitState;
	_functionMap[Object::Swimming] = &PlayerController::SwimState;
	_functionMap[Object::Diving] = &PlayerController::DivingState;
	_functionMap[Object::Falling] = &PlayerController::JumpState;
}

PlayerController::~PlayerController()
{
}

//Trạng thái Đứng
void PlayerController::StandState() //reset all state
{
	player->State = player->GetVelocity().x != 0 ? Object::Running : Object::Standing;
	player->SetBound(25, 35);

	if (key->IsKeyDown(Dik_JUMP))
	{
		player->StartJump(-Gravity);
		return;
	}

	if (key->IsKeyDown(Dik_DOWN))
	{
		SitState();
		return;
	}
}

//Trang thái chạy
void PlayerController::RunState()
{
}

void PlayerController::Fall()
{
}

//Trạng thái nhảy
void PlayerController::JumpState()
{
	player->SetBound(20, 20);
	player->JumpState();
}

//Trạng thái chêt
void PlayerController::DeadState()
{
	player->State = Object::Dying;

}

//Trạng thái ngồi
void PlayerController::SitState()
{
	player->isFall = false;
	player->State = Object::Sitting;
	player->SetBound(32, 16);
	if (key->GIsKeyUp(Dik_DOWN))
	{
		StandState();
		return;
	}

	if (key->IsKeyDown(Dik_JUMP))
	{
		player->isFall = true;
		return;
	}
}

void PlayerController::SwimState()
{
	player->State = Object::Swimming;
	player->SetBound(25, 16);

	if (key->IsKeyDown(Dik_DOWN))
	{
		DivingState();
		return;
	}
}

void PlayerController::DivingState()
{
	player->State = Object::Diving;
	player->SetBound(1, 1);
	if (key->GIsKeyUp(Dik_DOWN))
	{
		SwimState();
		return;
	}
}

void PlayerController::MoveX()
{
	//Nếu chết rồi không có di chuyển
	if (player->State == Object::Dying || player->State == Object::Sitting || player->State == Object::Diving)
	{
		player->SetVelocityX(0);
		return;
	}

	float speed = 0;
	if (key->IsKeyDown(Dik_RIGHT))
	{
		player->SetFlipFlag(false);
		speed = RunSpeed;
	}
	else if (key->IsKeyDown(Dik_LEFT))
	{
		player->SetFlipFlag(true);
		speed = -RunSpeed;
	}
	player->SetVelocityX(speed);
}

void PlayerController::PlayControllerF()
{
	//Lấy Function từ player state
	
	FunctionMap::iterator it = _functionMap.find(player->State);
	if( it != _functionMap.end())
	(this->*(it->second))();
	
}

void PlayerController::Update(float gameTime, Keyboard* key)
{
	this->key = key;
	if (player->State == Object::Dying) return;

	if (player->State != Object::Jumping) {
		player->SetVelocityY(Gravity);
	}

	this->PlayControllerF();
	MoveX();

	timeReload += gameTime;
	AttackState();
}

void PlayerController::AttackState()
{
	if (player->State == Object::Diving) return;
	//
	if (key->GIsKeyUp(Dik_ATTACK) || player->GunType == OItem::M)
	{
		//Tạo Đạn Theo Súng
		isAllowAttack = true;
	}
	if (isAttack && timeReload > 0.15)
	{
		isAttack = false;
	}
	//
	if (player->ListBullet.size() == 0)
		for (int i = 0; i < 11; i++)//tạo 10 viên đạn trước cho khỏi lag
		{
			OBullet* bullet = new OBullet();
			bullet->Reset();
			player->ListBullet.push_back(bullet);
			ObjectManager::GetInstance()->AddObjectMap(bullet);
		}

	vector <OBullet*> listBullet;
	listBullet.clear();
	FuncItem::GetBullet(listBullet);
	if (listBullet.size() == 0)
	{
		return;
	}

	float timeR = isReload ? 0.1 : 0.2;//ăn đạn R thì bắn nhanh hơn
	if (isAllowAttack && key->IsKeyDown(Dik_ATTACK) && (timeReload > timeR - 0.05))
	{
		timeReload = 0;
		isAllowAttack = false;

		isAttack = true;

		//Khong flip
		int	acc = 1;
		D3DXVECTOR2 pos = player->GetPosition();
		pos.x = player->GetBound().right;
		pos.y = player->GetBound().top - player->GetHeight() / 2;
		if (player->isFlip)
		{
			acc = -1;
			pos.x = player->GetBound().left;
		}
		for (size_t i = 0; i < listBullet.size(); i++)
		{
			listBullet[i]->Fire(pos);
		}
	}
}