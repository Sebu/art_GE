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
    float4 Pos2  : COLOR1;
    float2 Tex   : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 C0  : COLOR0;
    float4 C1  : COLOR1;
    float4 C2  : COLOR2;
    float4 C3  : COLOR3;
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
    float3 P2 = mul(float4(Pos, 1), (float4x3)World);  // position (view space)
    float4 N = mul(float4(Norm, 1),World);
    Out.Pos2 = P2.xyzz;
    Out.Norm = N;
    Out.Tex  = Tex; 
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)

    
    return Out;
}

sampler Sampler = sampler_state
{
    Texture   = (tex0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

PS_OUTPUT PS(
    float4 Norm  : COLOR0,
    float4 Pos2  : COLOR1,
    float2 Tex   : TEXCOORD0)
{
    PS_OUTPUT Out = (PS_OUTPUT)0;
    Out.C0=normalize(Norm);
    Out.C1=Pos2;
    Out.C2=tex2D(Sampler,Tex);
    return Out;
}




technique t0
{
    pass p0
    {
	 fvf          = XYZ | Normal | Tex1;
        // shaders
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
 
}
