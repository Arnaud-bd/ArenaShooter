#include "PMSkill.h"
#include "Player.h"

void PMSkill::OnActivateSkill()
{
	m_pPlayer->SetMoving(false);
	m_pMyWeapon->ChangeShootingState();
	m_pMyWeapon->SetShootingMultiplier(3.0f);
}

void PMSkill::OnEndingSkill()
{
	m_pPlayer->SetMoving(true);
	m_pMyWeapon->ChangeShootingState();
	m_pMyWeapon->SetShootingMultiplier(1.f / 3.f);
}
