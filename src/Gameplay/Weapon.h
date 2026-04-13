#pragma once

#include "Engine.h"
#include "Render.h"
#include "Skill.h"

using namespace gce;

DECLARE_SCRIPT(Weapon, ScriptFlag::Start | ScriptFlag::Update)

private:

	virtual void OnCollectWeapon() = 0;

protected:
	Geometry* m_pGeo = nullptr;

	gce::GameObject* m_pProjectileSpawn = nullptr;

	Skill* m_pSkill = nullptr;

	int mAmmo = 0;
	int mMaxAmmo = 0;
	bool mAmmoLock = false;

	float mShootingPace = 0.f;
	float mShootingTimer = 0.f;

	float mMuzzleDistance = 0.f;
	float mMuzzleHeightOffset = 0.f;

	int mWeaponIndex; //0=RocketL; 1=PM; 2=Gun

public:
	virtual void Shoot() = 0;

	void GiveAmmo(int ammo);
	void SetShootingMultiplier(float multiplier);
	void UseSkill();
	void ChangeShootingState() { mAmmoLock = !mAmmoLock; }
	void Start();
	void Update();
	void EndSkill();
	bool HasSkillActive();
	void DropWeapon();
	void CollectWeapon(GameObject* parent);
	void SetPosition(Vector3f32 position);

	gce::GameObject* GetOwner() { return m_pOwner; }
	gce::Vector3f32 GetPos();
	Skill* GetSkill() { return m_pSkill; }
	int GetIndex() { return mWeaponIndex; }
	int GetAmmos() { return mAmmo; }

	END_SCRIPT