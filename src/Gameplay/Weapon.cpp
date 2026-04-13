#include "Weapon.h"
#include "ArenaShooterManager.h"
#include "Room/Room.h"

void Weapon::Start()
{
	gce::Vector3f32 position = m_pOwner->transform.GetLocalPosition();

	m_pProjectileSpawn = &GameObject::Create(*m_pOwner->GetScene());
	m_pProjectileSpawn->SetParent(*m_pOwner);
	m_pProjectileSpawn->transform.SetLocalRotation({ m_pOwner->transform.GetLocalRotation()});
	m_pProjectileSpawn->SetName("ProjectileSpawn");


	m_pOwner->SetName("Weapon");
}

void Weapon::SetPosition(Vector3f32 position)
{
	m_pOwner->transform.SetWorldPosition(position);
}

void Weapon::SetShootingMultiplier(float multiplier)
{
	mShootingPace /= multiplier;
}

void Weapon::UseSkill()
{
	m_pSkill->ActivateSkill(true);
}

void Weapon::Update()
{
	float dt = GameManager::DeltaTime();

	m_pSkill->UpdateSkill();

	mShootingTimer += dt;

	if (mShootingTimer >= mShootingPace)
		mShootingTimer = mShootingPace;
}

void Weapon::EndSkill()
{
	if (m_pSkill == nullptr)
		return;

	m_pSkill->EndSkill();
}

bool Weapon::HasSkillActive()
{
	return m_pSkill->SkillIsActive();
}

void Weapon::GiveAmmo(int ammo)
{
	mAmmo += ammo;

	if (mAmmo > mMaxAmmo)
		mAmmo = mMaxAmmo;
}

void Weapon::DropWeapon()
{
	if(m_pOwner->HasParent())
		m_pOwner->RemoveParent();

	SphereCollider* gunBox = m_pOwner->AddComponent<SphereCollider>();
	gunBox->localScale = 4;
	Vector3f32 myPos = m_pOwner->transform.GetWorldPosition();
	m_pOwner->transform.SetWorldPosition({myPos.x, 0.0f, myPos.z});
	EndSkill();
	ArenaShooterManager::GetInstance()->GetCurrentRoom()->AddGameObject(m_pOwner);
}

void Weapon::CollectWeapon(GameObject* parent)
{
	m_pOwner->RemoveComponent<BoxCollider>();
	m_pOwner->SetParent(*parent);
	m_pOwner->transform.SetLocalRotation(parent->transform.GetLocalRotation());

	OnCollectWeapon();
}

Vector3f32 Weapon::GetPos()
{
	return m_pOwner->transform.GetWorldPosition();
}