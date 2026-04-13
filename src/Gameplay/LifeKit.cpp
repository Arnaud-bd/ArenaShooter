#include "LifeKit.h"


void LifeKit::Start()
{
	m_pOwner->SetName("LifeKit");
}
void LifeKit::GetCollected()
{
	m_pOwner->Destroy();
}
