#include "ShowScene.h"
#include "MapLoader.hpp"

void ShowScene::Start()
{
	m_pPso = new D12PipelineObject(SHADERS.VERTEX, SHADERS.PIXEL, SHADERS.HULL, SHADERS.DOMAIN_, SHADERS.ROOT_SIGNATURE);
	m_pCameraOwner = &m_pCamera->GetOwner();
	mCurrentSpeed = mSpeed;

	//-----------------------------------------------//

	m_pRessourceManager = new RessourceManager();
	m_pRessourceManager->InitRM();

	std::vector<GameObject*> geo = MapLoader::LoadMap(RES_PATH"res/JSON/spider.json", m_pScene);

	Texture* texture = new Texture("res/Textures/spidy_classic_initialShadingGroup_OcclusionRoughnessMetallic.png");

	geo[0]->GetComponent<MeshRenderer>()->SetAmbientTexture(texture);

}

void ShowScene::Update()
{
	if (m_pModel == nullptr && mSetGeo == false)
	{
		m_pModel = GameManager::GetFirstGameObjectWithName("spidy_classic", m_pScene);

		if (m_pModel != nullptr)
		{
			mSetGeo = true;
		}
	}

	//Deplacement camera
	if (GetKey(Keyboard::Q) && m_pModel != nullptr)
	{
		m_pModel->transform.WorldRotate({ 0.f, mCurrentSpeed, 0.f });
	}
	if (GetKey(Keyboard::D) && m_pModel != nullptr)
	{
		m_pModel->transform.WorldRotate({ 0.f, -mCurrentSpeed, 0.f });
	}

	if (GetKey(Keyboard::Z) && m_pModel != nullptr)
	{
		m_pModel->transform.WorldRotate({ mCurrentSpeed, 0.f, 0.f });
	}
	if (GetKey(Keyboard::S) && m_pModel != nullptr)
	{
		m_pModel->transform.WorldRotate({ -mCurrentSpeed, 0.f, 0.f });
	}

	if (GetKeyDown(Keyboard::LSHIFT))
	{
		mCurrentSpeed = mMaxSpeed;
	}
	else if (GetKeyUp(Keyboard::LSHIFT))
	{
		mCurrentSpeed = mSpeed;
	}

	if (GetKey(Keyboard::I) && coups != nullptr)
	{
		coups->transform.LocalRotate({mCurrentSpeed, 0, 0});
	}

	// Zoom souris
		if (GetKey(Keyboard::UP_ARROW))
		{
			mCurrentZPos += 0.4f;
		}
	if (GetKey(Keyboard::DOWN_ARROW))
	{
		mCurrentZPos -= 0.4f;
	}

	if (mCurrentZPos > 0)
		mCurrentZPos = 0.f;
	else if (mCurrentZPos < -30)
		mCurrentZPos = -30.f;

	m_pCameraOwner->transform.SetLocalPosition(Vector3f32(0.f, 0.f, mCurrentZPos));
}