#include "Enemy.h"
#include "StateMachine.h"

#include "Pathfinding/Node.h"
#include "Pathfinding/Tile.h"
#include "Room/Room.h"
#include "Pathfinding/UtilsPathfinding.h"
#include "ArenaShooterManager.h"
#include "Player.h"
#include "MapLoader.hpp"
#include "RocketLauncher.h"
#include "Gun.h"
#include "PM.h"
#include "Bullets.h"
#include "Audio.h"

// === Idle State Functions ===
void Enemy::OnUpdateIdle()
{

}

// === Fight State Functions ===
void Enemy::OnUpdateFight()
{
    Vector3f32 posPlayer = m_pPlayer->GetOwner()->transform.GetWorldPosition();
    Vector3f32 posOwner = m_pOwner->transform.GetWorldPosition();

    float distanceFromPlayer = UtilsPathfinding::GetDistance(posPlayer, posOwner);

    if (distanceFromPlayer > mStopDistance)
    {
        Tile* playerTile = m_pRoom->GetTileWithPos({posPlayer.x, 0, posPlayer.z});

        if (playerTile != nullptr)
        {
            Vector3f32 playerPos = playerTile->GetPosition();

            if (mCurrentTargetPos != playerPos)
                Move(playerTile->mPos);
        }
    }

    if (distanceFromPlayer <= mStopDistance || mAttackWithStop == false)
    {
        if (mCooldownProgress <= 0)
        {
            UnsetTarget();
            //Attack
            Attack();
        }
    }
}

void Enemy::OnStartDeath()
{
    m_pOwner->SetType(EntityType::DEFAULT);

    if (m_pOwner->GetName() == "Boss")
        ArenaShooterManager::GetInstance()->SetGameState(GameState::WIN);

    DropLoot();

    AudioUse::Play("Death");
}

void Enemy::OnUpdateDeath()
{
    mDeathScale -= GameManager::DeltaTime() / mDeathScaleTime;

    if (mDeathScale >= 0)
        m_pOwner->transform.WorldScale(Vector3f32(mDeathScale, mDeathScale, mDeathScale));
    else
    {
        m_pPlayer->AddScore(mValue);

        m_pRoom->DeleteEnemy(this);
    }
}

void Enemy::DropLoot()
{
    int chance = rand() % 3;

    if (chance > 2)
        return;

    Vector3f32 myPos = m_pOwner->transform.GetWorldPosition();
    std::vector<GameObject*> weapon;

    switch (chance)
    {
    case(0):
        
        //Arme par terre
        weapon = MapLoader::LoadMap(RES_PATH"res/JSON/Rocket-Launcher-json.json", m_pOwner->GetScene());

        weapon[0]->AddScript<RocketLauncher>();

        break;

    case(1):
        
        //Arme par terre
        weapon = MapLoader::LoadMap(RES_PATH"res/JSON/Rifle-json.json", m_pOwner->GetScene());

        weapon[0]->AddScript<PM>();

        break;

    case(2):

        //Arme par terre
        weapon = MapLoader::LoadMap(RES_PATH"res/JSON/gun.json", m_pOwner->GetScene());

        weapon[0]->AddScript<Gun>();

        break;

    default:

        break;
    }

    weapon[0]->GetScript<Weapon>()->DropWeapon();
    weapon[0]->transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition());
}

// === Condition Function ===
bool Enemy::IsPlayerNear()
{
	if (m_pPlayer == nullptr) return false;

	Vector3f32 pPos = m_pPlayer->GetOwner()->transform.GetWorldPosition();
	Vector3f32 mPos = m_pOwner->transform.GetWorldPosition();

	float distance = sqrt((pPos.x - mPos.x) * (pPos.x - mPos.x) + (pPos.y - mPos.y) * (pPos.y - mPos.y) + (pPos.z - mPos.z) * (pPos.z - mPos.z));

    bool condition = distance < mDetecteDistance;

    if (condition)
        m_pRoom->ActiveAllSpider();

	return condition;
}

