#include "Room_4.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "Spider2.h"
#include "Spider3.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"
#include "Box.h"
#include "PM.h"
#include "Gun.h"

void Room_4::OnUpdate()
{
}

void Room_4::OnInitRoom()
{
    mCurrentDepth = 1;
    mCurrentHeight = 64;
    mCurrentWidth = 64;

    InitMap(RoomConfiguration::Type4());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_4.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }

    std::vector<GameObject*> spider1_go = MapLoader::LoadMap(RES_PATH"res/JSON/newShovKi’Soory.json", m_pScene);
    Spider3* spider1 = spider1_go[0]->AddScript<Spider3>();
    spider1_go[0]->transform.SetWorldPosition({ 24.f,10.f, 50.f });
    spider1->SetRoom(this);

    std::vector<GameObject*> spider2_go = MapLoader::LoadMap(RES_PATH"res/JSON/newShovKi’Soory.json", m_pScene);
    Spider3* spider2 = spider2_go[0]->AddScript<Spider3>();
    spider2_go[0]->transform.SetWorldPosition({ 42.f,10.f, 50.f });
    spider2->SetRoom(this);

    std::vector<GameObject*> spider3_go = MapLoader::LoadMap(RES_PATH"res/JSON/newSkilder.json", m_pScene);
    Spider2* spider3 = spider3_go[0]->AddScript<Spider2>();
    spider3_go[0]->transform.SetWorldPosition({ 31.f,0.f, 44.f });
    spider3->SetRoom(this);

    std::vector<GameObject*> spider4_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider4 = spider4_go[0]->AddScript<Spider1>();
    spider4_go[0]->transform.SetWorldPosition({ 20.f,0.f, 36.f });
    spider4->SetRoom(this);

    std::vector<GameObject*> spider5_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider5 = spider5_go[0]->AddScript<Spider1>();
    spider5_go[0]->transform.SetWorldPosition({ 30.f,0.f, 36.f });
    spider5->SetRoom(this);

    std::vector<GameObject*> spider6_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider6 = spider6_go[0]->AddScript<Spider1>();
    spider6_go[0]->transform.SetWorldPosition({ 40.f,0.f, 36.f });
    spider6->SetRoom(this);

    GameObject* door = MapLoader::LoadMap(RES_PATH"res/JSON/door.json", m_pScene)[0];
    door->transform.SetWorldPosition({ 32.f, 0.f, 62.f });

    Door* doorScript = door->AddScript<Door>();
    doorScript->SetRoomManager(m_pRoomManager);
    doorScript->SetRoom(this);
    m_vGameObject.push_back(door);

    mPlayerStartPos = { 32.f, 2.0f, 4.f };
}
