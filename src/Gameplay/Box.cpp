#include "Box.h"
#include "ArenaShooterManager.h"
#include "LifeKit.h"
#include "MapLoader.hpp"
#include "Room/Room.h"

void DestructibleBox::Update()
{

}

void DestructibleBox::CollisionEnter(GameObject* other)
{
	cstr name = other->GetName();

	if (name == "Bullet")
	{
		DropLoot();

		ArenaShooterManager::GetInstance()->GetCurrentRoom()->RemoveGameObject(m_pOwner);

		m_pOwner->Destroy();
	}
}

void DestructibleBox::DropLoot()
{
	int chance = rand() % 5;

	if (chance != 0)
		return;

	Vector3f32 myPos = m_pOwner->transform.GetWorldPosition();

	//Lifekit par terre
	std::vector<GameObject*> kit = MapLoader::LoadMap(RES_PATH"res/JSON/Lifekit-json.json", m_pOwner->GetScene());
	MeshRenderer* kitRenderer = kit[0]->AddComponent<MeshRenderer>();

	kit[0]->AddScript<LifeKit>();
	kit[0]->transform.SetWorldPosition({ myPos.x, 0.5f, myPos.z });

	SphereCollider* kitBox = kit[0]->AddComponent<SphereCollider>();
	kitBox->localScale = 4;

	ArenaShooterManager::GetInstance()->GetCurrentRoom()->AddGameObject(kit[0]);
}