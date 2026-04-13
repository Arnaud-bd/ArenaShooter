#include "Web.h"
#include "Player.h"

void Web::Start()
{
	SetSpeed(6.f);
}

void Web::Update()
{
	Entity::Update();
	mCooldownLifeProgress -= GameManager::DeltaTime();

	if (mCooldownLifeProgress <= 0)
	{
		m_pOwner->Destroy();
	}
}

void Web::CollisionEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		other->GetScript<Player>()->SetSpeedDivider(2.f);
	}
}

void Web::CollisionExit(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		other->GetScript<Player>()->SetSpeedDivider(1.f);
	}
}

