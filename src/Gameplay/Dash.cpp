#include "Dash.h"
#include "Player.h"

void Dash::OnActivateSkill()
{
	m_pPlayer->AddSpeedMultiplier(100);
}

void Dash::OnEndingSkill()
{
	m_pPlayer->AddSpeedMultiplier(-100);
}
