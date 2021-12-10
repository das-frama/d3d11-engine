struct VS_INPUT
{
    float4 position: POSITION0;
    float2 texcoord: TEXCOORD0;
    float3 normal:   NORMAL0;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    return output;
}

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(1,1,1,1);
}