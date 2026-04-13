#include "Skill.h"
#include "Weapon.h"
#include "Player.h"
#include "Audio.h"

using namespace gce;


void Skill::InitSkill(float duration, float cooldown, int numAmmo, Weapon* weapon, Player* player)
{
	mSkillDuration = duration;
	mActiveTimer = 0.f;
	mNumOfAmmoGiven = numAmmo;

	mSkillCooldown = cooldown;
	mCooldownTimer = 0.f;

	m_pPlayer = player;
	m_pMyWeapon = weapon;
}

void Skill::ActivateSkill(bool usAudio)
{
	if (mIsCoolDown)
		return;

	if (mIsActive)
		return;

	if (usAudio)
		AudioUse::Play("Skill");

	mIsActive = true;
	mActiveTimer = mSkillDuration;

	if (m_pMyWeapon != nullptr)
		m_pMyWeapon->GiveAmmo(mNumOfAmmoGiven);

	OnActivateSkill();
}

void Skill::UpdateSkill()
{
	if (mIsActive)
		UpdateActive();

	if (mIsCoolDown)
		UpdateCooldown();
}

void Skill::UpdateCooldown()
{
	mCooldownTimer -= GameManager::DeltaTime();

	if (mCooldownTimer <= 0.f)
	{
		mIsCoolDown = false;
	}
}

void Skill::UpdateActive()
{
	mActiveTimer -= GameManager::DeltaTime();

	if (mActiveTimer <= 0.f)
	{
		mIsActive = false;
		mIsCoolDown = true;
		mCooldownTimer = mSkillCooldown;
		OnEndingSkill();
	}
}

void Skill::EndSkill()
{
	mActiveTimer = 0.0f;

	UpdateSkill();
}
