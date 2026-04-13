#include "Entity.h"
#include "Pathfinding/UtilsPathfinding.h"

void Entity::Start()
{
	mTarget.isSet = false;
}

void Entity::Update()
{
	float dt = GameManager::DeltaTime();
	float distance = dt * mSpeed;
	gce::Vector3f32 translation = { distance * mDirection.x, distance * mDirection.y, distance * mDirection.z };
	//mShape.move(translation);
	m_pOwner->transform.SetWorldPosition(m_pOwner->transform.GetWorldPosition() + gce::Vector3f32(translation.x, translation.y, translation.z));

	if (mTarget.isSet)
	{
		float x1 = m_pOwner->transform.GetWorldPosition().x;
		float y1 = m_pOwner->transform.GetWorldPosition().y;
		float z1 = m_pOwner->transform.GetWorldPosition().z;

		float x2 = x1 + mDirection.x * mTarget.distance;
		float y2 = y1 + mDirection.y * mTarget.distance;
		float z2 = z1 + mDirection.z * mTarget.distance;

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			m_pOwner->transform.SetWorldPosition({ mTarget.position.x, mTarget.position.y, mTarget.position.z - 0.5f });
			mDirection = gce::Vector3f32(0.f, 0.f, 0.0f);
			mTarget.isSet = false;
		}
	}
}

bool Entity::GoToDirection(int x, int y, float z, float speed)
{
	gce::Vector3f32 position = m_pOwner->transform.GetWorldPosition();
	gce::Vector3f32 direction = gce::Vector3f32(x - position.x, y - position.y, z - position.z);

	bool success = UtilsPathfinding::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, direction.z, speed);

	return true;
}

bool Entity::GoToPosition(int x, int y, float z, float speed)
{
	if (GoToDirection(x, y, z, speed) == false)
		return false;

	gce::Vector3f32 position = m_pOwner->transform.GetWorldPosition();

	mTarget.position = gce::Vector3f32(x, y, z + 0.5f);
	mTarget.distance = UtilsPathfinding::GetDistance(position.x, position.y, position.z, x, y, z + 0.5f);
	mTarget.isSet = true;

	float rotate = std::atan2f(mDirection.x, mDirection.z);

	m_pOwner->transform.SetWorldRotation({0.f, rotate + PI/2, 0.f});

	return true;
}

bool Entity::GoToPosition(Vector3f32 pos, float speed)
{
	if (GoToDirection(pos.x, pos.y, pos.z, speed) == false)
		return false;

	gce::Vector3f32 position = m_pOwner->transform.GetWorldPosition();

	mTarget.position = gce::Vector3f32(pos.x, pos.y, pos.z + 0.5f);
	mTarget.distance = UtilsPathfinding::GetDistance(position.x, position.y, position.z, pos.x, pos.y, pos.z + 0.5f);
	mTarget.isSet = true;

	float rotate = std::atan2f(mDirection.x, mDirection.z);

	m_pOwner->transform.SetWorldRotation({ 0.f, rotate + PI / 2, 0.f });

	return true;
}

void Entity::SetDirection(float x, float y, float z, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = gce::Vector3f32(x, y, z);
	mTarget.isSet = false;
}

void Entity::UnsetTarget()
{
	mTarget.isSet = false;
	mDirection = { 0.f, 0.f, 0.f };
}