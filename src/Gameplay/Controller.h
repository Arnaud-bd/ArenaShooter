#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

class Player;

DECLARE_SCRIPT(Controller, ScriptFlag::Start | ScriptFlag::Update)

private:
	Player* m_pPlayer = nullptr;

public:
	void Start();

	void Update();

	void SetPlayer(Player* player) { m_pPlayer = player; }

	END_SCRIPT
