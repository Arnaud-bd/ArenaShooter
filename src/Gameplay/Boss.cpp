#include "Boss.h"
#include "StateMachine.h"
#include "Player.h"
#include "Pathfinding/UtilsPathfinding.h"
#include "Room/Room.h"
#include "Rock.h"
#include "RessourceManager.h"
#include "Head.h"
#include "MapLoader.hpp"

void Boss::Attack()
{
    mCooldownProgress = mCooldownTime;

    float distance = UtilsPathfinding::GetDistance(m_pOwner->transform.GetWorldPosition(), m_pPlayer->GetOwner()->transform.GetWorldPosition());

    if (distance >= 10)
        m_pSM->Transit("Throw");
    else
        m_pSM->Transit("Swing");
}

void Boss::OnStartSwing()
{
    StopMoving();

    if (mNeck != nullptr)
    {
        mNeck->transform.LocalRotate({ PI / 2.f, 0.f, 0.f });
    }

    mCurretnRotate = 0.f;

    mHasDealDamageSwing = false;

    //debug
    BoxCollider* box = mNeck->GetComponent<BoxCollider>();
    box->m_localOffSet = { 7.f, 0.f, 0.f };
}

void Boss::OnUpdateSwing()
{
    if (mCurretnRotate >= PI)
    {
        m_pSM->Transit("Fight");
    }
    else
    {
        mCurretnRotate += PI / 2.f * GameManager::DeltaTime();

        mNeck->transform.LocalRotate({ 0.f, 0.f, PI / 2.f * GameManager::DeltaTime() });
    }

    
}

void Boss::OnEndSwing()
{
    mNeck->transform.SetLocalRotation({ 0.f, 0.f, 0.f });

    ContinueMoving();
}

void Boss::OnStartThrow()
{
    StopMoving();

    mFactor = 1;
    mCurrentRotateThrow = 0.f;
}

void Boss::OnUpdateThrow()
{
    if (mCurrentRotateThrow >= PI / 2.f && mFactor == 1)
    {
        //Create Rock and go back up
        CreateRock();

        mFactor = -1;
        mCurrentRotateThrow = 0.f;
    }
    else if (mCurrentRotateThrow >= PI / 2.f && mFactor == -1)
    {
        m_pSM->Transit("Fight");
    }
    else
    {
        mCurrentRotateThrow += PI / 2.f * GameManager::DeltaTime();

        mNeck->transform.LocalRotate({ 0.f, 0.f, PI / 2.f * GameManager::DeltaTime() * mFactor });
    }
}

void Boss::OnEndThrow()
{
    ContinueMoving();

    mRock->RemoveParent();

    Vector3f32 start = mRock->transform.GetWorldPosition();
    Vector3f32 target = m_pPlayer->GetOwner()->transform.GetWorldPosition();

    Vector3f32 delta = target - start;

    // Temps de vol choisi (ex: 1 seconde)
    float t = 1.0f;

    // Vitesse horizontale
    Vector3f32 velocity;
    velocity.x = delta.x / t;
    velocity.z = delta.z / t;

    // Vitesse verticale CORRECTE
    velocity.y = (delta.y + 0.5f * 9.81f * t * t) / t;


    PhysicComponent* pc = mRock->GetComponent<PhysicComponent>();
    pc->SetActive(true);
    pc->SetGravityActive(true);
    pc->SetVelocity(velocity);
}

void Boss::CreateRock()
{
    GameObject* rock = MapLoader::LoadMap(RES_PATH"res/JSON/boulet.json", m_pOwner->GetScene())[0];    

    rock->AddScript<Rock>();
    rock->transform.SetLocalScale({ 5.f, 5.f, 5.f });
    rock->AddComponent<SphereCollider>()->isTrigger = true;

    m_pOwner->SetName("Boss");
    rock->SetParent(*mNeck);
    rock->transform.SetLocalPosition(mHead->transform.GetLocalPosition());
    rock->AddComponent<PhysicComponent>();

    mRock = rock;
}

void Boss::Start()
{
    mCooldownTime = 15.f;

    Enemy::Start();

    mStopDistance = 10;

    mDetecteDistance = 70.f;

    mAttackWithStop = false;

    mValue = 2500;

    SetLife(1000);

    //Swing
    String swing = "Swing";
    m_pSM->AddAction(swing, [this]() {this->OnStartSwing(); }, [this]() {this->OnUpdateSwing(); }, [this]() {this->OnEndSwing(); });

    //Throw
    String nameThrow = "Throw";
    m_pSM->AddAction(nameThrow, [this]() {this->OnStartThrow(); }, [this]() {this->OnUpdateThrow(); }, [this]() {this->OnEndThrow(); });

    //Get body part
    mBody = GameManager::GetFirstGameObjectWithName("boss_body", m_pOwner->GetScene());
    mNeck = GameManager::GetFirstGameObjectWithName("boss_neck", m_pOwner->GetScene());
    mHead = GameManager::GetFirstGameObjectWithName("boss_head", m_pOwner->GetScene());
}

void Boss::Update()
{
    Enemy::Update();

    if (mCurrentHealth <= 150 && mHasHead)
        SpecialAttack();
}

void Boss::CollisionEnter(GameObject* other)
{
    Enemy::CollisionEnter(other);

    if (other->GetName() == "Player" && m_pSM->actualAction == "Swing")
    {
        if (mHasDealDamageSwing == false)
            other->GetScript<Player>()->TakeDamage(25);

        mHasDealDamageSwing = true;
    }
}

void Boss::SpecialAttack()
{
    Vector3f32 headPos = mHead->transform.GetWorldPosition();

    mHead->RemoveParent();    

    Head* head = mHead->AddScript<Head>();

    Vector3f32 direction = m_pPlayer->GetOwner()->transform.GetWorldPosition() - headPos;
    head->SetDirection(direction);    

    mHasHead = false;
    mHead = &GameObject::Create(*m_pOwner->GetScene());
    mHead->transform.SetWorldPosition(headPos);
    mHead->SetParent(*mNeck);
}