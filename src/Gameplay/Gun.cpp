#include "Gun.h"
#include "NormalBullets.h"
#include "ArenaShooterManager.h"
#include "GunSkill.h"
#include "Player.h"

void Gun::Start()
{
	Weapon::Start();

	mWeaponIndex = 2;

	m_pGeo = gce::GeometryFactory::CreateCubeGeo();
	mAmmo = 24;
	mMaxAmmo = mAmmo;

	mShootingPace = 0.0f;
	mShootingTimer = mShootingPace;

	GameObject* go = GameManager::GetFirstGameObjectWithName("Player", m_pOwner->GetScene());
	Player* player = go->GetScript<Player>();

	m_pSkill = new GunSkill();
	Weapon* weapon = this;
	m_pSkill->InitSkill(10.f, 30.f, 12, weapon, player);

	mMuzzleDistance = 0.32f;
	mMuzzleHeightOffset = 0.075f;

	Vector3f32 localForward = m_pOwner->transform.GetLocalForward();
	m_pProjectileSpawn->transform.SetLocalPosition(localForward * mMuzzleDistance + Vector3f32{ 0, mMuzzleHeightOffset, 0 });
}

void Gun::Update()
{
	Weapon::Update();

	if (GetButtonUp(Mouse::LEFT))
		mHasShot = false;
}

void Gun::OnCollectWeapon()
{
	m_pOwner->transform.SetLocalPosition({ -0.2f, 0.f, 0.2f });
}

void Gun::Shoot()
{
	if (mHasShot)
		return;

	if (mAmmo == 0)
		return;

	if (mShootingTimer < mShootingPace)
		return;

	mShootingTimer = 0.0f;

	mAmmo--;

	gce::GameObject& bullet = ArenaShooterManager::GetInstance()->CreateBullet();
	bullet.transform.SetWorldScale({ 0.5f,0.5f, 0.5f });
	bullet.transform.SetWorldPosition(m_pProjectileSpawn->transform.GetWorldPosition());
	bullet.transform.SetForward(m_pProjectileSpawn->transform.GetWorldForward());
	NormalBullets* bulletScript = bullet.AddScript<NormalBullets>();
	bulletScript->SetDamage(10);

	mHasShot = true;
}