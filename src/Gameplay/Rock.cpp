#include "Rock.h"
#include "Player.h"
#include "ArenaShooterManager.h"

void Rock::Update()
{
	PhysicComponent* myphysic = m_pOwner->GetComponent<PhysicComponent>();
	
	if (ArenaShooterManager::GetInstance()->GetState() != GameState::PLAY)
		myphysic->SetActive(false);
	else
		myphysic->SetActive(true);

	
}

void Rock::CollisionEnter(GameObject* other)
{
	if (other->GetName() == "Player")
	{
		Player* player = other->GetScript<Player>();
		player->TakeDamage(35);
		player->SetStatut(Player::Statut::Stun);
		m_pOwner->Destroy();
	}
}