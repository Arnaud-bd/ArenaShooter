#include "SeekerBullets.h"
#include "Pathfinding/UtilsPathfinding.h"
#include "Enemy.h"

void SeekerBullets::Start()
{
	Bullets::Start();

	mSeekTime = 0.2f;
	mSeekProcess = 0.0f;

	std::vector<GameObject*> enemies = GameManager::GetAllGameObjectWithTag(EntityType::ENEMY, m_pOwner->GetScene());

	float min = 999999.f;

	for (int i = 0; i < enemies.size(); i++)
	{
		float temp = UtilsPathfinding::GetDistance(enemies[i]->transform.GetWorldPosition(), m_pOwner->transform.GetWorldPosition());

		if (temp < min)
		{
			min = temp;
			m_pTarget = enemies[i];
		}
	}
}

void SeekerBullets::Update()
{
	Bullets::Update();

	mSeekProcess += GameManager::DeltaTime();

	float dt = GameManager::FixedDeltaTime();

	if (mSeekProcess <= mSeekTime || m_pTarget == nullptr)
	{
		m_pOwner->transform.WorldTranslate({ mDirection * mBulletSpeed * dt });
	}
	else
	{
		Vector3f32 targetPos = m_pTarget->transform.GetWorldPosition();
		Vector3f32 dir = targetPos - m_pOwner->transform.GetWorldPosition();
		dir.SelfNormalize();
		m_pOwner->transform.WorldTranslate({dir * mBulletSpeed * dt });
	}
}