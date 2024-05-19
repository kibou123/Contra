#pragma once

class OWall;
class Bridge : public OWall
{
public:
	Bridge() : OWall() {};
	~Bridge();

	void Init(D3DXVECTOR2 pos, int type = Walltype::Brigde, int kind = 0);
	void SetBound(float width, float height);
	void BeforeUpdate(float gameTime, Keyboard* key);
	D3DXVECTOR2 OnCollision(Object* obj, D3DXVECTOR2 side);
	void Update(float gameTime, Keyboard* key);
	void OnCollision(Object* obj);
	void Render(Viewport* viewport);
	void Controller();
	void UpdateAnimation(float gameTime);
	static Animation* GetAnimationEnemy();
};

