#pragma once

#include "Skill.h"

using namespace gce;

class PMSkill : public Skill
{

public:
	void OnActivateSkill() override;
	void OnEndingSkill() override;
};