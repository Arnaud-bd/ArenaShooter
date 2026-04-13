#pragma once


#include "Engine.h"
#include "Render.h"
#include "Enemy.h"

class SalleManager;
class Room;

using namespace gce;

DECLARE_SCRIPT(Door, ScriptFlag::Start | ScriptFlag::CollisionEnter)

private:
	SalleManager* m_pRoomManager = nullptr;
	Room* m_pRoom = nullptr;

public:
	void Start();
	void CollisionEnter(GameObject* other);

	void SetRoomManager(SalleManager* manager) { m_pRoomManager = manager; }
	void SetRoom(Room* pRoom) { m_pRoom = pRoom; }

	END_SCRIPT