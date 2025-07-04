#pragma once

#include <memory>
#include <SimpleMath.h>
#include <DirectXTex.h>

#include "Component.h"
#include "Shader.h"


namespace UniDx {

class Camera;

// --------------------
// Textureクラス
// --------------------
class Texture : public Object
{
public:
    Texture() : Object([this]() {return fileName;})
    {
    }

    // 画像ファイルを読み込む
    bool load(const std::wstring& filePath);

    void setForRender() const;

protected:
    ComPtr<ID3D11Buffer> constantBuffer;
    std::wstring fileName;

    // シェーダーリソースビュー(画像データ読み取りハンドル)
    ComPtr<ID3D11ShaderResourceView> m_srv = nullptr;

    // 画像情報
    DirectX::TexMetadata m_info;
};


} // namespace UniDx
