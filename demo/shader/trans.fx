string tex0name = "stone.png";
texture tex1; //First texture
texture tex2; //First texture

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;
float4x4 ShadowWVP  : SHADOWWVP;
technique t0
{
    pass p0
    {   

//        WorldTransform[0]   = (World);
        ViewTransform       = (View);
	TextureTransform[0] = (ShadowWVP);
        ProjectionTransform = (Projection);

        fvf          = XYZRHW | Tex2;
        Texture[0] = <tex1>;
        Texture[1] = <tex2>;
	// Stage 1
	ColorOp[0] = SelectArg1;
        ColorArg1[0] = Texture;    

	ColorOp[1] = Modulate2X;
        ColorArg1[1] = Current;    
        ColorArg2[1] = Texture;    


	 
    }
}