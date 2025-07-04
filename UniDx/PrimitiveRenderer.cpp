#include "PrimitiveRenderer.h"

#include "Texture.h"
#include "Camera.h"
#include "Debug.h"

// キューブの1面あたり4頂点、3面で12頂点、2セットで24頂点
namespace {

// 1面2三角形×6面＝12三角形＝36頂点（法線・UVも面ごとに分ける）
constexpr Vector3 positions[] = {
    // 前面
    {-0.5f, -0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, {0.5f,  0.5f, -0.5f},
    // 右面
    {0.5f, -0.5f, -0.5f}, {0.5f,  0.5f,  0.5f}, {0.5f, -0.5f,  0.5f},
    {0.5f, -0.5f, -0.5f}, {0.5f,  0.5f, -0.5f}, {0.5f,  0.5f,  0.5f},
    // 後面
    {0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
    {0.5f, -0.5f,  0.5f}, {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
    // 左面
    {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
    // 上面
    {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, {0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f, -0.5f}, {0.5f,  0.5f,  0.5f}, {0.5f,  0.5f, -0.5f},
    // 下面
    {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f,  0.5f},
};

constexpr Vector2 uvs[] = {
    // 各面同じ
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {0,0}, {1,0}, {0,1}, {1,0}, {1,1},
    {0,1}, {0,0}, {1,0}, {0,1}, {1,0}, {1,1},
};

constexpr Vector3 normals[] = {
    // 前面
    {0,0,-1}, {0,0,-1}, {0,0,-1}, {0,0,-1}, {0,0,-1}, {0,0,-1},
    // 右面
    {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0}, {1,0,0},
    // 後面
    {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1},
    // 左面
    {-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0}, {-1,0,0},
    // 上面
    {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0},
    // 下面
    {0,-1,0}, {0,-1,0}, {0,-1,0}, {0,-1,0}, {0,-1,0}, {0,-1,0},
};

}

namespace UniDx {

void CubeRenderer::OnEnable()
{
    MeshRenderer::OnEnable();

    // メッシュの初期化
    auto submesh = std::make_unique<SubMesh>();
    submesh->positions = std::span<const Vector3>(positions, std::size(positions));
    submesh->uv = std::span<const Vector2>(uvs, std::size(uvs));
    submesh->normals = std::span<const Vector3>(normals, std::size(normals));
    submesh->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    submesh->createBuffer<VertexPT>();

    mesh.submesh.push_back(std::move(submesh));
}


}