#pragma once

#include "Engine.h"
#include "Render.h"
#include "Entity.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Web, Entity, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter | ScriptFlag::CollisionExit)

protected:
	float mCooldownLifeProgress = 5.f;

public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);
	void CollisionExit(GameObject* other);

	END_SCRIPT
