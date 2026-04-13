#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(ArenaCamera, ScriptFlag::Start | ScriptFlag::Update)

private:
	GameObject* m_pObject = nullptr;

public:
	void Start();

	void Update();

	Vector3f32 GetPos() { return m_pObject->transform.GetWorldPosition(); }

	END_SCRIPT
