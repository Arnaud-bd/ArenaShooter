#include "ArenaShooterManager.h"
#include "Controller.h"
#include "Player.h"

#include "ArenaUI.h"
#include "ArenaCamera.h"
#include "Gun.h"
#include "RocketLauncher.h"
#include "PM.h"
#include "Spider1.h"

#include "SeekerBullets.h"

#include "MapLoader.hpp"

#include "Room/SalleManager.h"
#include "Audio.h"

ArenaShooterManager* ArenaShooterManager::instance = nullptr;

ArenaShooterManager* ArenaShooterManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ArenaShooterManager();
	}

	return instance;
}

void ArenaShooterManager::Start()
{
	instance = this;

	m_pController = new Controller;
	
	m_pRessourceManager = new RessourceManager();
	m_pRessourceManager->InitRM();

	GameObject& player = GameObject::Create(*m_pScene);

	Vector3f32 playerPos = { m_pCameraOwner->transform.GetWorldPosition().x,
		m_pCameraOwner->transform.GetWorldPosition().y,
		m_pCameraOwner->transform.GetWorldPosition().z, };

	player.transform.SetWorldPosition(playerPos);
	player.AddComponent<BoxCollider>();
	PhysicComponent* playerphysic = player.AddComponent<PhysicComponent>();
	playerphysic->SetBounciness(0.0f);
	m_pPlayer = player.AddScript<Player>();

	m_pCameraOwner->SetParent(*m_pPlayer->GetObj());

	m_pController->SetPlayer(m_pPlayer);

	m_pPso = new D12PipelineObject(SHADERS.VERTEX, SHADERS.PIXEL, SHADERS.HULL, SHADERS.DOMAIN_, SHADERS.ROOT_SIGNATURE);

	GameObject& ui = GameObject::Create(*m_pScene);
	m_pUI = ui.AddScript<ArenaUI>();
	m_pUI->SetWinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pUI->m_pScene = m_pScene;
	m_pUI->m_pCamera = m_pCamera;
	m_pUI->SetPlayer(m_pPlayer);

	m_pRoomManager = m_pOwner->AddScript<SalleManager>();
	m_pRoomManager->SetPlayer(m_pPlayer);	

	//Sound
	const std::wstring gunFirePath = WRES_PATH L"res/Sounds/gunFire.wav";
	AudioUse::LoadSound("GunFire", gunFirePath.c_str());

	const std::wstring deathPath = WRES_PATH L"res/Sounds/death.wav";
	AudioUse::LoadSound("Death", deathPath.c_str());

	const std::wstring skillPath = WRES_PATH L"res/Sounds/Skill.wav";
	AudioUse::LoadSound("Skill", skillPath.c_str());

	const std::wstring lifeKitPath = WRES_PATH L"res/Sounds/LifeKit.wav";
	AudioUse::LoadSound("LifeKit", lifeKitPath.c_str());

	const std::wstring dashPath = WRES_PATH L"res/Sounds/Dash_player.wav";
	AudioUse::LoadSound("DashPlayer", dashPath.c_str());
}

void ArenaShooterManager::Update()
{
	m_pController->Update();	
}

void ArenaShooterManager::SetCameraOwner(GameObject* pOwner)
{
	m_pCameraOwner = pOwner;

	m_pCameraOwner->AddScript<ArenaCamera>();
}

void ArenaShooterManager::SetGameState(GameState newState)
{
	if (mState == newState)
		return;

	mState = newState;

	m_pPlayer->SetCursorLock();

	PhysicComponent* playerPhysic = m_pPlayer->GetOwner()->GetComponent<PhysicComponent>();

	if (mState != GameState::PLAY)
	{
		playerPhysic->SetActive(false);
	}
	else
	{
		playerPhysic->SetActive(true);
	}
}

void ArenaShooterManager::ResetGame()
{
	m_pRoomManager->Reset();
	m_pPlayer->Reset();
	SetGameState(GameState::PLAY);
}

GameObject& ArenaShooterManager::CreateBullet()
{
	Geometry* bulletGeo = m_pRessourceManager->GetGeoWithName("Bullet");
	
	GameObject& bullet = GameObject::Create(*m_pScene);

	MeshRenderer* meshRenderer1 = bullet.AddComponent<MeshRenderer>();
	meshRenderer1->SetGeometry(bulletGeo);
	bullet.AddComponent<SphereCollider>();
	PhysicComponent* pc = bullet.AddComponent<PhysicComponent>();
	pc->SetGravityActive(false);
	return bullet;
}

Room* ArenaShooterManager::GetCurrentRoom()
{
	return m_pRoomManager->GetCurrentRoom();
}