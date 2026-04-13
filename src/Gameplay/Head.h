#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(Head, ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:
	Vector3f32 mDirection;

public:
	void Update();
	void CollisionEnter(GameObject* other);
	
	void SetDirection(Vector3f32 dir) { mDirection = dir; }

	END_SCRIPT