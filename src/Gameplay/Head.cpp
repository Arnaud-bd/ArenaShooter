#include "Head.h"
#include "Player.h"

void Head::Update()
{
	m_pOwner->transform.WorldTranslate(mDirection * GameManager::DeltaTime());

	if (m_pOwner->transform.GetWorldPosition().y <= -5)
		m_pOwner->Destroy();
}

void Head::CollisionEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		Player* player = other->GetScript<Player>();
		player->TakeDamage(150);
	}
}