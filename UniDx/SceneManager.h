#pragma once

#include <memory>

#include "Singleton.h"

namespace UniDx
{
class Scene;
}

std::unique_ptr<UniDx::Scene> CreateDefaultScene();


namespace UniDx
{

class Scene;

// シーンマネージャ
class SceneManager : public Singleton<SceneManager>
{
public:
    void createScene() { activeScene = std::move(CreateDefaultScene()); }

    Scene* GetActiveScene() { return activeScene.get(); }

protected:
    std::unique_ptr<Scene> activeScene;
};

}