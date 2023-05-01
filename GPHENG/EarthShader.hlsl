Texture2D EarthColor : register(t0);
sampler EarthColorSampler : register(s0);
Texture2D EarthSpec : register(t1);
sampler EarthSpecSampler : register(s1);
Texture2D EarthClouds : register(t2);
sampler EarthCloudsSampler : register(s2);
Texture2D EarthNight : register(t3);
sampler EarthNightSampler : register(s3);

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
    float m_time;
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

    float4 earthDayColor = EarthColor.Sample(EarthColorSampler, 1.0f - input.texcoord);
    float4 earthNightColor = EarthNight.Sample(EarthNightSampler, 1.0f - input.texcoord);
    float4 earthSpec = EarthSpec.Sample(EarthSpecSampler, 1.0f - input.texcoord).r;
    float4 earthClouds = EarthClouds.Sample(EarthCloudsSampler, 1.0f - input.texcoord + float2(m_time / 260.0f, 0.0f)).r;

    float dotValue = (dot(m_lightDirection.xyz, noramizedNormal) + 1.0f) / 2.0f;
    earthClouds = lerp(float4(0.0f, 0.0f, 0.0f, earthClouds.a), earthClouds, dotValue);

    float ka = 1.5f;
    float3 ia = float3(0.09f, 0.09f, 0.09f);
    ia *= (earthDayColor.rgb);
    float3 ambient = ka * ia;

    float kd = 1.0f;
    float3 idDay = float3(1.0f, 1.0f, 1.0f);
    idDay *= (earthDayColor.rgb + earthClouds);
    float3 idNight = float3(1.0f, 1.0f, 1.0f);
    idNight *= (earthNightColor.rgb + earthClouds);
    float amountDiffuse = dot(m_lightDirection.xyz, noramizedNormal);
    float3 id = lerp(idNight, idDay, (amountDiffuse + 1.0f) / 2.0f);
    float3 diffuse = kd * id;

    float ks = earthSpec.r;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(m_lightDirection.xyz, noramizedNormal);
    float smoothness = 20.0f;
    float specular = ks * pow(max(0.0f, dot(reflectedLight, input.cameraDirection)), smoothness) * is;

    float3 finalLight = ambient + diffuse + specular;

	return float4(finalLight, 1.0f);
}