#include "Controller.h"
#include "Player.h"
#include "ArenaShooterManager.h"

void Controller::Start()
{

}

void Controller::Update()
{
    if (GetKeyDown(Keyboard::ESCAPE))
    {
        ArenaShooterManager* manager = ArenaShooterManager::GetInstance();

        if (manager->GetState() == GameState::PAUSE)
            manager->SetGameState(GameState::PLAY);
        else if (manager->GetState() == GameState::PLAY)
            manager->SetGameState(GameState::PAUSE);
    }

    if (ArenaShooterManager::GetInstance()->GetState() != GameState::PLAY)
        return;

    if (m_pPlayer == nullptr)
        return;

    if (GetKey(Keyboard::Q) || GetKey(Keyboard::D) || GetKey(Keyboard::S) || GetKey(Keyboard::Z))
    {
        m_pPlayer->Move();
    }

    if (GetButton(Mouse::LEFT))
    {  
        m_pPlayer->Shoot();
    }

    if (GetKeyDown(Keyboard::A))
    {
        m_pPlayer->UseWeaponSkill();
    }

    if (GetKeyDown(Keyboard::TAB))
    {
        m_pPlayer->SwitchWeapon();
    }

    if(GetKeyDown(Keyboard::F))
    {
        m_pPlayer->CollectItem();
    }

    if (GetKeyDown(Keyboard::E))
    {
        m_pPlayer->PlayerDash();
    }

    if (GetKeyUp(Keyboard::LSHIFT))
    {
        m_pPlayer->AddSpeedMultiplier(-3);
        m_pPlayer->SetRunningState(false);
    }

    if (GetKeyDown(Keyboard::LSHIFT))
    {
        m_pPlayer->AddSpeedMultiplier(3);
        m_pPlayer->SetRunningState(true);
    }
    
    if (GetKeyDown(Keyboard::SPACEBAR))
    {
        m_pPlayer->Jump();
    }

    if (GetKeyDown(Keyboard::R))
    {
        m_pPlayer->UseLifeKit();
    }

}