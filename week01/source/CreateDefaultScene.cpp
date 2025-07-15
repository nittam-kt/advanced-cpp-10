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
#include <Rigidbody.h>
#include <Random.h>
#include <Collider.h>

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
    t->load(L"Resource/Albedo.png");
    material->addTexture(move(t));

    model->materials.push_back(move(material));
    auto modelptr = model.get();

    auto player = make_unique<GameObject>(L"テストキャラ",
        move(model));
    player->transform->localPosition = modelptr->position + Vector3(0,0,4);
    player->transform->localScale = modelptr->scale;
    player->transform->localRotation = modelptr->rotation * Quaternion::CreateFromYawPitchRoll(float(std::numbers::pi),0,0);

    // 複数のボールを作成
    array< unique_ptr<GameObject>, 10> balls;
    for (auto& b : balls)
    {
        b = make_unique<GameObject>(L"ボール",
            SphereRenderer::create<VertexPT>(L"Resource/Albedo.hlsl", L"Resource/gradation.png"),
            make_unique<Rigidbody>(),
            make_unique<SphereCollider>()
        );
        b->transform->position = Random::global().insideUnitSphere() * 2.5 + Vector3(0, 3, 2);
    }

    // キューブレンダラを作ってサイズを調整
    auto rb = make_unique<Rigidbody>();
    rb->gravityScale = 0;
    rb->mass = numeric_limits<float>::infinity();
    auto floor = make_unique<GameObject>(L"床",
        CubeRenderer::create<VertexPT>(L"Resource/Albedo.hlsl", L"Resource/floor.png"),
        move(rb),
        make_unique<AABBCollider>());
    floor->transform->localPosition = Vector3(0.0f, -0.5f, 0.0f);
    floor->transform->localScale = Vector3(10.0f, 0.5f, 10.0f);

    return make_unique<Scene>(

        make_unique<GameObject>(L"オブジェクトルート",
            move(player),
            move(floor),
            move(balls[0]),
            move(balls[1]),
            move(balls[2]),
            move(balls[3]),
            move(balls[4]),
            move(balls[5]),
            move(balls[6]),
            move(balls[7]),
            move(balls[8]),
            move(balls[9])
        ),

        make_unique<GameObject>(L"カメラルート",
            make_unique<Camera>(),
            make_unique<TestCameraBehaviour>()
        )
    );
}
