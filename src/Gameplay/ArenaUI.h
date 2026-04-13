#pragma once

#include "Engine.h"
#include "Render.h"

#include <map>
#include <vector>
#include "Components/UiImage.h"
#include "ArenaShooterManager.h"

class Player;

using namespace gce;


DECLARE_SCRIPT(ArenaUI, ScriptFlag::Start | ScriptFlag::Update)

private:

	std::map<GameState, std::vector<GameObject*>> m_mUIs;
	std::vector<GameObject*> m_vActiveUI;

	std::vector<GameObject*> m_vObject;
	std::vector<GameObject*> m_vLifeBar;
	std::vector<GameObject*> m_vUIWeapons;

	GameState mLastGameState;

	BitMapBrush* m_pButtonBrush = nullptr;
	BitMapBrush* m_pTriggerButtonBrush = nullptr;

	float mWinWidth;
	float mWinHeight;

	Vector3f32 mButtonSize = { 270.f, 100.f, 0.0f };
	
	BitMapBrush* m_ptextBrush = nullptr;
	
	//life
	int mLifeBarIndex = 0;
	int mNewLifeIndex = 5;

	Font* m_pFont = nullptr;
	BitMapBrush* m_pColorBrush = nullptr;

	//score
	TextRenderer* m_pScoreText = nullptr;
	WString* m_pScore = nullptr;

	//cooldown
	TextRenderer* m_pCooldownText = nullptr;
	WString* m_pTime = nullptr;

	//ammo
	TextRenderer* m_pAmmoText = nullptr;
	WString* m_pAmmos = nullptr;

	//lifeKit
	TextRenderer* m_pKitText = nullptr;
	WString* m_pKits = nullptr;

	//Player
	Player* m_pPlayer = nullptr;

	GameObject* m_pLoadingScreen = nullptr;
	GameObject* m_pEffectScreen = nullptr;
	float mEffectTime = 0.5f;
	float mEffectProgress = 0.f;
	//weaponUI
	int mWeaponIndex = -1;
	int mNewWeaponIndex = -1;

	static ArenaUI* instance;

	void OnStartMenuUI();
	void OnStartPauseUI();
	void OnStartPlayUI();
	void OnStartWinUI();
	void OnStartLoseUI();
	void HandlePlayUI();
	void UpdateGameState();
	void SwitchUI(GameState newState);
public:
	static ArenaUI* GetInstance();

	Scene* m_pScene = nullptr;
	Camera* m_pCamera = nullptr;

	void UIStart();
	
	void Start();
	void Update();
	void SetPlayer(Player* pPlay) { m_pPlayer = pPlay; }
	void SetWeapons(int index) { mNewWeaponIndex = index; }
	void SetLifeBar(int index) { mNewLifeIndex = index; }

	void SetLoadScreen(bool isActive);
	void SetEffectScreen(bool isActive);

	void SetWinSize(int width, int height) { mWinHeight = (float)height; mWinWidth = (float)width; }

	END_SCRIPT