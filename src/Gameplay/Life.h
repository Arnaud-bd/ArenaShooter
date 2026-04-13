#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(Life, ScriptFlag::Update)

protected:
int mCurrentHealth;
int mMaxHealth;

public:
	Life();

	void SetLife(int maxHealth);
	int GetHP();

	bool IsDead();
	virtual void TakeDamage(int dmg);

	END_SCRIPT

