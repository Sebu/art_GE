string tex0name = "kacheln.bmp";
texture tex0; //First texture

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Norm  : COLOR0;
};



VS_OUTPUT VS
	(
    	float3 Pos  : POSITION, 
	float3 Norm : NORMAL, 
	float2 Tex  : TEXCOORD0
	)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldView = mul(World, View);

    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);  // position (view space)
    float4 N = mul(float4(Pos, 1),World);
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Norm  = N.xyzz;
    
    return Out;
}

sampler Sampler = sampler_state
{
    Texture   = (tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

float4 PS(
    float4 Norm  : COLOR0) : COLOR
{
    return Norm;
}




technique t0
{
    pass p0
    {
	 fvf          = XYZ | Normal | Tex1;
        // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
 
}
