#include "Spider2.h"
#include "StateMachine.h"
#include "Player.h"
#include "Pathfinding/UtilsPathfinding.h"
#include "ArenaShooterManager.h"
#include "Room/Room.h"

void Spider2::Attack()
{
    mCooldownProgress = mCooldownTime;

    m_pSM->Transit("Rush");
}

void Spider2::OnStartRush()
{

    m_pRoom->ResetByEntity(this);

    Vector3f32 posPlayer = m_pPlayer->GetOwner()->transform.GetWorldPosition();
    mBeginPos = m_pOwner->transform.GetWorldPosition();

    mRushDirection = posPlayer - mBeginPos;

    mRushDirection.y = 0.f;

    mCooldownRushProgress == mCooldownRushTime;

    mIsInRush = true;

    UnsetTarget();
}

void Spider2::OnUpdateRush()
{

    mCooldownRushProgress -= GameManager::DeltaTime();

    if (mCooldownRushProgress <= 0)
    {
        m_pOwner->transform.WorldTranslate(mRushDirection * mRushSpeed * GameManager::DeltaTime());

        Vector3f32 posOwner = m_pOwner->transform.GetWorldPosition();

        float distanceRush = UtilsPathfinding::GetDistance(mBeginPos, posOwner);

        if (distanceRush > mDistanceRush)
        {
            m_pSM->Transit("Fight");
        }
    }
}

void Spider2::Start()
{
    mCooldownTime = 10.f;

    Enemy::Start();

    mStopDistance = 8;

    mValue = 120;

    SetLife(130);

    //Rush
    String rush = "Rush";
    m_pSM->AddAction(rush, [this]() {this->OnStartRush(); }, [this]() {this->OnUpdateRush(); });

}

void Spider2::Update()
{
    Enemy::Update();
}

void Spider2::CollisionEnter(GameObject* other)
{
    Enemy::CollisionEnter(other);

    if (mIsInRush)
    {
        if (other->GetName() == "Player")
        {
            other->GetScript<Player>()->TakeDamage(30);
        }

        m_pSM->Transit("Fight");
    }
}