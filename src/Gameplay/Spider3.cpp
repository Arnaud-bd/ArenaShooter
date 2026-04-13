#include "Spider3.h"
#include "Room/Room.h"
#include "Pathfinding/Tile.h"
#include "Player.h"

#include "Pathfinding/UtilsPathfinding.h"
#include "ArenaShooterManager.h"
#include "Web.h"

void Spider3::Attack()
{
    mCooldownProgress = mCooldownTime;
    //shoot
    Geometry* demi_sphere = RessourceManager::GetGeoWithName("demi_sphere");

    if (demi_sphere == nullptr)
    {
        demi_sphere = GeometryFactory::CreateHalfSphereGeo(0.5f, 32, 32);
        RessourceManager::AddGeometry(demi_sphere, "demi_sphere");
    }

    GameObject& gameObject = GameObject::Create(*m_pOwner->GetScene());
    gameObject.SetName("Web");
    MeshRenderer* mesh = gameObject.AddComponent<MeshRenderer>();
    mesh->SetGeometry(demi_sphere);
    BoxCollider* bc = gameObject.AddComponent<BoxCollider>();
    bc->isTrigger = true;
    bc->localScale = { 1.f, 0.2f, 1.f };
    Web* web = gameObject.AddScript<Web>();
    gameObject.transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition());

    Vector3f32 pos = m_pPlayer->GetOwner()->transform.GetWorldPosition();
    pos.y = 0.f;

    web->GoToPosition(pos);    
}

void Spider3::Start()
{
    Enemy::Start();

    mStopDistance = 5;

    mValue = 80;

    SetLife(80);
}

void Spider3::Update()
{
    Enemy::Update();
}

void Spider3::CollisionEnter(GameObject* other)
{
    Enemy::CollisionEnter(other);
}