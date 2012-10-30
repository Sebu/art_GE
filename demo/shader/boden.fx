string tex0name = "stone.png";
texture tex0; //First texture
texture tex1; //First texture
float3 vec0;

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;

float4x4 Projection : PROJECTION;
float4x4 ShadowWVP: SHADOWWVP;
float4x4 LightWVP: LIGHTWVP;

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
    float4 Spec : COLOR1;
    float4 Tex  : TEXCOORD0;
    float2 Tex2  : TEXCOORD1;
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
    Out.Pos  = mul(float4(P, 1), Projection);             // position (projected)

    float4 Z = mul(float4(Pos, 1), (float4x4)ShadowWVP);  // position (view space)
    float4 L = mul(float4(Pos, 1), (float4x4)LightWVP);  // position (view space)

//    float3 N = normalize(mul(Norm, (float3x3)World)); // normal (world space)
//    Out.Diff = max(0, dot(N, vec0)); // diffuse + ambient

    Out.Diff = L.z*0.01;
    Out.Tex = Z.xyzw;
    Out.Tex2 = Tex;	                                      
    return Out;
}

sampler Sampler = sampler_state
{
    Texture   = (tex0);
};

sampler Sampler1 = sampler_state
{
    Texture   = (tex1);
};


float4 PS(
    float4 Diff : COLOR0,
    float4 Spec : COLOR1,
    float4 Tex  : TEXCOORD0,
    float2 Tex2  : TEXCOORD1
 		) : COLOR
{
    float4 X =Diff - tex2Dproj(Sampler1,Tex).r;
    return step(X,0) * tex2D(Sampler,Tex2);
}




technique t0
{
    pass p0
    {
	 fvf          = XYZ | Normal | Tex1;
        // shaders
	AddressU[1] = CLAMP;
 	AddressV[1] = CLAMP;

        VertexShader = compile vs_1_1 VS();
        PixelShader  = compile ps_2_0 PS();
    }  
 
}
