#pragma once

#include "Engine.h"
#include "Render.h"

#include "RessourceManager.h"

using namespace gce;

DECLARE_SCRIPT(ShowScene, ScriptFlag::Start | ScriptFlag::Update)

private:
	RessourceManager* m_pRessourceManager = nullptr;

	D12PipelineObject* m_pPso;
	GameObject* m_pCameraOwner;
	GameObject* m_pModel = nullptr;
	std::vector<GameObject*> m_vGameObject;

	float mSpeed = 0.1f;
	float mCurrentSpeed;
	float mMaxSpeed = 0.4f;
	float mCurrentZPos = -10.f;
	bool mSetGeo = false;

	GameObject* coups = nullptr;

public:
	Scene* m_pScene;
	Camera* m_pCamera;

	void Start();
	void Update();

	END_SCRIPT