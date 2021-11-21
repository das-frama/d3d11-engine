struct VS_INPUT
{
    float4 position: POSITION0;
    float2 texcoord: TEXCOORD0;
    float3 normal:   NORMAL0;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float4 color: COLOR0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // world.
    output.position = mul(input.position, world);
    // view.
    output.position = mul(output.position, view);
    // screen space.
    output.position = mul(output.position, proj);

    return output;
}

struct PS_INPUT
{
    float4 position: SV_POSITION;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(1,1,1,1);
}