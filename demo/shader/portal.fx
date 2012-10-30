string tex0name = "kacheln.bmp";
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

        fvf          = XYZ;

        Texture[0] = <tex0>;

	// Stage 1
	ColorOp[0] = SelectArg1;
        ColorArg1[0] = Texture;    

    }
}