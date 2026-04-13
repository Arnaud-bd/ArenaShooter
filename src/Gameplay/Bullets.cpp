#include "Bullets.h"

void Bullets::Start()
{
	mDirection = m_pOwner->transform.GetWorldForward();
	mLifeTime = 2.f;
	mBulletSpeed = 100.0f;
	m_pOwner->SetName("Bullet");
}

void Bullets::Update()
{
	mLifeTime -= GameManager::DeltaTime();

	if(mLifeTime <= 0.f || mToDestroy)
		m_pOwner->Destroy();
}

void Bullets::CollisionEnter(GameObject* other)
{
	cstr name = other->GetName();
	if (name == "Player" || name == "Weapon" || name == m_pOwner->GetName())
		return;

	mToDestroy = true;
}