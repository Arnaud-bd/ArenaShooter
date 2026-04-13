#include "Room_5.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"
#include "Box.h"
#include "Gun.h"
#include "PM.h"

void Room_5::OnUpdate()
{

}

void Room_5::OnInitRoom()
{
    mCurrentDepth = 1;
    mCurrentHeight = 0;
    mCurrentWidth = 0;

    InitMap(RoomConfiguration::Type5());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_5.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }

    float offset = 1.2f;

    for (int i = 0; i < 5; i++)
    {
        for (int j = i; j < 5; j++)
        {
            GameObject* box = MapLoader::LoadMap(RES_PATH"res/JSON/breakableBox.json", m_pScene)[0];
            box->AddComponent<BoxCollider>();
            box->transform.SetWorldPosition({ 5.0f + i * offset, 1.f, 35.f + j * offset });
            box->AddScript<DestructibleBox>();

            m_vGameObject.push_back(box);
            
        }
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = i; j < 5; j++)
        {
            GameObject* box = MapLoader::LoadMap(RES_PATH"res/JSON/breakableBox.json", m_pScene)[0];
            box->AddComponent<BoxCollider>();
            box->transform.SetWorldPosition({ 31.f - i * offset, 0.5f, 35.f + j * offset });
            box->AddScript<DestructibleBox>();

            m_vGameObject.push_back(box);
        }
    }

    GameObject* door = MapLoader::LoadMap(RES_PATH"res/JSON/door.json", m_pScene)[0];
    door->transform.SetWorldPosition({ 24.f, 0.f, 46.f });

    Door* doorScript = door->AddScript<Door>();
    doorScript->SetRoomManager(m_pRoomManager);
    doorScript->SetRoom(this);
    m_vGameObject.push_back(door);

    mPlayerStartPos = { 24.0f, 2.0f, 5.0f};
}
