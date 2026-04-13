#include "Room_3.h"

#include "Enemy.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "ArenaShooterManager.h"
#include "Spider1.h"
#include "Spider2.h"
#include "RessourceManager.h"
#include "Door.h"
#include "Pathfinding/RoomConfiguration.h"
#include "Box.h"

void Room_3::OnUpdate()
{
}

void Room_3::OnInitRoom()
{
    mCurrentDepth = 1;
    mCurrentHeight = 56;
    mCurrentWidth = 32;

    InitMap(RoomConfiguration::Type3());

    std::vector<GameObject*> map = MapLoader::LoadMap(RES_PATH"res/JSON/Rooms/Room_3.json", m_pScene);

    for (GameObject* go : map)
    {
        m_vGameObject.push_back(go);
    }

    std::vector<GameObject*> spider1_go = MapLoader::LoadMap(RES_PATH"res/JSON/newSkilder.json", m_pScene);
    Spider2* spider1 = spider1_go[0]->AddScript<Spider2>();
    spider1_go[0]->transform.SetWorldPosition({ 3.f,0.f, 20.f});
    spider1->SetRoom(this);

    std::vector<GameObject*> spider3_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider3 = spider3_go[0]->AddScript<Spider1>();
    spider3_go[0]->transform.SetWorldPosition({ 9.f,0.f, 40.f });
    spider3->SetRoom(this);
    spider3->AddPointToPath(WPosMPos(36, 6, 0, 6, 0, 36));

    std::vector<GameObject*> spider2_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider2 = spider2_go[0]->AddScript<Spider1>();
    spider2_go[0]->transform.SetWorldPosition({ 13.f,0.f, 44.f });
    spider2->SetRoom(this);
    spider2->AddPointToPath(WPosMPos(46, 15, 0, 15, 0, 46));

    std::vector<GameObject*> spider4_go = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);
    Spider1* spider4 = spider4_go[0]->AddScript<Spider1>();
    spider4_go[0]->transform.SetWorldPosition({ 10.f,0.f, 43.f });
    spider4->SetRoom(this);
    spider4->AddPointToPath(WPosMPos(47, 5, 0, 5, 0, 47));

    Geometry* cube = RessourceManager::GetGeoWithName("cube");

    if (cube == nullptr)
    {
        cube = GeometryFactory::CreateCubeGeo();
        RessourceManager::AddGeometry(cube, "cube");
    }

    for (int i = 0; i < 10; i++)
    {
        GameObject* box = MapLoader::LoadMap(RES_PATH"res/JSON/breakableBox.json", m_pScene)[0];
        box->AddComponent<BoxCollider>();
        box->transform.SetWorldPosition({ 17.5f + i, 0.5f, 16.f });
        box->AddScript<DestructibleBox>();

        m_vGameObject.push_back(box);
    }

    GameObject* door = MapLoader::LoadMap(RES_PATH"res/JSON/door.json", m_pScene)[0];
    door->transform.SetWorldPosition({ 2.f, 0.f, 48.f });
    door->transform.WorldRotate({0.f, -PI/2.f, 0.f});

    Door* doorScript = door->AddScript<Door>();
    doorScript->SetRoomManager(m_pRoomManager);
    doorScript->SetRoom(this);
    m_vGameObject.push_back(door);

    mPlayerStartPos = { 5.f, 2.0f, 8.f };
}
