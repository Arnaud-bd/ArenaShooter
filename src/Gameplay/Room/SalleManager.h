#pragma once

#include "Engine.h"
#include "Render.h"

class Room;
class Player;
class ArenaShooterManager;

using namespace gce;

DECLARE_SCRIPT(SalleManager, ScriptFlag::Start | ScriptFlag::Update)

private:
	std::vector<Room*> m_vRooms;
	Player* m_pPlayer = nullptr;
	ArenaShooterManager* m_pManager = nullptr;

public:
	void Start();
	void Update();
	void NextRoom();
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
	void Reset();

	Room* GetCurrentRoom() { return m_vRooms[0]; }

	END_SCRIPT