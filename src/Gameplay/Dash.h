#pragma once

#include "Skill.h"

using namespace gce;

class Dash : public Skill
{

public:
	void OnActivateSkill() override;
	void OnEndingSkill() override;

};

