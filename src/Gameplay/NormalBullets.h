#pragma once

#include "Bullets.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(NormalBullets, Bullets, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:


public:
	void Start();
	void Update();

	END_SCRIPT

