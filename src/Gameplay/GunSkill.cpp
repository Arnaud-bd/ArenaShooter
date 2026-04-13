#include "GunSkill.h"
#include "Player.h"

void GunSkill::OnActivateSkill()
{
	m_pPlayer->AddSpeedMultiplier(4);
}

void GunSkill::OnEndingSkill()
{
	m_pPlayer->AddSpeedMultiplier(-4);
}

