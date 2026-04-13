#include "ArenaUI.h"
#include <Font.h>
#include "Player.h"

#include <vector>

ArenaUI* ArenaUI::instance = nullptr;

ArenaUI* ArenaUI::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ArenaUI();
	}
	return instance;
}

void ArenaUI::OnStartMenuUI()
{
    std::vector<GameObject*> m_vUIObject;

    std::vector<const char*> vPath = {
    UI_PATH"Button_r.png",
    UI_PATH"Button_r.png",
    };

    std::vector<const char*> vAltPath = {
        UI_PATH"SelectedButton_r.png",
        UI_PATH"SelectedButton_r.png",
    };

    std::vector<void(*)()> vFonctions = {
        []() {ArenaShooterManager::GetInstance()->SetGameState(GameState::PLAY); },
        []() {exit(1); },

    };

    std::vector<std::wstring> vText = {
    L"  Start",
    L"   Quit",
    };

    std::vector<float> vScale = {
        1.f,
        1.f,
    };

    std::vector<Vector2f32> vPos = {
        { mWinWidth * 0.5f, mWinHeight * 0.4f },
        { mWinWidth * 0.5f, mWinHeight * 0.6f },
    };

    //BackGround
    float BGScale = 0.5f;

    GameObject& BGObj = GameObject::Create(*m_pScene);
    UiImage& BGImage = *BGObj.AddComponent<UiImage>();

    BGImage.btmBrush = new BitMapBrush(UI_PATH"menu.png");
    Vector2f32 bmp = BGImage.btmBrush->GetSize();
    Vector2f32 finalSize = { bmp.x * BGScale, bmp.y * BGScale };
    Vector2f32 pos = { mWinWidth * 0.5f - finalSize.x * 0.5f,
                        mWinHeight * 0.5f - finalSize.y * 0.5f };

    BGImage.InitializeImage(pos, finalSize, 1.f);
    BGImage.btmBrush->SetTransformMatrix({ pos.x, pos.y, 0 }, { BGScale, BGScale, 1.f }, 0);

    m_vUIObject.push_back(&BGObj);

    // ===============UI_Object=================
    for (int i = 0; i < vText.size(); i++)
    {
        GameObject& uiObj = GameObject::Create(*m_pScene);
        uiObj.transform.SetWorldPosition({ vPos[i].x, vPos[i].y, 0.f });
        uiObj.transform.SetWorldScale(mButtonSize);
        UiButton& button = *uiObj.AddComponent<UiButton>();
        button.AddListener(vFonctions[i]);

        button.pBitMapBrush = new BitMapBrush(vPath[i]);
        button.pHoverBitMapBrush = new BitMapBrush(vAltPath[i]);

        m_vUIObject.push_back(&uiObj);

        // ===============UI_Text=================
        GameObject& textObject = GameObject::Create(*m_pScene);
        TextRenderer* textRenderer = textObject.AddComponent<TextRenderer>();
        textRenderer->pFont = m_pFont;
        textRenderer->pBrush = m_ptextBrush;
        textRenderer->text = vText[i];

        textRenderer->rectPosF = new RectanglePosF(vPos[i].x - 95.f, vPos[i].y - 35.f,
            0.0f, 0.f);

        m_vUIObject.push_back(&textObject);
    }

    m_mUIs[GameState::MENU] = m_vUIObject;
}

