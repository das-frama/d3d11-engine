cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
};

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD;
    float3 normal:   NORMAL;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(0.0f, 1.0f, 1.0f, 1.0f);   
}