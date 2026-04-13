#pragma once

#include "Skill.h"

using namespace gce;

class GunSkill : public Skill
{

public:
	void OnActivateSkill() override;
	void OnEndingSkill() override;
};