void ArenaUI::OnStartPauseUI()
{
    std::vector<GameObject*> m_vUIObject;

    std::vector<const char*> vPath = {
        UI_PATH"Button_r.png",
        UI_PATH"Button_r.png",
        UI_PATH"Button_r.png",
    };

    std::vector<const char*> vAltPath = {
        UI_PATH"SelectedButton_r.png",
        UI_PATH"SelectedButton_r.png",
        UI_PATH"SelectedButton_r.png",
    };

    std::vector<void(*)()> vFonctions = {
        []() {ArenaShooterManager::GetInstance()->SetGameState(GameState::PLAY); },
        []() {ArenaShooterManager::GetInstance()->ResetGame(); },
        []() {exit(1); },
        
    };

    std::vector<std::wstring> vText = {
    L"Resume",
    L"Restart",
    L"   Quit",
    };

    std::vector<float> vScale = {
        1.f,
        1.f,
        1.f,
    };

    std::vector<Vector2f32> vPos = {
        { mWinWidth * 0.5f, mWinHeight * 0.3f },
        { mWinWidth * 0.5f, mWinHeight * 0.5f },
        { mWinWidth * 0.5f, mWinHeight * 0.7f },
    };

    //BackGround
    float BGScale = 0.5f;

    GameObject& BGObj = GameObject::Create(*m_pScene);
    UiImage& BGImage = *BGObj.AddComponent<UiImage>();

    BGImage.btmBrush = new BitMapBrush(UI_PATH"pause.png");
    Vector2f32 bmp = BGImage.btmBrush->GetSize();
    Vector2f32 finalSize = { bmp.x * BGScale, bmp.y * BGScale };
    Vector2f32 pos = { mWinWidth * 0.5f - finalSize.x * 0.5f,
                        mWinHeight * 0.5f - finalSize.y * 0.5f };

    BGImage.InitializeImage(pos, finalSize, 1.f);
    BGImage.btmBrush->SetTransformMatrix({ pos.x, pos.y, 0 }, { BGScale, BGScale, 1.f }, 0);

    m_vUIObject.push_back(&BGObj);

    // ===============UI_Object=================
    for (int i = 0; i < vText.size(); i++)
    {
        GameObject& uiObj = GameObject::Create(*m_pScene);
        uiObj.transform.SetWorldPosition({ vPos[i].x, vPos[i].y, 0.f});
        uiObj.transform.SetWorldScale(mButtonSize);
        UiButton& button = *uiObj.AddComponent<UiButton>();
        button.AddListener(vFonctions[i]);

        button.pBitMapBrush = new BitMapBrush(vPath[i]);
        button.pHoverBitMapBrush = new BitMapBrush(vAltPath[i]);

        m_vUIObject.push_back(&uiObj);

        // ===============UI_Text=================
        GameObject& textObject = GameObject::Create(*m_pScene);
        TextRenderer* textRenderer = textObject.AddComponent<TextRenderer>();
        textRenderer->pFont = m_pFont;
        textRenderer->pBrush = m_ptextBrush;
        textRenderer->text = vText[i];

        textRenderer->rectPosF = new RectanglePosF(vPos[i].x - 95.f, vPos[i].y - 35.f,
            0.0f, 0.f);

        m_vUIObject.push_back(&textObject);
    }

    for (int i = 0; i < m_vUIObject.size(); i++)
        m_vUIObject[i]->SetActive(false);

    m_mUIs[GameState::PAUSE] = m_vUIObject;
}

