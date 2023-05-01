Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct VS_INPUT
{
    float4 position: POSITION0;
    float2 texcoord: TEXCOORD0;
    float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
    float3 normal: NORMAL0;
    float3 cameraDirection: NORMAL1;
};

cbuffer constant: register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_lightDirection;
    float4 m_cameraPosition;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    output.position = mul(input.position, m_world);
    output.cameraDirection = normalize(output.position.xyz - m_cameraPosition.xyz);
    output.position = mul(output.position, m_view);
    output.position = mul(output.position, m_proj);

    output.texcoord = input.texcoord;
    output.normal = input.normal;
    return output;
}

float4 psmain(VS_OUTPUT input) : SV_TARGET
{
    float3 noramizedNormal = normalize(input.normal);

    float ka = 0.1f;
    float3 ia = float3(1.0f, 1.0f, 1.0f);
    float3 ambient = ka * ia;

    float kd = 0.8f;
    float3 id = float3(1.0f, 1.0f, 1.0f);
    float3 diffuse = kd * max(0.0f, dot(m_lightDirection.xyz, noramizedNormal)) * id;

    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(m_lightDirection.xyz, noramizedNormal);
    float smoothness = 20.0f;
    float specular = ks * pow(max(0.0f, dot(reflectedLight, input.cameraDirection)), smoothness) * is;

    float3 finalLight = ambient + diffuse + specular;

	//return Texture.Sample(TextureSampler, input.texcoord * 1.0f) * float4(finalLight, 1.0f);
	//return Texture.Sample(TextureSampler, input.texcoord * 0.5f);
	return float4(finalLight, 1.0f);
}