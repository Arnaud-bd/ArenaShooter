#pragma once

#include "Weapon.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(PM, Weapon, ScriptFlag::Start | ScriptFlag::Update)

private:

	void OnCollectWeapon() override;

public:
	void Shoot() override;
	void Start();
	void Update();

	END_SCRIPT