void ArenaUI::OnStartPlayUI()
{
    std::vector<GameObject*> m_vUIObject;

    // ===============UI_lifeBar================
    {
        std::vector<const char*> vPath = {
        LIFE_PATH"6.png",
        LIFE_PATH"5.png",
        LIFE_PATH"4.png",
        LIFE_PATH"3.png",
        LIFE_PATH"2.png",
        LIFE_PATH"1.png",
        };

        float vScale = 1.f;

        Vector2f32 vSize = { 300.f * vScale, 300.f * vScale };

        Vector2f32 vPos = { mWinWidth * 0.1f, mWinHeight * 0.15f };

        for (int i = 0; i < vPath.size(); i++)
        {
            GameObject& uiObj = GameObject::Create(*m_pScene);
            UiImage& uiImage = *uiObj.AddComponent<UiImage>();

            uiImage.btmBrush = new BitMapBrush(vPath[i]);
            Vector2f32 bmp = uiImage.btmBrush->GetSize();

            Vector2f32 finalSize = { bmp.x * vScale, bmp.y * vScale };

            Vector2f32 pos = { vPos.x - finalSize.x * 0.5f,
                               vPos.y - finalSize.y * 0.5f };

            uiImage.InitializeImage(pos, vSize, 1.f);

            float sx = vScale;
            float sy = vScale;

            uiImage.btmBrush->SetTransformMatrix(
                { pos.x, pos.y, 0 },
                { sx, sy, 1.f },
                0
            );

            m_vLifeBar.push_back(&uiObj);
            m_vUIObject.push_back(&uiObj);
        }

        for (int i = 0; i < m_vLifeBar.size(); i++)
        {
            if (i == m_vLifeBar.size())//full
                m_vLifeBar[i]->SetActive(true);
            else
                m_vLifeBar[i]->SetActive(false);
        }
    }

    // ===============UI_Object=================
    {
        std::vector<const char*> vPath = {
        UI_PATH"crosshair_r.png",
        UI_PATH"score_test_r.png",
        UI_PATH"weapon_r.png",
        };

        std::vector<float> vScale = {
            0.25f,
            0.9f,
            1.3f,
        };

        std::vector<Vector2f32> vSize = {
            {300.f * vScale[0], 300.f * vScale[0]},
            {300.f, 300.f},
            {300.f * vScale[2], 300.f * vScale[2]},
        };

        std::vector<Vector2f32> vPos = {
            { mWinWidth * 0.5f, mWinHeight * 0.5f },
            { mWinWidth * 0.9f, mWinHeight * 0.145f },
            { mWinWidth * 0.9f, mWinHeight * 0.85f },

        };

        for (int i = 0; i < vPath.size(); i++)
        {
            GameObject& uiObj = GameObject::Create(*m_pScene);
            UiImage& uiImage = *uiObj.AddComponent<UiImage>();

            uiImage.btmBrush = new BitMapBrush(vPath[i]);
            Vector2f32 bmp = uiImage.btmBrush->GetSize();

            Vector2f32 finalSize = { bmp.x * vScale[i], bmp.y * vScale[i] };

            Vector2f32 pos = { vPos[i].x - finalSize.x * 0.5f,
                               vPos[i].y - finalSize.y * 0.5f };

            uiImage.InitializeImage(pos, vSize[i], 1.f);

            float sx = vScale[i];
            float sy = vScale[i];

            uiImage.btmBrush->SetTransformMatrix(
                { pos.x, pos.y, 0 },
                { sx, sy, 1.f },
                0
            );

            m_vObject.push_back(&uiObj);
            m_vUIObject.push_back(&uiObj);
        }
    }

    // ===============UI_weapons================
    {
        std::vector<const char*> vPath = {
        WEAPONS_LIFE"RocketLauncher_UI_r.png",
        WEAPONS_LIFE"Rifle_UI_r.png",
        WEAPONS_LIFE"Gun_UI_r.png"
        };

        float vScale = 0.18f;

        Vector2f32 vSize = { 300.f, 300.f };

        Vector2f32 vPos = { mWinWidth * 0.947f, mWinHeight * 0.925f };

        for (int i = 0; i < vPath.size(); i++)
        {
            GameObject& uiObj = GameObject::Create(*m_pScene);
            UiImage& uiImage = *uiObj.AddComponent<UiImage>();

            uiImage.btmBrush = new BitMapBrush(vPath[i]);
            Vector2f32 bmp = uiImage.btmBrush->GetSize();

            Vector2f32 finalSize = { bmp.x * vScale, bmp.y * vScale };

            Vector2f32 pos = { vPos.x - finalSize.x * 0.5f,
                               vPos.y - finalSize.y * 0.5f };

            uiImage.InitializeImage(pos, vSize, 1.f);

            float sx = vScale;
            float sy = vScale;

            uiImage.btmBrush->SetTransformMatrix(
                { pos.x, pos.y, 0 },
                { sx, sy, 1.f },
                0
            );

            m_vUIWeapons.push_back(&uiObj);
        }

        for (int i = 0; i < m_vUIWeapons.size(); i++)
        {
            if (i == m_vUIWeapons.size())//Gun
                m_vUIWeapons[i]->SetActive(true);
            else
                m_vUIWeapons[i]->SetActive(false);
        }
    }

    // ===============UI_Text===================
    {
        m_pFont = new Font(L"Arial");
        m_pColorBrush = new BitMapBrush("res/Textures/Font_Texture/red_neon.png");
        m_pColorBrush->SetTransformMatrix({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);

        m_pScore = new WString();

        GameObject& textObject = GameObject::Create(*m_pScene);
        m_pScoreText = textObject.AddComponent<TextRenderer>();
        m_pScoreText->pFont = m_pFont;
        m_pScoreText->pBrush = m_pColorBrush;
        m_pScoreText->text = *m_pScore;
        m_pScoreText->rectPosF = new RectanglePosF(1520.f, 65.0f, 170.f, 170.0f);

        m_vObject.push_back(&textObject);
        m_vUIObject.push_back(&textObject);
    }

    // ===============UI_Cooldown===============
    {
        m_pTime = new WString();

        GameObject& textObject = GameObject::Create(*m_pScene);
        m_pCooldownText = textObject.AddComponent<TextRenderer>();
        m_pCooldownText->pFont = m_pFont;
        m_pCooldownText->pBrush = m_pColorBrush;
        m_pCooldownText->text = *m_pTime;
        m_pCooldownText->rectPosF = new RectanglePosF(1498.f, 703.0f, 170.f, 170.0f);

        m_vObject.push_back(&textObject);
        m_vUIObject.push_back(&textObject);
    }

    // ===============UI_Ammo===================
    {
        m_pAmmos = new WString();

        GameObject& textObject = GameObject::Create(*m_pScene);
        m_pAmmoText = textObject.AddComponent<TextRenderer>();
        m_pAmmoText->pFont = m_pFont;
        m_pAmmoText->pBrush = m_pColorBrush;
        m_pAmmoText->text = *m_pAmmos;
        m_pAmmoText->rectPosF = new RectanglePosF(1567.f, 623.0f, 170.f, 170.0f);

        m_vObject.push_back(&textObject);
        m_vUIObject.push_back(&textObject);
    }

    // ===============UI_LifeKit================
    {
        m_pKits = new WString();

        GameObject& textObject = GameObject::Create(*m_pScene);
        m_pKitText = textObject.AddComponent<TextRenderer>();
        m_pKitText->pFont = m_pFont;
        m_pKitText->pBrush = m_pColorBrush;
        m_pKitText->text = *m_pKits;
        m_pKitText->rectPosF = new RectanglePosF(125.f, 25.0f, 170.f, 170.0f);

        m_vObject.push_back(&textObject);
        m_vUIObject.push_back(&textObject);
    }

    for (int i = 0; i < m_vUIObject.size(); i++)
        m_vUIObject[i]->SetActive(false);

    m_mUIs[GameState::PLAY] = m_vUIObject;
}

void ArenaUI::OnStartWinUI()
{
    std::vector<GameObject*> m_vUIObject;

    std::vector<const char*> vPath = {
         UI_PATH"Button_r.png",
         UI_PATH"Button_r.png",
    };

    std::vector<const char*> vAltPath = {
        UI_PATH"SelectedButton_r.png",
        UI_PATH"SelectedButton_r.png",
    };

    std::vector<void(*)()> vFonctions = {
        []() {ArenaShooterManager::GetInstance()->ResetGame(); },
        []() {exit(1); },

    };

    std::vector<std::wstring> vText = {
    L"Restart",
    L"   Quit",
    };

    std::vector<float> vScale = {
        1.f,
        1.f,
    };

    std::vector<Vector2f32> vPos = {
        { mWinWidth * 0.5f, mWinHeight * 0.4f },
        { mWinWidth * 0.5f, mWinHeight * 0.6f },
    };

    //BackGround
    float BGScale = 0.5f;

    GameObject& BGObj = GameObject::Create(*m_pScene);
    UiImage& BGImage = *BGObj.AddComponent<UiImage>();

    BGImage.btmBrush = new BitMapBrush(UI_PATH"win.png");
    Vector2f32 bmp = BGImage.btmBrush->GetSize();
    Vector2f32 finalSize = { bmp.x * BGScale, bmp.y * BGScale };
    Vector2f32 pos = { mWinWidth * 0.5f - finalSize.x * 0.5f,
                        mWinHeight * 0.5f - finalSize.y * 0.5f };

    BGImage.InitializeImage(pos, finalSize, 1.f);
    BGImage.btmBrush->SetTransformMatrix({ pos.x, pos.y, 0 }, { BGScale, BGScale, 1.f }, 0);

    m_vUIObject.push_back(&BGObj);

    // ===============UI_Object=================
    for (int i = 0; i < vText.size(); i++)
    {
        GameObject& uiObj = GameObject::Create(*m_pScene);
        uiObj.transform.SetWorldPosition({ vPos[i].x, vPos[i].y, 0.f });
        uiObj.transform.SetWorldScale(mButtonSize);
        UiButton& button = *uiObj.AddComponent<UiButton>();
        button.AddListener(vFonctions[i]);

        button.pBitMapBrush = new BitMapBrush(vPath[i]);
        button.pHoverBitMapBrush = new BitMapBrush(vAltPath[i]);

        m_vUIObject.push_back(&uiObj);

        // ===============UI_Text=================
        GameObject& textObject = GameObject::Create(*m_pScene);
        TextRenderer* textRenderer = textObject.AddComponent<TextRenderer>();
        textRenderer->pFont = m_pFont;
        textRenderer->pBrush = m_ptextBrush;
        textRenderer->text = vText[i];

        textRenderer->rectPosF = new RectanglePosF(vPos[i].x - 95.f, vPos[i].y - 35.f,
            0.0f, 0.f);

        m_vUIObject.push_back(&textObject);
    }

    for (int i = 0; i < m_vUIObject.size(); i++)
        m_vUIObject[i]->SetActive(false);

    m_mUIs[GameState::WIN] = m_vUIObject;
}

void ArenaUI::OnStartLoseUI()
{
    std::vector<GameObject*> m_vUIObject;

    std::vector<const char*> vPath = {
         UI_PATH"Button_r.png",
         UI_PATH"Button_r.png",
    };

    std::vector<const char*> vAltPath = {
        UI_PATH"SelectedButton_r.png",
        UI_PATH"SelectedButton_r.png",
    };

    std::vector<void(*)()> vFonctions = {
        []() {ArenaShooterManager::GetInstance()->ResetGame(); },
        []() {exit(1); },

    };

    std::vector<std::wstring> vText = {
    L"Restart",
    L"   Quit",
    };

    std::vector<float> vScale = {
        1.f,
        1.f,
    };

    std::vector<Vector2f32> vPos = {
        { mWinWidth * 0.5f, mWinHeight * 0.4f },
        { mWinWidth * 0.5f, mWinHeight * 0.6f },
    };

    //BackGround
    float BGScale = 0.5f;

    GameObject& BGObj = GameObject::Create(*m_pScene);
    UiImage& BGImage = *BGObj.AddComponent<UiImage>();

    BGImage.btmBrush = new BitMapBrush(UI_PATH"loose.png");
    Vector2f32 bmp = BGImage.btmBrush->GetSize();
    Vector2f32 finalSize = { bmp.x * BGScale, bmp.y * BGScale };
    Vector2f32 pos = { mWinWidth * 0.5f - finalSize.x * 0.5f,
                        mWinHeight * 0.5f - finalSize.y * 0.5f };

    BGImage.InitializeImage(pos, finalSize, 1.f);
    BGImage.btmBrush->SetTransformMatrix({ pos.x, pos.y, 0 }, { BGScale, BGScale, 1.f }, 0);

    m_vUIObject.push_back(&BGObj);

    // ===============UI_Object=================
    for (int i = 0; i < vText.size(); i++)
    {
        GameObject& uiObj = GameObject::Create(*m_pScene);
        uiObj.transform.SetWorldPosition({ vPos[i].x, vPos[i].y, 0.f });
        uiObj.transform.SetWorldScale(mButtonSize);
        UiButton& button = *uiObj.AddComponent<UiButton>();
        button.AddListener(vFonctions[i]);

        button.pBitMapBrush = new BitMapBrush(vPath[i]);
        button.pHoverBitMapBrush = new BitMapBrush(vAltPath[i]);

        m_vUIObject.push_back(&uiObj);

        // ===============UI_Text=================
        GameObject& textObject = GameObject::Create(*m_pScene);
        TextRenderer* textRenderer = textObject.AddComponent<TextRenderer>();
        textRenderer->pFont = m_pFont;
        textRenderer->pBrush = m_ptextBrush;
        textRenderer->text = vText[i];

        textRenderer->rectPosF = new RectanglePosF(vPos[i].x - 95.f, vPos[i].y - 35.f,
            0.0f, 0.f);

        m_vUIObject.push_back(&textObject);
    }

    for (int i = 0; i < m_vUIObject.size(); i++)
        m_vUIObject[i]->SetActive(false);

    m_mUIs[GameState::LOSE] = m_vUIObject;
}

void ArenaUI::Start()
{
    m_pFont = new Font(L"Arial");
    m_ptextBrush = new BitMapBrush("res/Textures/Font_Texture/red_neon.png");
    m_ptextBrush->SetTransformMatrix({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 1.0f);

    OnStartPlayUI();
    OnStartMenuUI();
    OnStartPauseUI();
    OnStartWinUI();
    OnStartLoseUI();

    m_vActiveUI = m_mUIs[GameState::MENU];
    mLastGameState = GameState::MENU;

    std::vector<GameObject*> m_vUIObject;

    //BackGround
    float BGScale = 0.5f;

    m_pLoadingScreen = &GameObject::Create(*m_pScene);
    m_pLoadingScreen->SetActive(false);
    UiImage& BGImage = *m_pLoadingScreen->AddComponent<UiImage>();

    BGImage.btmBrush = new BitMapBrush(UI_PATH"background_loading.png");
    Vector2f32 bmp = BGImage.btmBrush->GetSize();
    Vector2f32 finalSize = { bmp.x * BGScale, bmp.y * BGScale };
    Vector2f32 pos = { mWinWidth * 0.5f - finalSize.x * 0.5f,
                        mWinHeight * 0.5f - finalSize.y * 0.5f };

    BGImage.InitializeImage(pos, finalSize, 1.f);
    BGImage.btmBrush->SetTransformMatrix({ pos.x, pos.y, 0 }, { BGScale, BGScale, 1.f }, 0);

    m_vUIObject.push_back(m_pLoadingScreen);
    m_mUIs[GameState::LOAD] = m_vUIObject;

    //Damage screen
    m_pEffectScreen = &GameObject::Create(*m_pScene);
    m_pEffectScreen->SetActive(false);
    UiImage& BGImage2 = *m_pEffectScreen->AddComponent<UiImage>();

    BGImage2.btmBrush = new BitMapBrush(UI_PATH"effect.png");
    Vector2f32 bmp2 = BGImage2.btmBrush->GetSize();
    Vector2f32 finalSize2 = { bmp2.x * BGScale, bmp2.y * BGScale };
    Vector2f32 pos2 = { mWinWidth * 0.5f - finalSize2.x * 0.5f,
                        mWinHeight * 0.5f - finalSize2.y * 0.5f };

    BGImage2.InitializeImage(pos2, finalSize2, 1.f);
    BGImage2.btmBrush->SetTransformMatrix({ pos2.x, pos2.y, 0 }, { BGScale, BGScale, 1.f }, 0);
}

void ArenaUI::SwitchUI(GameState newState)
{
    for (int i = 0; i < m_vActiveUI.size(); i++)
    {
        m_vActiveUI[i]->SetActive(false);
    }

    m_vActiveUI = m_mUIs[newState];

    if (newState == GameState::PLAY)
    {
        HandlePlayUI();
        return;
    }

    for (int i = 0; i < m_vActiveUI.size(); i++)
    {
        m_vActiveUI[i]->SetActive(true);
    }
}

void ArenaUI::UpdateGameState()
{
    GameState currentState = ArenaShooterManager::GetInstance()->GetState();

    if (mLastGameState != currentState)
    {
        SwitchUI(currentState);
        mLastGameState = currentState;
    }

}

void ArenaUI::Update()
{
    UpdateGameState();

    if (mLastGameState != GameState::PLAY)
        return;

    if (m_pPlayer == nullptr)
        return;
    
    //score
    *m_pScore = std::to_wstring(m_pPlayer->GetScore());
    m_pScoreText->text = *m_pScore;

    //couldown
    if (m_pPlayer->GetCurrentWeapon()->GetSkill() != nullptr && ArenaShooterManager::GetInstance()->GetState() != GameState::LOAD)
    {
        int timer = m_pPlayer->GetCurrentWeapon()->GetSkill()->GetSkillCouldown();

        *m_pTime = std::to_wstring(timer);
        m_pCooldownText->text = *m_pTime;
    }
    
    
    //ammos
    int ammos = m_pPlayer->GetCurrentWeapon()->GetAmmos();

    *m_pAmmos = std::to_wstring(ammos);
    m_pAmmoText->text = *m_pAmmos;

    //kits
    int kitz = m_pPlayer->GetKit();

    *m_pKits = std::to_wstring(kitz);
    m_pKitText->text = *m_pKits;
    
    
    if (mNewLifeIndex != mLifeBarIndex)
    {
        m_vLifeBar[mLifeBarIndex]->SetActive(false);

        mLifeBarIndex = mNewLifeIndex;

        m_vLifeBar[mLifeBarIndex]->SetActive(true);
    }

    if (mNewWeaponIndex != mWeaponIndex)
    {
        if (mWeaponIndex != -1)
            m_vUIWeapons[mWeaponIndex]->SetActive(false);

        mWeaponIndex = mNewWeaponIndex;

        m_vUIWeapons[mWeaponIndex]->SetActive(true);
    }
    
    mEffectProgress -= GameManager::DeltaTime();
    if (mEffectProgress <= 0)
        m_pEffectScreen->SetActive(false);
    else
        m_pEffectScreen->SetActive(true);
}

void ArenaUI::HandlePlayUI()
{
    for (int i = 0; i < m_vObject.size(); i++)
    {
        m_vObject[i]->SetActive(true);
    }

    m_vLifeBar[mLifeBarIndex]->SetActive(true);
}

void ArenaUI::SetLoadScreen(bool isActive)
{
    m_pLoadingScreen->SetActive(isActive);

    if (isActive)
    {
        ArenaShooterManager::GetInstance()->SetGameState(GameState::LOAD);
        UpdateGameState();
        GameManager::Draw();
    }
    else
    {
        ArenaShooterManager::GetInstance()->SetGameState(GameState::PLAY);
        UpdateGameState();
    }
}

void ArenaUI::SetEffectScreen(bool isActive)
{
    m_pEffectScreen->SetActive(isActive);

    if (isActive)
    {
        mEffectProgress = mEffectTime;
    }
}