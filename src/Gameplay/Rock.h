#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(Rock, ScriptFlag::Update | ScriptFlag::CollisionEnter)

protected:

public:
	void Update();
	void CollisionEnter(GameObject* other);

	END_SCRIPT
