#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include "Engine.h"
#include <fstream>
#include <wincodec.h>

#include "Core/nlohmann.hpp"
#include "Core/Maths/Quaternion.h"
#include "RessourceManager.h"

using json = nlohmann::json;
using namespace gce;

struct MapLoader
{
    static std::vector<GameObject*> LoadMap(String const& path, Scene* pScene)
    {
        std::vector<GameObject*> vAllObjects;

        if (pScene == nullptr) return vAllObjects;

        Map<GameObject*, String> mObjects;
        
        
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier JSON\n";
        }
        json j;
        try {
            file >> j;
        }
        catch (json::parse_error& e) {
            std::cerr << "Erreur de parsing: " << e.what() << "\n";
        }

        json jObjects = j["objects"];
        for (int i = 0; i < jObjects.size(); ++i)
        {
            json currObject = jObjects[i];
            GameObject& gameObject = GameObject::Create(*pScene);
            MeshRenderer& mesh = *gameObject.AddComponent<MeshRenderer>();
            mesh.SetGeometry(GeometryFactory::LoadJsonGeometry(currObject));
            mesh.pGeometry->name = currObject["name"].get<String>();
            gameObject.SetName(mesh.pGeometry->name.c_str());

            vAllObjects.push_back(&gameObject);
            
            if (currObject.contains("_texture") && currObject["_texture"].is_string())
            {
                std::string texturePath;
                texturePath.append("res/Textures/Rooms_Textures");
                texturePath.append(currObject["_texture"].get<std::string>());
                Texture* texture = RessourceManager::GetTextureWithName(texturePath);;
                mesh.SetAlbedoTexture(texture);
            }
            else if (currObject.contains("texture") && currObject["texture"].is_string())
            {
                std::string texturePath;
                texturePath.append("res/Textures/");
                texturePath.append(currObject["texture"].get<std::string>());
                Texture* texture = RessourceManager::GetTextureWithName(texturePath);
                mesh.SetAlbedoTexture(texture);
            }
            
            // Pos / Scale / Rot
            {
                Vector3f32 position;
                position.x = currObject["position"][0].get<float>();
                position.y = currObject["position"][2].get<float>();
                position.z = currObject["position"][1].get<float>();
                gameObject.transform.SetLocalPosition(position);

                Vector3f32 scale;
                scale.x = currObject["scale"][0].get<float>();
                scale.y = currObject["scale"][2].get<float>();
                scale.z = currObject["scale"][1].get<float>();
                gameObject.transform.SetLocalScale(scale);
            
                float32 rotX = currObject["rotation"][0].get<float>();
                float32 rotY = currObject["rotation"][1].get<float>();
                float32 rotZ = currObject["rotation"][2].get<float>();
                float32 rotW = currObject["rotation"][3].get<float>();
                Quaternion rotation(rotX, rotY, rotZ, -rotW);
                gameObject.transform.SetLocalRotation(rotation);
            }

            if (currObject.contains("parent") && currObject["parent"].is_string())
            {
                std::pair<GameObject*, String> el;
                el.first = &gameObject;
                el.second = currObject["parent"].get<std::string>();
                mObjects.insert(el);
            }

            if (currObject.contains("has_collider") && currObject["has_collider"].is_boolean() && currObject["has_collider"].get<bool>() == true)
            {
                if (currObject["has_collider"].get<bool>())
                    BoxCollider* b = gameObject.AddComponent<BoxCollider>();
            }
        }

        for (std::pair<GameObject*, String> el : mObjects)
        {
            for (GameObject* el2 :vAllObjects)
            {
                if (el.first == el2) continue;

                if (el.second == el2->GetName())
                    el.first->SetParent(*el2);
            }
        }


        return vAllObjects;
    }
};

#endif