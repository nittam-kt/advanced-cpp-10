#pragma once

#include "Renderer.h"
#include "Texture.h"

namespace UniDx {

// --------------------
// CubeRendererクラス
// --------------------
class CubeRenderer : public MeshRenderer
{
public:
    template<typename TVertex>
    static std::unique_ptr<CubeRenderer> create(const std::wstring& shaderPath, const std::wstring& texturePath)
    {
        auto ptr = std::unique_ptr<CubeRenderer>(new CubeRenderer());
        ptr->addMaterial<TVertex>(shaderPath, texturePath);
        return ptr;
    }

protected:
    virtual void OnEnable() override;
};


// --------------------
// SphereRendererクラス
// --------------------
class SphereRenderer : public MeshRenderer
{
public:
    template<typename TVertex>
    static std::unique_ptr<SphereRenderer> create(const std::wstring& shaderPath, const std::wstring& texturePath)
    {
        auto ptr = std::unique_ptr<SphereRenderer>(new SphereRenderer());
        ptr->addMaterial<TVertex>(shaderPath, texturePath);
        return ptr;
    }

protected:
    static std::vector<Vector3> positions;
    static std::vector<Vector3> normals;
    static std::vector<Vector2> uvs;
    static std::vector<uint32_t> indices;
    static void createVertex();

    virtual void OnEnable() override;
};


} // namespace UniDx
