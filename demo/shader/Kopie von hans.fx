string tex0name = "kacheln.bmp";
texture tex0; //First texture

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;


// light direction (view space)
float3 lightDir = {0.0, 0.0, 1.0};

// light intensity
float4 I_a = { 0.1f, 0.1f, 0.1f, 1.0f };    // ambient
float4 I_d = { 1.0f, 1.0f, 1.0f, 1.0f };    // diffuse
float4 I_s = { 1.0f, 1.0f, 1.0f, 1.0f };    // specular

// material reflectivity
float4 k_a : MATERIALAMBIENT = { 1.0f, 1.0f, 1.0f, 1.0f };    // ambient
float4 k_d : MATERIALDIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };    // diffuse
float4 k_s : MATERIALSPECULAR= { 1.0f, 1.0f, 1.0f, 1.0f };    // specular
int    n   : MATERIALPOWER = 32;                            // power



struct VS_OUTPUT
{
    float4 Pos  : POSITION;
    float4 Diff : COLOR0;
    float4 Tex  : COLOR1;
};



VS_OUTPUT VS
	(
    	float3 Pos  : POSITION, 
	float3 Norm : NORMAL, 
	float2 Tex  : TEXCOORD0
	)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float3 L = -lightDir;
	
    float4x4 WorldView = mul(World, View);

    float3 P = mul(float4(Pos, 1), (float4x3)WorldView);  // position (view space)
    float3 N = normalize(mul(Norm, (float3x3)WorldView)); // normal (view space)

    Out.Diff = I_a * k_a + I_d * k_d * max(0, dot(N, L)); // diffuse + ambient
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)
    Out.Tex  = P.z * 0.01;                                       
    
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
    float4 Diff : COLOR0,		
    float4 Tex  : COLOR1) : COLOR
{
    return  Tex + Diff;
}




technique t0
{
    pass p0
    {
	 fvf          = XYZ | Normal | Tex1;
        // shaders
        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_1_1 PS();
    }  
 
}
