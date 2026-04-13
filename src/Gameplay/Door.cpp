#include "Door.h"
#include "ArenaShooterManager.h"
#include "ArenaUI.h"
#include "Room/SalleManager.h"
#include "Room/Room.h"

void Door::Start()
{
}

void Door::CollisionEnter(GameObject* other)
{
	if (other->GetName() != "Player") return;

	if (m_pRoom->IsFinish())
		m_pRoomManager->NextRoom();
}