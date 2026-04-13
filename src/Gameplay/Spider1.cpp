#include "Spider1.h"
#include "Room/Room.h"
#include "Pathfinding/Tile.h"
#include "Player.h"

#include "Pathfinding/UtilsPathfinding.h"
#include "ArenaShooterManager.h"

void Spider1::Attack()
{
    mCooldownProgress = mCooldownTime;
    m_pPlayer->SetStatut(Player::Statut::Poisoned);
    
}

void Spider1::Start()
{
    Enemy::Start();

    mStopDistance = 4;

    mValue = 50;

    SetLife(100);
}

void Spider1::Update()
{
	Enemy::Update();
}

void Spider1::CollisionEnter(GameObject* other)
{
    Enemy::CollisionEnter(other);
}