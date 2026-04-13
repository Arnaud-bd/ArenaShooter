#include "Life.h"

Life::Life()
{
}

void Life::SetLife(int maxHealth)
{
	mCurrentHealth = maxHealth;
	mMaxHealth = maxHealth;
}

int Life::GetHP()
{
	return mCurrentHealth;
}

bool Life::IsDead()
{
	if (mCurrentHealth <= 0)
		return true;

	return false;
}

void Life::TakeDamage(int dmg)//maybe defense later if bonus
{
	mCurrentHealth -= dmg;
	
	if (mCurrentHealth < 0)
		mCurrentHealth = 0;
}
