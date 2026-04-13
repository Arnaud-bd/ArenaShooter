#include "PM.h"
#include "ArenaShooterManager.h"
#include "NormalBullets.h"
#include "PMSkill.h"
#include "Player.h"

void PM::Start()
{
	Weapon::Start();

	mWeaponIndex = 1;

	m_pGeo = gce::GeometryFactory::CreateCubeGeo();
	mAmmo = 60;
	mMaxAmmo = mAmmo;

	mShootingPace = 0.2f;
	mShootingTimer = mShootingPace;

	GameObject* go = GameManager::GetFirstGameObjectWithName("Player", m_pOwner->GetScene());
	Player* player = go->GetScript<Player>();

	m_pSkill = new PMSkill();
	m_pSkill->InitSkill(5.f, 45.f, 60, this, player);

	mMuzzleDistance = 0.5f;
	mMuzzleHeightOffset = 0.11f;

	Vector3f32 localForward = m_pOwner->transform.GetLocalForward();
	m_pProjectileSpawn->transform.SetLocalPosition(localForward * mMuzzleDistance + Vector3f32{ 0, mMuzzleHeightOffset, 0 });

}

void PM::Update()
{
	Weapon::Update();
}

void PM::OnCollectWeapon()
{
	m_pOwner->transform.SetLocalPosition({ -0.2f, 0.f, 0.f });
}

void PM::Shoot()
{
	if (mShootingTimer < mShootingPace)
		return;

	if (mAmmoLock == false)
	{
		if (mAmmo == 0)
			return;

		mAmmo--;
	}

	mShootingTimer = 0.0f;

	gce::GameObject& bullet = ArenaShooterManager::GetInstance()->CreateBullet();
	bullet.transform.SetWorldScale({ 0.5f, 0.5f, 0.5f });
	bullet.transform.SetWorldPosition(m_pProjectileSpawn->transform.GetWorldPosition());
	bullet.transform.SetForward(m_pProjectileSpawn->transform.GetWorldForward());
	NormalBullets* bulletScript = bullet.AddScript<NormalBullets>();
	bulletScript->SetDamage(5);
}