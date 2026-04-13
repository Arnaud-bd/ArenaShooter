#pragma once

#include "Engine.h"
#include "Render.h"

using namespace gce;

DECLARE_SCRIPT(LifeKit, ScriptFlag::Start)

private:
	Geometry* m_pGeo = nullptr;


public:
	void Start();

	void GetCollected();

	END_SCRIPT