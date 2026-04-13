#include "Room_1.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"

void Room_1::OnUpdate()
{

}

void Room_1::OnInitRoom()
{
    InitMap(RoomConfiguration::Type1());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_1.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }

    GameObject* door = MapLoader::LoadMap(RES_PATH"res/JSON/door.json", m_pScene)[0];
    door->transform.SetWorldPosition({15.f, 0.f, 38.f});

    Door* doorScript = door->AddScript<Door>();
    doorScript->SetRoomManager(m_pRoomManager);
    doorScript->SetRoom(this);
    m_vGameObject.push_back(door);
    

    mPlayerStartPos = {15.f, 2.f, 5.f};

    Vector3f32 playerPos = m_pPlayer->GetOwner()->transform.GetWorldPosition();
}
