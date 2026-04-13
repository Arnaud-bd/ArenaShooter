#include "RocketLauncher.h"
#include "SeekerBullets.h"
#include "ArenaShooterManager.h"
#include "RocketLauncherSkill.h"
#include "Player.h"

void RocketLauncher::Start()
{
	Weapon::Start();

	mWeaponIndex = 0;

	m_pGeo = gce::GeometryFactory::CreateCubeGeo();
	mAmmo = 3;
	mMaxAmmo = mAmmo;

	mShootingPace = 1.5f;
	mShootingTimer = mShootingPace;

	GameObject* go = GameManager::GetFirstGameObjectWithName("Player", m_pOwner->GetScene());
	Player* player = go->GetScript<Player>();

	m_pSkill = new RocketLauncherSkill();
	m_pSkill->InitSkill(0.f, 90.0f,  2, this, player);

	mMuzzleDistance = 0.5f;
	mMuzzleHeightOffset = 0.f;

	Vector3f32 localForward = m_pOwner->transform.GetLocalForward();
	m_pProjectileSpawn->transform.SetLocalPosition(localForward * mMuzzleDistance + Vector3f32{ 0, mMuzzleHeightOffset, 0 });

}

void RocketLauncher::Update()
{
	Weapon::Update();
}

void RocketLauncher::OnCollectWeapon()
{
	m_pOwner->transform.SetLocalPosition({ 0.f, 0.f, 0.5f });

}
void RocketLauncher::Shoot()
{
	if (mAmmo == 0)
		return;

	if (mShootingTimer < mShootingPace)
		return;

	mShootingTimer = 0.0f;

	mAmmo--;

	gce::GameObject& bullet = ArenaShooterManager::GetInstance()->CreateBullet();

	bullet.transform.SetWorldPosition(m_pProjectileSpawn->transform.GetWorldPosition());
	bullet.transform.SetForward(m_pProjectileSpawn->transform.GetWorldForward());
	SeekerBullets* bulletScript = bullet.AddScript<SeekerBullets>();
	bulletScript->SetDamage(120);
}

void RocketLauncher::AlternativeShot()
{
	mAmmo--;

	Vector3f32 forward = m_pProjectileSpawn->transform.GetWorldForward();
	Vector3f32 right = m_pProjectileSpawn->transform.GetWorldRight();
	Vector3f32 position = m_pProjectileSpawn->transform.GetWorldPosition();

	float spread = 0.2f;

	for (int i = -1; i <= 1; i++)
	{
		gce::GameObject& bullet =
			ArenaShooterManager::GetInstance()->CreateBullet();

		bullet.transform.SetWorldPosition(position);

		Vector3f32 dir = forward + right * (i * spread);
		dir.SelfNormalize();

		bullet.transform.SetForward(dir);

		SeekerBullets* bulletScript = bullet.AddScript<SeekerBullets>();
		bulletScript->SetDamage(80);
	}
}