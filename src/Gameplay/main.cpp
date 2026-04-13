#include <windows.h>
#include <Engine.h>
#include <Shapes.h>

#include "ArenaShooterManager.h"
#include "ShowScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Spider2.h"


DECLARE_SCRIPT(ObjectRotatorScript, ScriptFlag::Update)

void Update() {
	m_pOwner->transform.LocalRotate({ 0.0f, 0.01f, 0.0f });
	m_pOwner->transform.LocalTranslate({ 0.0f, 0.01f, 0.0f });

}

END_SCRIPT

using namespace gce;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    Console::Init();
    GameManager::Create();

    Scene& scene = Scene::Create();

    WindowParam windowParam;
    windowParam.SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    windowParam.isFullScreen = FULLSCREEN;

    GameObject& cameraObject = GameObject::Create(scene);
    cameraObject.transform.LocalTranslate({ 0, 0, -3 });
    cameraObject.transform.LocalRotate({ 0, 0, 0 });

    Camera* pCamera = cameraObject.AddComponent<Camera>();
    pCamera->SetType(PERSPECTIVE);
    pCamera->SetMainCamera();
    pCamera->perspective.fov = XM_PIDIV4;
    pCamera->perspective.nearPlane = 0.001f;
    pCamera->perspective.farPlane = 500.0f;
    pCamera->perspective.up = { 0.0f, 0.0f, 0.0f };

    cameraObject.SetName("Camera");

    srand(time(NULL));

    /*----------------------------------------------------------------------------------*/

    GameObject& skyBox = GameObject::Create(scene);
    skyBox.AddComponent<SkyBoxComponent>()->Create("res/Textures/GrassCube.dds", { 40.0f,40.0f,40.0f });
    Light& light = skyBox.AddComponent<Light>()->DefaultDirectionLight();
    light.color = {0.969f,0.357f,1.f,1.f};
    light.strength = { 10.f, 10.f, 10.f };

    GameObject& manager = GameObject::Create(scene);
    ArenaShooterManager& script = *manager.AddScript<ArenaShooterManager>();
    script.SetCameraOwner(&cameraObject);
    script.m_pScene = &scene;
    script.m_pCamera = pCamera;

    /*----------------------------------------------------------------------------------*/

    GameManager::Run(windowParam);

    GameManager::Destroy();

    return 0;

}