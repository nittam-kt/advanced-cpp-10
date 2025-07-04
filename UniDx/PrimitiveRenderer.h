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


} // namespace UniDx
