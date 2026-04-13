#pragma once

#include "Skill.h"

using namespace gce;

class RocketLauncherSkill : public Skill
{

public:
	void OnActivateSkill() override;
	void OnEndingSkill() override;
};

