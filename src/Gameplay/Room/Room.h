#pragma once

#include "Engine.h"
#include "Pathfinding/Node.h"

class Enemy;
class Player;
class SalleManager;
class Tile;
class Entity;

using namespace gce;

class Room
{
private:
	std::vector<std::vector<std::vector<Tile*>>> m_vTiles;
	std::vector<std::vector<std::vector<Node<Tile>>>> m_vNodes;

	bool mMapUpdate = false;

	void SetNeighbors(Node<Tile>* node);
	void UpdateNode(Node<Tile>* node, bool becomeObstacle);
	Tile* CreateTile();

protected:
	int mCurrentHeight = 0;
	int mCurrentWidth = 0;
	int mCurrentDepth = 0;

	Player* m_pPlayer = nullptr;

	Scene* m_pScene = nullptr;

	SalleManager* m_pRoomManager = nullptr;

	std::vector<GameObject*> m_vGameObject;
	std::vector<Enemy*> m_vEnemy;

	Vector3f32 mPlayerStartPos;

	virtual void OnInitRoom() {};
	virtual void OnUpdate() {};

	void InitMap(std::vector<std::vector<std::vector<char>>> vMaps);

public:
	void InitRoom();
	void DestroyRoom();
	void AddEnemy(Enemy* pEnemy);
	void DeleteEnemy(Enemy* pEnemy);
	void Update();	
	bool IsFinish();
	void ResetNodes();

	gce::Vector3f32 GetPosInTab(gce::Vector3f32 pos);
	Node<Tile>* GetNodeWithPos(gce::Vector3f32 pos);
	Node<Tile>* GetNodeWithTabPos(gce::Vector3f32 pos);
	gce::Vector3f32 GetMapSize() { return gce::Vector3f32(mCurrentWidth, mCurrentHeight, mCurrentDepth); }
	Tile* GetTileWithPos(gce::Vector3f32 pos);

	void ResetByEntity(Entity* pEntity);

	void ActiveAllSpider();

	bool IsMapUpdate() { return mMapUpdate; }
	void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }
	void SetRoomManager(SalleManager* manager) { m_pRoomManager = manager; }
	void AddGameObject(GameObject* gameObject) { m_vGameObject.push_back(gameObject); }
	void RemoveGameObject(GameObject* gameObject);
};

