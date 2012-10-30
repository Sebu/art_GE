string tex0name = "light1.tga";
texture tex0; //First texture

// transformations
float4x4 World      : WORLD;
float4x4 View       : VIEW;
float4x4 Projection : PROJECTION;

technique t0
{
    pass p0
    {   

        WorldTransform[0]   = (World);
        ViewTransform       = (View);
        ProjectionTransform = (Projection);


        fvf          = XYZ | Normal | Tex1;


        Texture[0] = <tex0>;

	// Stage 1
	ColorOp[0] = SelectArg1;
        ColorArg1[0] = Texture;    

	//Stage 2
        ColorOp[1] = Addsigned;
        ColorArg1[1] = Current;    
        ColorArg2[1] = Texture;    
    }
}