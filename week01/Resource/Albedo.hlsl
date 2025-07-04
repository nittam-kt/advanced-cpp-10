// 0番のテクスチャスロットを使用する
Texture2D texture0 : register(t0);

// 0番のサンプラスロットを使用する
SamplerState sampler0 : register(s0);

cbuffer VSConstants : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
};

// 頂点シェーダーへ入力するデータ
struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXUV;
};


// 頂点シェーダーから出力するデータ
struct PSInput
{
    float4 pos : SV_Position; // 頂点の座標(射影座標系)
    float2 uv : TEXCOORD0; // UV座標
};
 
// ----------------------------------------------------------
// 頂点シェーダー
// ----------------------------------------------------------
PSInput VS(VSInput vin)
{
    PSInput Out;
    float4 p = mul(world, vin.pos);
    p = mul(view, p);
    p = mul(projection, p);
    Out.pos = p;
    Out.uv = vin.uv;
    return Out;
}
 
// ----------------------------------------------------------
// ピクセルシェーダー
// ----------------------------------------------------------
float4 PS(PSInput In) : SV_Target0
{
    // テクスチャから色を取得
    float4 texColor = texture0.Sample(sampler0, In.uv);
 
    // テクスチャの色を出力
    return texColor;
}
