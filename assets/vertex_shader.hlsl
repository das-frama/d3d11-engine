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
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    // output.position = input.position;
    // output.texcoord = float2(0, 0);
    // output.normal = float3(0, 0, 1);

    output.position = input.position;
    output.texcoord = input.texcoord;
    output.normal = input.normal;

    return output;
}