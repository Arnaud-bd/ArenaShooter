#pragma once

#include "Weapon.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Gun, Weapon, ScriptFlag::Start | ScriptFlag::Update)

private:

	bool mHasShot = false;

	void OnCollectWeapon() override;

public:
	void Shoot() override;

	void Start();
	void Update();

	END_SCRIPT

