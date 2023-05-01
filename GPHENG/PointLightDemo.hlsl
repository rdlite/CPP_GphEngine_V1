Texture2D TextureColor: register(t0);
sampler TextureColorSampler: register(s0);

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
    float3 normal: TEXCOORD1;
    float3 cameraDirection: TEXCOORD2;
    float3 worldPos: TEXCOORD3;
};

cbuffer constant: register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_lightDirection;
    float4 m_cameraPosition;
    float4 m_lightPosition;
    float m_lightRadius;
    float m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;

	output.position = mul(input.position, m_world);
    output.worldPos = output.position.xyz;
	output.position = mul(output.position, m_view);
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}

float4 psmain(VS_OUTPUT input) : SV_TARGET
{
    float3 noramizedNormal = normalize(input.normal);

    float3 albedoColor = TextureColor.Sample(TextureColorSampler, 1.0f - input.texcoord);

    float ka = 1.5f;
    float3 ia = float3(0.09f, 0.09f, 0.09f);
    float3 ambient = ka * (ia * albedoColor);

    float kd = 0.7f;

    float3 lightDir = normalize(m_lightPosition.xyz - input.worldPos.xyz);
    float distanceLightObject = length(m_lightPosition.xyz - input.worldPos.xyz);
    float fadeArea = max(0.0f, distanceLightObject - m_lightRadius);

    float constant_func = 1.0f;
    float linear_func = 0.1f;
    float quadratic_func = 1.0f;

    float attenuation = constant_func + linear_func * fadeArea + quadratic_func * fadeArea * fadeArea;

    float3 id = albedoColor;
    float amountDiffuse = max(0.0f, dot(lightDir.xyz, noramizedNormal));
    float3 diffuse = (kd * id * amountDiffuse) / attenuation;

    float ks = 1.0f;
    float3 directionToCamera = normalize(input.worldPos.xyz - m_cameraPosition.xyz);
    float3 is = albedoColor;
    float3 reflectedLight = reflect(lightDir, noramizedNormal);
    float smoothness = 20.0f;
    float specular = (ks * pow(max(0.0f, dot(reflectedLight, directionToCamera)), smoothness) * is) / attenuation;

    float3 finalLight = ambient + diffuse + specular;

    //return float4(finalLight, 1.0f);
    return float4(finalLight, 1.0f);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
}