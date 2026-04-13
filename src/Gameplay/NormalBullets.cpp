#include "NormalBullets.h"

void NormalBullets::Start()
{
	Bullets::Start();
}

void NormalBullets::Update()
{
	Bullets::Update();

	float dt = GameManager::FixedDeltaTime();

	m_pOwner->transform.WorldTranslate({ mDirection * mBulletSpeed * dt});
}