bool Enemy::HaveTakenDamage()
{
    if (mHaveTakenDamage)
        m_pRoom->ActiveAllSpider();

    return mHaveTakenDamage;
}

void Enemy::CreateStateMachine()
{
    m_pSM = GameManager::GetStatesSystem().CreateStateMachine(m_pOwner);

    // Add Idle state
    String idle = "Idle";
    m_pSM->AddAction(idle, nullptr, [this]() {this->OnUpdateIdle();});

    //Add Fight state
    String fight = "Fight";
    m_pSM->AddAction(fight, [this]() {this->OnStartFight(); }, [this]() {this->OnUpdateFight(); });

    //Add Dead state
    String dead = "Dead";
    m_pSM->AddAction(dead, [this]() {this->OnStartDeath();}, [this]() {this->OnUpdateDeath();});

    //Add transition Idle -> Fight
    StateMachine::Condition nearCondition = { [this]() { return this->IsPlayerNear(); } };
    StateMachine::Condition damageCondition = { [this]() {return this->HaveTakenDamage(); } };
    Vector<StateMachine::Condition> fightConditions;
    fightConditions.PushBack(nearCondition);
    fightConditions.PushBack(damageCondition);

    m_pSM->AddTransition(fightConditions, idle, fight);

    //Add transition Fight -> Death
    StateMachine::Condition deadCondition = { [this]() {return this->IsDead();} };
    Vector<StateMachine::Condition> deathConditions;
    deathConditions.PushBack(deadCondition);

    m_pSM->AddTransition(deathConditions, fight, dead);
}

void Enemy::Start()
{
    Entity::Start();

    CreateStateMachine();

    m_pPlayer = ArenaShooterManager::GetInstance()->GetPlayer();

    Node<Tile>* node = m_pRoom->GetNodeWithPos(m_pOwner->transform.GetWorldPosition());

    if (node == nullptr)
        return;

    node->data->mIsLock = true;

    mCurrentTabPos = gce::Vector3f32(node->data->mPos.col, node->data->mPos.row, node->data->mPos.height);

    if (node->data != nullptr)
    {
        ChangePatrouille(node->data->mPos);
    }

    Move(m_vPatrouillePoint[0]);

    mCooldownProgress = mCooldownTime;

    m_pOwner->SetType(EntityType::ENEMY);
}

void Enemy::Destroy()
{
    Node<Tile>* node = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);

    node->data->mIsLock = false;
}

void Enemy::Update()
{
    if (IsDead()) return;

    if (ArenaShooterManager::GetInstance()->GetState() != GameState::PLAY);

    Entity::Update();

    mCooldownProgress -= GameManager::DeltaTime();

    mToStop = false;

    if (mIsMoving && m_vPath.empty() == false && mStopMoving == false)
    {
        if (m_pRoom->IsMapUpdate())
        {
            WPosMPos nextPos;

            if (mIsPatrouille)
            {
                nextPos = m_vPatrouillePoint[mCurrentPos];
            }
            else
            {
                nextPos = mNextPos;
            }

            if (FindPath(nextPos))
            {
                GetPath(nextPos);
            }
        }

        gce::Vector3f32 goTo = m_vPath[0];

        CheckNextNode();

        if (mIsWaiting)
        {
            mWaitProgress -= GameManager::DeltaTime();

            if (mWaitProgress <= 0)
            {
                mIsWaiting = false;

                Node<Tile>* node = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);

                node->data->mIsLock = false;
            }
            else
                return;
        }

        if (mToStop == false)
        {
            GoToPosition(goTo.x, goTo.y, goTo.z, mSpeed);
        }

        float distMin = 0.05f;

        Vector3f32 pos = m_pOwner->transform.GetWorldPosition();

        float dist = UtilsPathfinding::GetDistance(pos.x, pos.y, pos.z, goTo.x, goTo.y, goTo.z);

        if (dist <= distMin)
        {
            m_pOwner->transform.SetWorldPosition(goTo);

            m_vPath.erase(m_vPath.begin());

            if (m_vPath.empty())
            {
                if (mIsPatrouille)
                {
                    if (m_vPatrouillePoint.size() > mCurrentPos + 1)
                        mCurrentPos++;
                    else
                        mCurrentPos = 0;

                    WPosMPos nextPos = m_vPatrouillePoint[mCurrentPos];

                    if (FindPath(nextPos))
                    {
                        GetPath(nextPos);
                        //ClearPath();
                    }
                    else
                    {
                        Node<Tile>* node = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);

                        node->data->mIsLock = true;
                        m_vPatrouillePoint.clear();
                        mIsPatrouille = false;
                        mIsMoving = false;
                    }
                }
                else
                {
                    mIsMoving = false;

                    Node<Tile>* node = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);

                    node->data->mIsLock = true;
                }
            }

        }
    }
}


