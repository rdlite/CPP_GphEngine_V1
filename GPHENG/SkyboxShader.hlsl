Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
    float3 normal: NORMAL0;
    float3 cameraDirection: TEXCOORD1;
};

float4 psmain(VS_OUTPUT input) : SV_TARGET
{
    return Texture.Sample(TextureSampler, (1.0 - input.texcoord));
}