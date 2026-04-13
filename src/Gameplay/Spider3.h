#pragma once

#include "Enemy.h"

using namespace gce;

DECLARE_CHILD_SCRIPT(Spider3, Enemy, ScriptFlag::Start | ScriptFlag::Update | ScriptFlag::CollisionEnter)

private:
	void Attack() override;

public:
	void Start();
	void Update();
	void CollisionEnter(GameObject* other);

	END_SCRIPT