//----Pathfinding----//
void Enemy::AddToPriorityQueue(std::vector<Node<Tile>*>& priority_queue, Node<Tile>* nodeToAdd)
{
    if (priority_queue.empty())
    {
        priority_queue.push_back(nodeToAdd);
        return;
    }
    //si = prendre le plus loin de start
    for (int i = 0; i < priority_queue.size(); i++)
    {
        if (nodeToAdd->totalCost < priority_queue[i]->totalCost)
        {
            priority_queue.insert(priority_queue.begin() + i, nodeToAdd);
            return;
        }
        else if (nodeToAdd->totalCost == priority_queue[i]->totalCost)
        {
            if (nodeToAdd->costFromStart > priority_queue[i]->costFromStart)
            {
                priority_queue.insert(priority_queue.begin() + i, nodeToAdd);
                return;
            }
        }
    }

    priority_queue.push_back(nodeToAdd);
}

bool Enemy::FindPath(WPosMPos newPos, bool addToPath)
{
    m_pRoom->ResetNodes();

    gce::Vector3f32 endPos = gce::Vector3f32(newPos.col, newPos.row, newPos.height);

    Node<Tile>* start;

    if (mIsMoving && m_vNodesPath.size() >= 2)
        start = m_vNodesPath[1];
    else if (addToPath == false)
        start = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);
    else
        start = m_vNodesPath.back();

    if (start == nullptr)
        return false;
    

    Node<Tile>* end = m_pRoom->GetNodeWithTabPos(endPos);

    Tile* tileStart = start->data;
    Tile* tileEnd = end->data;

    start->totalCost = abs(tileStart->mPos.col - tileEnd->mPos.col) + abs(tileStart->mPos.row - tileEnd->mPos.row);

    std::vector<Node<Tile>*> priority_queue;
    priority_queue.push_back(start);

    while (!priority_queue.empty())
    {
        Node<Tile>* node = priority_queue[0];
        priority_queue.erase(priority_queue.begin());

        Tile* tile = node->data;

        node->isVisited = true;

        if (tile == tileEnd)
        {
            return true;
        }

        for (Node<Tile>* neighbor : node->neighbors)
        {
            Tile* tileN = neighbor->data;

            if (tileN->mIsObstacle || tileN->mIsLock)
            {
                continue;
            }

            int addToCostFromStart = 1;

            if (tileN->mPos.z != tile->mPos.z)
                addToCostFromStart = 2;

            if (neighbor->isVisited)
            {
                //si visiter verifier si le chemin actuel serait plus interessant
                int otherCost = neighbor->totalCost;

                int newCostFromStart = node->costFromStart + addToCostFromStart;
                int newTotalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + newCostFromStart;

                if (otherCost > newTotalCost)
                {
                    neighbor->comeFrom = node;
                    neighbor->costFromStart = newCostFromStart;
                    neighbor->totalCost = newTotalCost;
                }

                continue;
            }

            neighbor->isVisited = true;

            int newCostFromStart = node->costFromStart + addToCostFromStart;
            neighbor->comeFrom = node;
            neighbor->costFromStart = newCostFromStart;
            neighbor->totalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + neighbor->costFromStart;

            AddToPriorityQueue(priority_queue, neighbor);
        }

        int i = 0;
        for (Node<Tile>* neighbor : node->diagoNeighbors)
        {
            if (neighbor == nullptr)
            {
                i++;
                continue;
            }

            Tile* tileN = neighbor->data;

            if (tileN->mIsObstacle || tileN->mIsLock)
            {
                i++;
                continue;
            }

            if (neighbor->data->mPos.z + 1 < m_pRoom->GetMapSize().z)
            {
                Node<Tile>* upNode = m_pRoom->GetNodeWithTabPos(gce::Vector3f32(tileN->mPos.x, tileN->mPos.y, tileN->mPos.z + 1));

                if (upNode->data->mIsObstacle == false)
                {
                    i++;
                    continue;
                }
            }

            if (neighbor->isVisited)
            {
                //si visiter verifier si le chemin actuel serait plus interessant
                int otherCost = neighbor->totalCost;

                int newCostFromStart = node->costFromStart + 1;
                int newTotalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + newCostFromStart;

                if (otherCost > newTotalCost)
                {
                    neighbor->comeFrom = node;
                    neighbor->costFromStart = newCostFromStart;
                    neighbor->totalCost = newTotalCost;
                }

                i++;
                continue;
            }

            gce::Vector3f32 posWorld = { neighbor->data->mPos.x, neighbor->data->mPos.y, neighbor->data->mPos.z };

            gce::Vector3f32 pos1;
            gce::Vector3f32 pos2;

            gce::Vector3f32 posTab = m_pRoom->GetPosInTab(posWorld);
            mCurrentTabPos;

            switch (i)
            {
            case 0:
            {
                pos1 = posTab + gce::Vector3f32(1, 0, 0);
                pos2 = posTab + gce::Vector3f32(0, 1, 0);
                break;
            }
            case 1:
            {
                pos1 = posTab + gce::Vector3f32(-1, 0, 0);
                pos2 = posTab + gce::Vector3f32(0, 1, 0);
                break;
            }
            case 2:
            {
                pos1 = posTab + gce::Vector3f32(-1, 0, 0);
                pos2 = posTab + gce::Vector3f32(0, -1, 0);
                break;
            }
            case 3:
            {
                pos1 = posTab + gce::Vector3f32(1, 0, 0);
                pos2 = posTab + gce::Vector3f32(0, -1, 0);
                break;
            }
            };

            Node<Tile>* node1 = m_pRoom->GetNodeWithTabPos(pos1);
            Node<Tile>* node2 = m_pRoom->GetNodeWithTabPos(pos2);

            if (node1->data->mIsObstacle || node2->data->mIsObstacle)
            {
                i++;
                continue;
            }

            if (node1->data->mIsLock || node2->data->mIsLock)
            {
                i++;
                continue;
            }
            neighbor->isVisited = true;

            int newCostFromStart = node->costFromStart + 1;
            neighbor->comeFrom = node;
            neighbor->costFromStart = newCostFromStart;
            neighbor->totalCost = abs(tileN->mPos.col - endPos.x) + abs(tileN->mPos.row - endPos.y) + neighbor->costFromStart;

            AddToPriorityQueue(priority_queue, neighbor);

            i++;
        }
    }

    return false;
}

