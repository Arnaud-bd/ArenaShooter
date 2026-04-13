#pragma once
#include <iostream>
#include "Engine/Engine.h"
#include "Render.h"

using namespace gce;

class RessourceManager
{
	std::map<std::string, gce::Geometry*> m_mGeometryMap;
	std::map<std::string, gce::Texture*> m_mTextureMap;

	static gce::Texture* LoadTexture(std::string path);

	Scene* m_pScene = nullptr;

	inline static RessourceManager* s_pInstance = nullptr;
public:
	void InitRM();

	static gce::Geometry* GetGeoWithName(std::string name);
	static gce::Texture* GetTextureWithName(std::string name);

	static void AddGeometry(gce::Geometry* newGeo, std::string name);
};

