#include "Player.h"
#include "ArenaShooterManager.h"
#include "ArenaUI.h"

#include "MapLoader.hpp"
#include "PM.h"
#include "RocketLauncher.h"
#include "PM.h"
#include "Gun.h"
#include "Dash.h"
#include "Room/Room.h"
#include "Audio.h"

#define MAX_LIFEKIT 5

void Player::Start()
{
	m_pOwner->SetType(EntityType::PLAYER);

	m_pCameraOwner = ArenaShooterManager::GetInstance()->GetCameraOwner();

	m_pOwner->transform.LocalTranslate({0.f, 12.f, 0.f});
	m_pOwner->SetName("Player");
	
	mCenter = Vector2i32(GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2);

	SetMousePosition(mCenter);
	LockMouseCursor();

	m_pWeaponSocket = &GameObject::Create(*m_pOwner->GetScene());
	m_pWeaponSocket->SetParent(*m_pCameraOwner);
	m_pWeaponSocket->transform.SetLocalPosition({0.5f, -0.1f , 0.25f });
	m_pWeaponSocket->transform.SetLocalRotation(m_pWeaponSocket->transform.GetLocalRotation());
	m_pWeaponSocket->SetName("WeaponSocket");

	m_pDash = new Dash();
	m_pDash->InitSkill(0.05f, 5.0f, 0, nullptr, this);

	StartVariables();
}

void Player::StartVariables()
{
	std::vector<GameObject*> go = MapLoader::LoadMap(RES_PATH"res/JSON/gun.json", m_pOwner->GetScene());
	m_pPrimaryWeapon = go[0]->AddScript<Gun>();
	go[0]->SetParent(*m_pWeaponSocket);
	go[0]->transform.SetLocalPosition({ -0.2f, 0.f, 0.2f });
	go[0]->transform.SetLocalRotation(m_pWeaponSocket->transform.GetLocalRotation());

	SetStatut(Statut::Normal);

	m_pCurrentWeapon = m_pPrimaryWeapon;

	SetLife(100);

	mMaxHealth = 100;
	mCurrentHealth = mMaxHealth;

	mMaxPA = 100;
	mPA = mMaxPA;

	mSpeed = 5.0f;
	mSpeedMultiplier = 1.0f;
	mSpeedDivider = 1.0f;

	mScore = 0;
	mLifeKitsNumber = 0;
}

void Player::Update()
{
	ArenaShooterManager* manager = ArenaShooterManager::GetInstance();
	if (IsDead())
	{
		manager->SetGameState(GameState::LOSE);
		return;
	}
	
	if (manager->GetState() != GameState::PLAY)
		return;

	HandlePA();

	m_pDash->UpdateSkill();

	int life = mCurrentHealth / 20;

	if (life != mCurrentLifeIndex)
	{
		ArenaUI* ui = ArenaShooterManager::GetInstance()->GetIU();
		mCurrentLifeIndex = life;

		ui->SetLifeBar(mCurrentLifeIndex);
	}

	UpdateView();
	StatutUpdate();
}

void Player::HandlePA()
{
	if (mIsRunning)
	{
		if (mPA == 0)
			SetRunningState(false);

		mPA_Progress += GameManager::DeltaTime();

		if (mPA_Progress >= mPA_Time)
		{
			mPA--;
			mPA_Progress = 0.0f;
		}
	}
	else
	{
		if (mPA == mMaxPA)
			return;

		mPA_Progress += GameManager::DeltaTime();

		if (mPA_Progress >= mPA_Time * 5.0f)
		{
			mPA++;
			mPA_Progress = 0.0f;
		}
	}
}

void Player::SetRunningState(bool state)
{
	mIsRunning = state;
	mPA_Progress = 0.0f;
}

void Player::StatutUpdate()
{
	switch (mStatut)
	{
	case Poisoned:
	{
		mPoisonedDamageProgress -= GameManager::DeltaTime();

		if (mPoisonedDamageProgress <= 0)
		{
			mPoisonedDamageProgress = mPoisonedDamageTime;
			TakeDamage(mPoisonDamage);
			mPoisonedDamageCount--;
		}

		if (mPoisonedDamageCount <= 0)
		{
			SetStatut(Normal);
		}
		break;
	}
	case Stun:
	{
		mStunProgress -= GameManager::DeltaTime();

		if (mStunProgress <= 0)
		{
			mCanMove = true;
			SetStatut(Normal);
		}
		break;
	}
	}
}

void Player::Move()
{
	if (mCanMove == false)
		return;

	Vector3f32 translation {};

	translation += m_pOwner->transform.GetLocalRight() * static_cast<float32>(GetKey(Keyboard::D) - GetKey(Keyboard::Q));
	translation += m_pOwner->transform.GetLocalForward() * static_cast<float32>(GetKey(Keyboard::Z) - GetKey(Keyboard::S));

	float32 dl = GameManager::FixedDeltaTime();

	if (dl >= 0.05f)
		dl = 0.01f;

	Vector3f32 worldPos = m_pOwner->transform.GetLocalPosition();

	Vector3f32 trans = {translation.x * dl * mSpeed * mSpeedMultiplier,
					0,
					translation.z * dl * mSpeed * mSpeedMultiplier };

	m_pOwner->transform.LocalTranslate(trans / mSpeedDivider);
}

