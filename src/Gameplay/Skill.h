#pragma once

#include "Engine.h"
#include "Render.h"

class Weapon;
class Player;

using namespace gce;

class Skill
{

protected:
	Player* m_pPlayer = nullptr;
	Weapon* m_pMyWeapon = nullptr;

	float mSkillDuration;
	float mActiveTimer;

	float mCooldownTimer;
	float mSkillCooldown;

	int mNumOfAmmoGiven;

	bool mIsActive;
	bool mIsCoolDown;

	virtual void OnActivateSkill() {};
	virtual void OnEndingSkill() {};

public:
	void InitSkill(float duration, float cooldown, int numAmmo, Weapon* weapon, Player* player);
	
	void ActivateSkill(bool usAudio = false);
	void EndSkill();
	bool SkillIsActive() { return mIsActive; }
	void UpdateSkill();
	void UpdateCooldown();
	void UpdateActive();

	float GetSkillCouldown() { return mCooldownTimer; }
};
