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

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.pos = input.pos;
    output.color = input.color;

    return output;
}

float4 psmain(VS_OUTPUT input) : SV_Target
{
    return float4(input.color, 1.0f);
}