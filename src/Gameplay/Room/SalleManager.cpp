#include "SalleManager.h"
#include "ArenaUI.h"
#include "ArenaShooterManager.h"
#include "Room_1.h"
#include "Room_2.h"
#include "Room_3.h"
#include "Room_4.h"
#include "Room_5.h"
#include "Room_6.h"
#include "Player.h"

#include "ArenaShooterManager.h"
#include "ArenaUI.h"

void SalleManager::Start()
{
	m_pManager = ArenaShooterManager::GetInstance();

	//Create each room
	Room_1* room = new Room_1;
	room->SetPlayer(m_pPlayer);
	room->SetRoomManager(this);
	room->InitRoom();	
	m_vRooms.push_back(room);

	Room_2* room2 = new Room_2;
	m_vRooms.push_back(room2);

	Room_3* room3 = new Room_3;
	m_vRooms.push_back(room3);

	Room_4* room4 = new Room_4;
	m_vRooms.push_back(room4);

	Room_5* room5 = new Room_5;
	m_vRooms.push_back(room5);

	Room_6* room6 = new Room_6;
	m_vRooms.push_back(room6);

}

void SalleManager::Update()
{
	m_vRooms[0]->Update();
}

void SalleManager::NextRoom()
{
	ArenaShooterManager::GetInstance()->GetIU()->SetLoadScreen(true);

	m_pPlayer->SetStatut(Player::Normal);

	m_vRooms[0]->DestroyRoom();
	m_vRooms.erase(m_vRooms.begin());
	m_vRooms[0]->SetPlayer(m_pPlayer);
	m_vRooms[0]->SetRoomManager(this);
	m_vRooms[0]->InitRoom();

	ArenaShooterManager::GetInstance()->GetIU()->SetLoadScreen(false);
}

void SalleManager::Reset()
{
	for (int i = 0; i < m_vRooms.size(); i++)
	{
		m_vRooms[i]->DestroyRoom();
	}

	m_vRooms.clear();

	Start();
}