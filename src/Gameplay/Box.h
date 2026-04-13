#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(DestructibleBox, ScriptFlag::Update | ScriptFlag::CollisionEnter)

protected:

public:
	void Update();
	void CollisionEnter(GameObject* other);
	void DropLoot();
	END_SCRIPT

