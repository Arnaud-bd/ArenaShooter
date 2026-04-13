#pragma once

#include "Entity.h"
#include "Pathfinding/WPosMPos.h"
#include "Pathfinding/Node.h"

class Tile;
class Room;
class Player;

using namespace gce;

DECLARE_CHILD_SCRIPT(Enemy, Entity, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::Destroy | ScriptFlag::CollisionEnter)

private:
	//----StateMachine function----//
	void OnUpdateIdle();
	void OnStartFight();
	void OnUpdateFight();
	void OnStartDeath();
	void OnUpdateDeath();
	bool IsPlayerNear();
	bool HaveTakenDamage();

	void DropLoot();
	void CreateStateMachine();

	//Pathfinding
	std::vector<gce::Vector3f32> m_vPath;
	std::vector<Node<Tile>*> m_vNodesPath;
	bool mIsMoving = false;

	bool mIsPatrouille = false;
	std::vector<WPosMPos> m_vPatrouillePoint;
	int mCurrentPos = 0;

	WPosMPos mNextPos;
	Vector3f32 mCurrentTabPos;

	bool mIsWaiting = false;
	float mWaitTiming = 0.5f;
	float mWaitProgress = 0.5f;

	bool mToStop = false;

	float mSpeed = 4.0f;

	bool mHaveTakenDamage = false;

	bool mStopMoving = false;

	float mDeathScale = 1.f;

	float mDeathScaleTime = 2.f;

	void AddToPriorityQueue(std::vector<Node<Tile>*>& priority_queue, Node<Tile>* nodeToAdd);
	bool FindPath(WPosMPos newPos, bool addToPath = false);
	void GetPath(WPosMPos newPos, bool addToPath = false);
	void CheckNextNode();

	virtual void Attack() {};

protected:
	StateMachine* m_pSM = nullptr;

	Room* m_pRoom = nullptr;

	Player* m_pPlayer = nullptr;

	Vector3f32 mCurrentTargetPos;
	float mStopDistance = 0;

	float mDetecteDistance = 5.f;

	float mCooldownTime = 5.f;
	float mCooldownProgress = 0.f;

	bool mAttackWithStop = true;

	int mValue = 50;

public:
	void Start();
	void Update();
	void Destroy();
	void CollisionEnter(GameObject* other);

	void ChangePatrouille(WPosMPos currentPos);
	void AddPointToPath(WPosMPos newPos);
	void Move(WPosMPos newPos);
	void StopMoving();
	void ContinueMoving();

	void StartFight();

	void SetRoom(Room* pRoom);

	END_SCRIPT