void Player::UpdateView()
{
	Vector2i32 mousePos = GetMousePosition();

	mMouseDire.x = (mCenter.x - mousePos.x) * mSensitivity;
	mMouseDire.y = (mCenter.y - mousePos.y) * mSensitivity;

	if (mCursorLock)
	{
		SetMousePosition(mCenter);
	}

	float32 const deltaTime = GameManager::FixedDeltaTime();
	float32 constexpr speed = 50.f;

	Vector3f32 rotation;

	rotation.x = -mMouseDire.y;
	rotation.y = -mMouseDire.x;

	rotation *= 3.14f / 180.0f * deltaTime * speed;

	m_pOwner->transform.LocalRotate({ 0.f, rotation.y, 0.f });
	m_pCameraOwner->transform.LocalRotate({ rotation.x, 0.f, 0.f });
}

void Player::SwitchWeapon()
{
	if (m_pSecondaryWeapon == nullptr)
		return;

	if (m_pCurrentWeapon->HasSkillActive())
		m_pCurrentWeapon->EndSkill();

	m_pCurrentWeapon->GetOwner()->SetActive(false);

	//Switch
	m_pPrimaryWeapon = m_pSecondaryWeapon;
	m_pSecondaryWeapon = m_pCurrentWeapon;
	m_pCurrentWeapon = m_pPrimaryWeapon;

	m_pCurrentWeapon->GetOwner()->SetActive(true);

	ArenaUI* ui = ArenaShooterManager::GetInstance()->GetIU();
	int secondaryWeaponIndex = m_pSecondaryWeapon->GetIndex();

	ui->SetWeapons(secondaryWeaponIndex);
}

void Player::PlayerDash()
{
	AudioUse::Play("DashPlayer");

	if (mPA < 25)
		return;

	mPA -= 25;
	m_pDash->ActivateSkill();
}

void Player::UseWeaponSkill()
{
	m_pCurrentWeapon->UseSkill();
}

void Player::Shoot()
{
	AudioUse::Play("GunFire");

	m_pCurrentWeapon->Shoot();
}

void Player::CollectItem()
{
	if (m_pClosestObject == nullptr)
		return;

	cstr objectName = m_pClosestObject->GetName();

	Room* pCurrentRoom = ArenaShooterManager::GetInstance()->GetCurrentRoom();

	if (objectName == "Weapon")
	{

		if (m_pSecondaryWeapon == nullptr)
		{
			m_pSecondaryWeapon = m_pClosestObject->GetScript<Weapon>();
			m_pSecondaryWeapon->CollectWeapon(m_pWeaponSocket);
			m_pSecondaryWeapon->GetOwner()->SetActive(false);

			m_pClosestObject = nullptr;

			pCurrentRoom->RemoveGameObject(m_pSecondaryWeapon->GetOwner());

			SwitchWeapon();
		}
		else
		{
			GameObject* temp = m_pCurrentWeapon->GetOwner();

			m_pCurrentWeapon->DropWeapon();
			m_pCurrentWeapon = m_pClosestObject->GetScript<Weapon>();
			m_pCurrentWeapon->CollectWeapon(m_pWeaponSocket);

			m_pClosestObject = temp;
			m_pPrimaryWeapon = m_pCurrentWeapon;
			pCurrentRoom->RemoveGameObject(m_pPrimaryWeapon->GetOwner());
		}
	}
	else
	{
		m_pClosestObject->GetScript<LifeKit>()->GetCollected();
		mLifeKitsNumber++;

		if (mLifeKitsNumber > MAX_LIFEKIT)
			mLifeKitsNumber = MAX_LIFEKIT;

		m_pClosestObject = nullptr;
	}	
}

void Player::CollisionEnter(GameObject* other)
{

	if (other->GetName() == "Weapon" || other->GetName() == "LifeKit")
		m_pClosestObject = other;
}

void Player::CollisionExit(GameObject* other)
{
	if (other == m_pClosestObject)
		m_pClosestObject = nullptr;
}
void Player::SetCursorLock()
{
	mCursorLock = !mCursorLock;

	if (mCursorLock)
		HideMouseCursor();
	else
		ShowMouseCursor();
}

void Player::SetStatut(Statut newStatut)
{
	mStatut = newStatut;

	switch (mStatut)
	{
	case Poisoned:
	{
		mPoisonedDamageProgress = mPoisonedDamageTime;
		mPoisonedDamageCount = 3;
		break;
	}
	case Normal:
	{
		break;
	}
	case Stun:
	{
		mStunProgress = mStunTime;
		mCanMove = false;
		break;
	}
	}
}

void Player::Jump()
{
	Vector3f32 null = { 0.0f,0.0f,0.0f };
	
	if (m_pOwner->GetComponent<PhysicComponent>()->GetVelocity().y != 0.0f)
		return;

	float32 dt = GameManager::FixedDeltaTime();

	Vector3f32 trans = { 0.0f, mJumpSpeed * dt * mSpeed, 0.f};
	m_pOwner->GetComponent<PhysicComponent>()->SetVelocity(trans);
}

void Player::UseLifeKit()
{
	if (mLifeKitsNumber <= 0) return;

	AudioUse::Play("LifeKit");

	mCurrentHealth += 50;
	mLifeKitsNumber--;

	if (mCurrentHealth > mMaxHealth)
		mCurrentHealth = mMaxHealth;
}

void Player::Reset()
{
	m_pPrimaryWeapon->GetOwner()->Destroy();

	if(m_pSecondaryWeapon != nullptr)
		m_pSecondaryWeapon->GetOwner()->Destroy();

	m_pPrimaryWeapon = nullptr;
	m_pSecondaryWeapon = nullptr;

	m_pWeaponSocket;

	StartVariables();
}

void Player::TakeDamage(int dmg)
{
	Life::TakeDamage(dmg);
	ArenaShooterManager::GetInstance()->GetIU()->SetEffectScreen(true);
}