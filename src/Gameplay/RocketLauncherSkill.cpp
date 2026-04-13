#include "RocketLauncherSkill.h"
#include "Player.h"
#include "RocketLauncher.h"

void RocketLauncherSkill::OnActivateSkill()
{
	RocketLauncher* RL = dynamic_cast<RocketLauncher*>(m_pMyWeapon);
	RL->AlternativeShot();
}

void RocketLauncherSkill::OnEndingSkill()
{
}

