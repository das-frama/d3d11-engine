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
    float3 normal:   NORMAL0;
    float3 world_pos: TEXCOORD1;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 proj;
    float4 light_direction;
    float4 cam_position;
    float4 light_position;
    float light_radius;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    // world.
    output.position = mul(input.position, world);
    output.world_pos = output.position.xyz;
    // view.
    output.position = mul(output.position, view);
    // screen space.
    output.position = mul(output.position, proj);

    // output.position = input.position;
    output.texcoord = input.texcoord;
    output.normal = normalize(mul(input.normal, world));
    // output.normal = input.normal;

    return output;
}