#include "Room_2.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"

void Room_2::OnUpdate()
{

}

void Room_2::OnInitRoom()
{
    mCurrentDepth = 1;
    mCurrentHeight = 48;
    mCurrentWidth = 56;

    InitMap(RoomConfiguration::Type2());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_2.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }
    
    std::vector<GameObject*> spider1_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider1 = spider1_go[0]->AddScript<Spider1>();
    spider1_go[0]->transform.SetWorldPosition({ 40.f,0.f, 5.f });
    spider1->SetRoom(this);
    spider1->AddPointToPath(WPosMPos(5, 35, 0, 35, 0, 5));
    spider1->AddPointToPath(WPosMPos(10, 35, 0, 35, 0, 10));

    std::vector<GameObject*> spider2_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider2 = spider2_go[0]->AddScript<Spider1>();
    spider2_go[0]->transform.SetWorldPosition({ 50.f,0.f, 5.f });
    spider2->SetRoom(this);
    spider2->AddPointToPath(WPosMPos(10, 54, 0, 54, 0, 10));

    GameObject* door = MapLoader::LoadMap(RES_PATH"res/JSON/door.json", m_pScene)[0];
    door->transform.SetWorldPosition({ 48.f, 0.f, 2.f });
    door->transform.WorldRotate({0.f, PI, 0.f});

    Door* doorScript = door->AddScript<Door>();
    doorScript->SetRoomManager(m_pRoomManager);
    doorScript->SetRoom(this);
    m_vGameObject.push_back(door);

    mPlayerStartPos = {40.f, 2.0f, 35.f};
}