void Enemy::GetPath(WPosMPos newPos, bool addToPath)
{
    int offset = 0;
    int offsetNode = 0;

    if (!addToPath)
    {
        m_vPath.clear();
        m_vNodesPath.clear();
    }
    else
    {
        offset = m_vPath.size();
        offsetNode = m_vNodesPath.size();
    }

    Node<Tile>* node = m_pRoom->GetNodeWithTabPos(gce::Vector3f32(newPos.col, newPos.row, newPos.height));

    while (node != nullptr)
    {
        // Ajoute la position
        m_vPath.insert(m_vPath.begin() + offset, node->data->GetPosition());

        // Ajoute aussi la node (obligatoire pour CheckNextNode)
        m_vNodesPath.insert(m_vNodesPath.begin() + offsetNode, node);

        // Stop : si on est � la startNode
        if (node->comeFrom == nullptr)
        {
            return;
        }

        // Suivant
        node = node->comeFrom;
    }
}

void Enemy::CheckNextNode()
{
    if (m_vNodesPath.size() <= 1)
        return;

    Node<Tile>* current = m_vNodesPath[0];
    Node<Tile>* next = m_vNodesPath[1];

    if (next->data->IsTempLock() && next->data->mEntity != this)
    {
        mIsWaiting = true;
        mToStop = true;
        mWaitProgress = mWaitTiming;

        return;
    }

    // lock uniquement la vraie nextNode
    next->data->SetTempLock(true, this);
    current->data->SetTempLock(true, this);

    if (UtilsPathfinding::IsInside(next->data->mPos, m_pOwner->transform.GetWorldPosition()))
    {
        current->data->SetTempLock(false, nullptr);

        mCurrentTabPos = gce::Vector3f32(next->data->mPos.col, next->data->mPos.row, next->data->mPos.height);

        // avancer dans le chemin
        m_vNodesPath.erase(m_vNodesPath.begin());

        // lock suivante si encore
        if (m_vNodesPath.size() > 1)
        {
            if (m_vNodesPath[1]->data->mIsLock == true)
            {
                WPosMPos nextPos;

                if (mIsPatrouille)
                {
                    nextPos = m_vPatrouillePoint[mCurrentPos];
                }
                else
                {
                    nextPos = mNextPos;
                }

                if (m_vNodesPath[1]->data->mPos == nextPos)//si dernier bloquer
                {
                    mIsMoving = false;
                    m_vPath.clear();
                    next->data->mIsLock = true;
                    mToStop = true;

                    return;
                }
                else if (FindPath(nextPos))
                {
                    GetPath(nextPos);
                }
            }
            else if (m_vNodesPath[1]->data->mIsObstacle == true)
            {
                mIsMoving = false;
                m_vPath.clear();
                next->data->mIsLock = true;
                mToStop = true;

                return;
            }
            else if (m_vNodesPath[1]->data->IsTempLock()) //si deja temp lock alors attendre
            {
                mIsWaiting = true;

                next->data->mIsLock = true;

                mToStop = true;
                mWaitProgress = mWaitTiming;
            }
            else
            {
                m_vNodesPath[1]->data->SetTempLock(true, this);
            }
        }
    }
}

