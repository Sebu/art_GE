string tex0name = "bla.png";
texture tex1; //First texture
texture tex2;
float3 vec0;

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Norm  : COLOR0;
    float4 V     : COLOR1;
    float2 Tex   : TEXCOORD0;
};

VS_OUTPUT VS
	(
    	float3 Pos  : POSITION, 
	float2 Tex  : TEXCOORD0
	)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    Out.Tex  = Tex; 
    Out.V = -normalize(Pos).xyzz;
    Out.Pos  = Pos.xyzz;

    
    return Out;
}

sampler Sampler1 = sampler_state
{
    Texture   = (tex1);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler Sampler2 = sampler_state
{
    Texture   = (tex2);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PS(
    float4 Norm  : COLOR0,
    float4 V     : COLOR1,
    float2 Tex   : TEXCOORD0) : COLOR
{

    float4 N = tex2D(Sampler2,Tex);
    float4 L = float4(vec0,1);
    return tex2D(Sampler1,Tex) * dot(N,L);

}




technique t0
{
    pass p0
    {
	 fvf          = XYZRHW | Tex1;
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
 
}
