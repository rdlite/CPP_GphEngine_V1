struct VS_INPUT
{
    float4 pos : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

cbuffer constant : register (b0)
{
    unsigned int time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.pos = lerp(input.pos, float4(1.0f + input.pos.x, 1.0f - input.pos.y, 1.0f - (sin(time / 1000.0f) + 1.0f) / 2.0f, 1.0f), (sin(time / 1000.0f) + 1.0f) / 2.0f);
    output.color = input.color;

    return output;
}

float4 psmain(VS_OUTPUT input) : SV_Target
{
    return float4(input.color, 1.0f);
}