#include "PrimitiveRenderer.h"

#include "Texture.h"
#include "Camera.h"
#include "Debug.h"

// キューブの1面あたり4頂点、3面で12頂点、2セットで24頂点
namespace {

// 1面2三角形×6面＝12三角形＝36頂点（法線・UVも面ごとに分ける）
constexpr Vector3 cube_positions[] = {
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

constexpr Vector2 cube_uvs[] = {
    // 各面同じ
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {1,0}, {1,1}, {0,1}, {0,0}, {1,0},
    {0,1}, {0,0}, {1,0}, {0,1}, {1,0}, {1,1},
    {0,1}, {0,0}, {1,0}, {0,1}, {1,0}, {1,1},
};

constexpr Vector3 cube_normals[] = {
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
    submesh->positions = std::span<const Vector3>(cube_positions, std::size(cube_positions));
    submesh->uv = std::span<const Vector2>(cube_uvs, std::size(cube_uvs));
    submesh->normals = std::span<const Vector3>(cube_normals, std::size(cube_normals));
    submesh->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    submesh->createBuffer<VertexPT>();

    mesh.submesh.push_back(std::move(submesh));
}


void SphereRenderer::createVertex()
{
    if (indices.size() > 0)
    {
        return;
    }

    constexpr int longitude = 16; // 経度分割数
    constexpr int latitude = 9;   // 緯度分割数

    // 頂点生成
    for (int lat = 0; lat < latitude; ++lat) {
        float v = static_cast<float>(lat) / (latitude - 1); // 0～1
        float phi = v * DirectX::XM_PI; // 0～π

        for (int lon = 0; lon < longitude; ++lon) {
            float u = static_cast<float>(lon) / (longitude - 1); // 0～1
            float theta = u * DirectX::XM_2PI; // 0～2π

            float x = std::sin(phi) * std::cos(theta) * 0.5f;
            float y = std::cos(phi) * 0.5f;
            float z = std::sin(phi) * std::sin(theta) * 0.5f;

            positions.emplace_back(x, y, z);
            normals.emplace_back(x * 2.0f, y * 2.0f, z * 2.0f); // 半径0.5なので法線は2倍
            uvs.emplace_back(u, v);
        }
    }

    // インデックス生成（三角形リスト）
    for (int lat = 0; lat < latitude - 1; ++lat) {
        for (int lon = 0; lon < longitude - 1; ++lon) {
            int current = lat * longitude + lon;
            int next = current + longitude;

            // 1つの四角形を2つの三角形に分割
            indices.push_back(current);
            indices.push_back(current + 1);
            indices.push_back(next);

            indices.push_back(current + 1);
            indices.push_back(next + 1);
            indices.push_back(next);
        }
    }
}


std::vector<Vector3> SphereRenderer::positions;
std::vector<Vector3> SphereRenderer::normals;
std::vector<Vector2> SphereRenderer::uvs;
std::vector<uint32_t> SphereRenderer::indices;

void SphereRenderer::OnEnable()
{
    MeshRenderer::OnEnable();

    createVertex();

    auto submesh = std::make_unique<SubMesh>();
    submesh->positions = std::span<const Vector3>(positions.data(), positions.size());
    submesh->normals = std::span<const Vector3>(normals.data(), normals.size());
    submesh->uv = std::span<const Vector2>(uvs.data(), uvs.size());
    submesh->indices = std::span<const uint32_t>(indices.data(), indices.size());
    submesh->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    submesh->createBuffer<VertexPT>();

    mesh.submesh.push_back(std::move(submesh));
}

}