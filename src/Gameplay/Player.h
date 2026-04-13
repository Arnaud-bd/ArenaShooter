#pragma once

#include "Engine.h"
#include "Render.h"

#include "Entity.h"
#include "LifeKit.h"
#include "Weapon.h"

using namespace gce;
	
DECLARE_CHILD_SCRIPT(Player, Entity, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionExit)

enum Statut 
{
	Normal,
	Poisoned,
	Stun
};

private:
	GameObject* m_pCameraOwner = nullptr;
	GameObject* m_pWeaponSocket = nullptr;
	GameObject* m_pClosestObject = nullptr;
	Weapon* m_pCurrentWeapon = nullptr;
	Skill* m_pDash = nullptr;

	Vector2i32 mCenter;
	Vector2f32 mMouseDire = { 0, 0 };

	float mSpeed;
	float mSpeedMultiplier;
	float mSpeedDivider;
	float mJumpSpeed = 150.0f;
	float mSensitivity = 0.02f;
	bool mCanMove = true;

	int mLifeKitsNumber = 0;
	bool mCursorLock = false;

	Statut mStatut = Normal;

	//Endurance
	int mPA;
	int mMaxPA;

	bool mIsRunning = false;

	float mPA_Time = 0.2f;
	float mPA_Progress = 0.f;

	//Poisoned
	int mPoisonedDamageCount;

	float mPoisonedDamageTime = 1.f;
	float mPoisonedDamageProgress = 0.f;

	int mPoisonDamage = 5;

	//Stun
	float mStunTime = 2.f;
	float mStunProgress = 2.f;

	//UI
	int mScore = 0;

	Weapon* m_pPrimaryWeapon = nullptr;
	Weapon* m_pSecondaryWeapon = nullptr;

	int mCurrentLifeIndex = 0;

public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);
	void CollisionExit(GameObject* other);

	void StatutUpdate();

	void SetMoving(bool moving) { mCanMove = moving; }
	void Move();
	void Shoot();
	void FollowMouse();
	void UpdateView();
	void UseWeaponSkill();
	void PlayerDash();
	void SwitchWeapon();
	void Jump();
	void AddSpeedMultiplier(float multiplier) { mSpeedMultiplier += multiplier; }
	void SetSpeedDivider(float devider) { mSpeedDivider = devider; }
	void CollectItem();
	void DropCurrentWeapon();
	void UseLifeKit();
	void HandlePA();
	void SetRunningState(bool state);
	void SetStatut(Statut newStatue);
	void Reset();
	void StartVariables();
	void TakeDamage(int dmg) override;

	Weapon* GetCurrentWeapon() { return m_pPrimaryWeapon; }
	Weapon* GetInvWeapon() { return m_pSecondaryWeapon; }
	int GetScore() { return mScore; }
	int GetKit() { return mLifeKitsNumber; }

	void AddScore(int score) { mScore += score; }

	GameObject* GetObj() { return m_pOwner; }
	Vector3f32 const GetForward() const { return m_pOwner->transform.GetWorldForward(); }
	void SetCursorLock();

	END_SCRIPT