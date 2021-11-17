cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
};

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float4 color: COLOR0;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return input.color;
}