#pragma once

#include "Engine.h"
#include "Render.h"
#include "RessourceManager.h"

class SalleManager;
class Controller;
class Player;
class ArenaUI;
class SalleManager;
class Room;

using namespace gce;

enum class GameState
{
	MENU,
	PAUSE,
	PLAY,
	WIN,
	LOSE,
	LOAD
};

DECLARE_SCRIPT(ArenaShooterManager, ScriptFlag::Start | ScriptFlag::Update)

private:
	D12PipelineObject* m_pPso = nullptr;
	GameObject* m_pCameraOwner = nullptr;
	Controller* m_pController = nullptr;
	Player* m_pPlayer = nullptr;
	GameState mState = GameState::MENU;
	SalleManager* m_pRoomManager = nullptr;

	static ArenaShooterManager* instance;

	int mScore = 0;

public:
	static ArenaShooterManager* GetInstance();

	RessourceManager* m_pRessourceManager = nullptr;
	Scene* m_pScene = nullptr;
	Camera* m_pCamera = nullptr;
	ArenaUI* m_pUI = nullptr;

	void Start();
	void Update();

	void SetGameState(GameState newState);
	void SetScene(Scene* pScene) { m_pScene = pScene; };
	void SetCameraOwner(GameObject* pOwner);
	
	GameObject& CreateBullet();

	GameObject* GetCameraOwner() { return m_pCameraOwner; }
	Scene* GetScene() { return m_pScene; }
	Player* GetPlayer() { return m_pPlayer; }
	GameState GetState() { return mState; }
	Room* GetCurrentRoom();
	ArenaUI* GetIU() { return m_pUI; }

	void ResetGame();
	void AddScore(int score) { mScore += score; }

	END_SCRIPT