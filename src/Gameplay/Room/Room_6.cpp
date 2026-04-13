#include "Room_6.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "Spider2.h"
#include "Spider3.h"
#include "Boss.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"
#include "Box.h"

void Room_6::OnUpdate()
{
}

void Room_6::OnInitRoom()
{
    mCurrentDepth = 1;
    mCurrentHeight = 96;
    mCurrentWidth = 192;

    InitMap(RoomConfiguration::Type6());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_6.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }

    std::vector<GameObject*> geo = MapLoader::LoadMap(RES_PATH"res/JSON/boss.json", m_pScene);

    GameObject* baseBoss = nullptr;

    for (GameObject* gameObject : geo)
    {
        std::string name = gameObject->GetName();
        if (name == "boss_body")
            baseBoss = gameObject;
    }
    baseBoss->transform.SetWorldPosition({ 15.f, 0.f, 80.f });
    Boss* boss = baseBoss->AddScript<Boss>();
    boss->SetRoom(this);
    baseBoss->transform.WorldRotate({0.f,PI, 0.f});

    std::vector<GameObject*> spider1_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider1 = spider1_go[0]->AddScript<Spider1>();
    spider1_go[0]->transform.SetWorldPosition({ 34.f,0.f, 45.f });
    spider1->SetRoom(this);

    //
    std::vector<GameObject*> spider4_go = MapLoader::LoadMap(RES_PATH"res/JSON/newSkilder.json", m_pScene);
    Spider2* spider4 = spider4_go[0]->AddScript<Spider2>();
    spider4_go[0]->transform.SetWorldPosition({ 40.f,0.f, 67.f });
    spider4->SetRoom(this);
    std::vector<GameObject*> spider7_go = MapLoader::LoadMap(RES_PATH"res/JSON/newShovKi’Soory.json", m_pScene);
    Spider3* spider7 = spider7_go[0]->AddScript<Spider3>();
    spider7_go[0]->transform.SetWorldPosition({ 33.f,10.f, 74.f });
    spider7->SetRoom(this);

    std::vector<GameObject*> spider8_go = MapLoader::LoadMap(RES_PATH"res/JSON/newShovKi’Soory.json", m_pScene);
    Spider3* spider8 = spider8_go[0]->AddScript<Spider3>();
    spider8_go[0]->transform.SetWorldPosition({ 30.f,10.f, 69.f });
    spider8->SetRoom(this);

    mPlayerStartPos = { 32.f, 2.0f, 4.f };
}
