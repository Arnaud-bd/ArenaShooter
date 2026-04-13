#include "Tile.h"

#include <iostream>
#include "Entity.h"

void Tile::OnInitialize()
{
	
}

void Tile::OnUpdate()
{
	if (mIsTempLock == false)
		return;

	mTimerProgress -= GameManager::DeltaTime();

	if (mTimerProgress <= 0)
	{
		SetTempLock(false, nullptr);
	}
}

void Tile::SetTempLock(bool isTempLock, Entity* newEntity)
{
	mIsTempLock = isTempLock;

	mEntity = newEntity;
}

void Tile::SetColor(gce::Color color)
{

}

gce::Vector3f32 Tile::GetPosition() const
{
	return { mPos.x, mPos.y, mPos.z };
}

void Tile::SetPosition(float x, float y, float z)
{
	mPos.x = x;
	mPos.y = y;
	mPos.z = z;
}