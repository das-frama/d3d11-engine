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

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
    float4 color;
    float4 color1;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    input.position.w = 1.0f;
    // world.
    output.position = mul(input.position, world);
    // view.
    output.position = mul(output.position, view);
    // screen space.
    output.position = mul(output.position, proj);

    output.texcoord = input.texcoord;

    return output;
}

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(lerp(color, color1, input.texcoord.y));
    // return float4(color1);
}