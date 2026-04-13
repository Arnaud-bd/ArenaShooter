#include "RessourceManager.h"
#include "ArenaShooterManager.h"

#include "Engine.h"

gce::Texture* RessourceManager::LoadTexture(std::string path)
{
	gce::Texture* pTexture = new gce::Texture(path);
	s_pInstance->m_mTextureMap[path] = pTexture;

	return pTexture;
}

void RessourceManager::InitRM()
{
	s_pInstance = new RessourceManager();

	s_pInstance->m_pScene = ArenaShooterManager::GetInstance()->GetScene();

	gce::Geometry* objGeo = gce::GeometryFactory::CreateSphereGeo(0.05f, 10.f, 10.f);
	s_pInstance->m_mGeometryMap["Bullet"] = objGeo;
}

gce::Geometry* RessourceManager::GetGeoWithName(std::string name)
{
	return s_pInstance->m_mGeometryMap[name];
}

gce::Texture* RessourceManager::GetTextureWithName(std::string name)
{
	auto it = s_pInstance->m_mTextureMap.find(name);

	if (it == s_pInstance->m_mTextureMap.end())
		return LoadTexture(name);

	return it->second;
}

void RessourceManager::AddGeometry(gce::Geometry* newGeo, std::string name)
{
	s_pInstance->m_mGeometryMap[name] = newGeo;
}