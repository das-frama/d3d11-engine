Texture2D Tex: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD0;
    float3 normal:   NORMAL0;
    float3 world_pos: TEXCOORD1;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return Tex.Sample(TextureSampler, 1.0 - input.texcoord);
}