void Enemy::ChangePatrouille(WPosMPos currentPos)
{
    bool temp = !mIsPatrouille;

    if (temp == false)
    {
        m_vPatrouillePoint.clear();

        mIsPatrouille = false;
        mIsMoving = false;
        
    }
    else
    {
        if (mIsMoving)
            return;

        mIsPatrouille = true;
        m_vPatrouillePoint.push_back(currentPos);
    }
}

void Enemy::AddPointToPath(WPosMPos newPos)
{
    m_vPatrouillePoint.push_back(newPos);
}

void Enemy::Move(WPosMPos newPos)
{
    if (mIsMoving)
    {
        m_pRoom->ResetByEntity(this);
    }

    if (FindPath(newPos))
    {
        mIsMoving = true;
        Node<Tile>* pCurrentNode = m_pRoom->GetNodeWithTabPos(mCurrentTabPos);
        pCurrentNode->data->mIsLock = false;
        pCurrentNode->data->SetTempLock(true, this);

        mCurrentTargetPos = {newPos.x, newPos.y, newPos.z};

        GetPath(newPos);
    }
}

void Enemy::CollisionEnter(GameObject* other)
{
    if (other->GetName() == "Bullet")
    {
        mHaveTakenDamage = true;
        TakeDamage(other->GetScript<Bullets>()->GetDamage());
    }
}

void Enemy::OnStartFight()
{
    ChangePatrouille(WPosMPos());
    UnsetTarget();
}

void Enemy::SetRoom(Room* pRoom)
{
    m_pRoom = pRoom;

    m_pRoom->AddEnemy(this);
}

void Enemy::StopMoving()
{
    mStopMoving = true;

    UnsetTarget();
}

void Enemy::ContinueMoving()
{
    mStopMoving = false;
}

void Enemy::StartFight()
{
    m_pSM->Transit("Fight");
}