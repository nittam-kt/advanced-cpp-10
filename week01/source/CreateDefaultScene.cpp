// CreateDefaultScene.cpp
// デフォルトのシーンを生成します

#include <numbers>

#include <UniDx.h>
#include <Scene.h>
#include <GameObject.h>
#include <Component.h>
#include <Transform.h>
#include <Camera.h>
#include <PrimitiveRenderer.h>
#include <GltfRenderer.h>

#include "TestRenderer.h"
#include "TestCameraBehaviour.h"

using namespace std;
using namespace UniDx;


unique_ptr<Scene> CreateDefaultScene()
{
    auto model = make_unique<GltfRenderer>();
    model->load<VertexPT>(L"Resource/ModularCharacter.glb");

    // マテリアル
    auto material = make_unique<Material>();

    // マテリアルをシェーダーを読み込んで初期化
    material->shader.compile<VertexPT>(L"Resource/Albedo.hlsl");

    // テクスチャを読み込んでマテリアルに追加
    auto t = make_unique<Texture>();
    t->load(L"Resource/ModularCharacterAlbedo.png");
    material->addTexture(move(t));

    model->materials.push_back(move(material));
    auto modelptr = model.get();

    auto player = make_unique<GameObject>(L"テストキャラ",
        move(model));
    player->transform->localPosition = modelptr->position;
    player->transform->localScale = modelptr->scale;
    player->transform->localRotation = modelptr->rotation * Quaternion::CreateFromYawPitchRoll(float(std::numbers::pi),0,0);

    // キューブレンダラを作ってサイズを調整
    auto floor = make_unique<GameObject>(L"床",
        CubeRenderer::create<VertexPT>(L"Resource/Albedo.hlsl", L"Resource/floor.png"));
    floor->transform->localPosition = Vector3(0.0f, -0.5f, 0.0f);
    floor->transform->localScale = Vector3(2.0f, 0.5f, 2.0f);

    return make_unique<Scene>(

        make_unique<GameObject>(L"オブジェクトルート",
            move(player),
            move(floor)
        ),

        make_unique<GameObject>(L"カメラルート",
            make_unique<Camera>(),
            make_unique<TestCameraBehaviour>()
        )
    );
}
