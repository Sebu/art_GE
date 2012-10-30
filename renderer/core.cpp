/******************************************************************************************

    >>artGE RenderCore<<
	>>Main Renderer

    Project: artGE

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 02.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 02.11.2002
*/

#include "global.h"

#define SMT_FOMAT_X 800
#define SMT_FOMAT_Y 600
 
void CartGE::CameraSet(D3DXMATRIX *matView)
{
this->matView=*matView;
};


int CartGE::LoadFX(char *pFilename) {
   
   int   count, i,i2;
   CFX newFX;
   FILE		*pDatei;
   TCHAR	cTexturName[128],
			cFXName[128];
   artGELog.Write("Effect %s loading ...\n", pFilename);
   for (i=0; i < vFXManager.size(); i++) {
      if (lstrcmpi(&vFXManager[i].cName[0], pFilename) == 0) {
         artGELog.Write("Effect %s still in MEM\n", pFilename);
         return i;
         } 
      }

   lstrcpyn(&newFX.cName[0], pFilename, 128);

   newFX.iFrames=1;
   Frame=0;

   newFX.aTextur = new LPDIRECT3DTEXTURE9[1];

   HRESULT hr;
   hr = D3DXCreateEffectFromFile(pDevice, pFilename, NULL, NULL, 0, NULL, &newFX.pEffect, NULL );

    if (FAILED(hr)) {
		   artGELog.Write("Error: FX %s loading\n", pFilename);
	  } else {
		   artGELog.Write("...FX: %s loaded\n", pFilename);
	
	  }
   D3DXHANDLE hTechnique;
   newFX.pEffect->FindNextValidTechnique(NULL, &hTechnique);
   newFX.pEffect->SetTechnique(hTechnique);

   LPSTR szString;
   newFX.pEffect->GetValue("tex0name", (void*)&szString, sizeof(LPCSTR));
   TextureLoad(szString, &newFX.aTextur[0]);
      	
   newFX.pEffect->SetTexture("tex0", newFX.aTextur[0]);
   		newFX.pEffect->SetMatrix("World", &matWorld);
		newFX.pEffect->SetMatrix("View", &matView);
		newFX.pEffect->SetMatrix("Projection", &matProjection);
			

   
   vFXManager.push_back(newFX);
   artGELog.Write("Effect %s loaded\n", pFilename);

   return i;
} 


void SetFrustumMatrix( D3DMATRIX& mat, 
                                     FLOAT fLeft, FLOAT fRight, FLOAT fTop,
                                     FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane )
{
    float Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = ( 2.0f*fNearPlane )/( fRight - fLeft );
    mat._22 = ( 2.0f*fNearPlane )/( fTop - fBottom );
    mat._31 = ( fRight + fLeft )/ (fRight - fLeft );
    mat._32 = ( fTop + fBottom )/ (fTop - fBottom );
    mat._33 = Q;
    mat._34 = 1.0f;
    mat._43 = -Q*fNearPlane;
}


HRESULT SetAdjustedProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane,
                                     FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
                                     FLOAT fVPHeight )
{
    if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;

    float h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );

    float fTop = fNearPlane/h;
    float fBottom = -fTop;
    float fRight = fTop * fAspect;
    float fLeft = -fRight;

    float fXWSize = fRight - fLeft;
    float fYWSize = fTop - fBottom;

    float fDx = -( fPixDx*fXWSize/fVPWidth );
    float fDy = -( fPixDy*fYWSize/fVPHeight );
    
    SetFrustumMatrix( mat, fLeft+fDx, fRight+fDx, fTop+fDy, fBottom+fDy, fNearPlane,
        fFarPlane );

    return S_OK;
}


CartGE::~CartGE(void)
{
	if (pDirect3D)
	pDirect3D->Release();
	pDirect3D=NULL;

	if (pDevice)
		pDevice->Release();
	pDevice=NULL;

	artGELog.Write("DEVICE released");
};

int CartGE::FrameRate(void)
{
	static int zeit = 0; 
	static int bild = 0; 
	static int count = 0; 
	static int zeitalt = 0; 
	zeit = timeGetTime() ; 
	count++; 
	if(zeit-zeitalt >= 1000) 
	{ 
	bild = count; 
	zeitalt = zeit; 
	count = 0; 
	} 
	return bild;
};

void CartGE::Print(int x, int y, const char *fmt, ...)	
{
	char		text[1024];									
	va_list		ap;											// Zeiger auf Argumentliste

	if (fmt == NULL)										// wenn kein text 
		return;												// abbrechen
	va_start(ap, fmt);										// sucht nach variablen
	    vsprintf(text, fmt, ap);							// und ersetzt sie
	va_end(ap);												// text
	
      g_font->DrawText(x, //X position in screen coordinates
                       y, //Y position in screen coordinates
                      0xFFBBBBBB, //Colour
                     text);  //String to be rendered

};

void CartGE::TextureLoad(char *p_Filename, LPDIRECT3DTEXTURE9 *p_Textur)
{
   HRESULT hr;
       hr = D3DXCreateTextureFromFile(pDevice, p_Filename,
								  							p_Textur);
	   if (FAILED(hr)) {
		   artGELog.Write("Error: Textur %s loading\n", p_Filename);
	  } else {
		   artGELog.Write("...Textur: %s loaded\n", p_Filename);
	
	  }

	  
};


bool CartGE::LoadMesh(char *p_Filename,CMesh *mesh)
{
	FILE		*pDatei;
    float x, y, z, nx, ny, nz, tu, tv;
	WORD p1, p2, p3;
	int count, i;
	char chShader[50];
	
	pDatei = fopen(p_Filename, "r");
    
	lstrcpyn(&mesh->cName[0], p_Filename, 128);
	
	

	fscanf(pDatei, "%i\n",&count); 
	
	mesh->iNumVertices = count;
	mesh->p_Vertices = new CVertex[count];
	
	for (i=0;i<mesh->iNumVertices;i++)
	{
   		fscanf(pDatei, "%f %f %f %f %f %f %f %f\n",&x,&y,&z, &tu, &tv ,&nx, &ny, &nz); 
		mesh->p_Vertices[i].vPos.x=x;
		mesh->p_Vertices[i].vPos.y=y;
		mesh->p_Vertices[i].vPos.z=z;
		mesh->p_Vertices[i].vNormal.x=nx;
		mesh->p_Vertices[i].vNormal.y=ny;
		mesh->p_Vertices[i].vNormal.z=nz;

		mesh->p_Vertices[i].fU=tu;
		mesh->p_Vertices[i].fV=tv;
		
	}

	fscanf(pDatei, "%i\n",&count); 
	mesh->iTriangles = count;
	fscanf(pDatei, "%i\n",&count); 
	mesh->iNumIndices = count;
	mesh->p_Indices = new WORD[count];

	for (i=0;i<mesh->iNumIndices;i++)
	{
   		fscanf(pDatei, "%d",&p1); 
		mesh->p_Indices[i]=p1;

	}
	
	fscanf(pDatei, "%s\n", &chShader);
	mesh->iCgFXIndex = this->LoadFX(chShader);
	
	fscanf(pDatei, "%i\n",&count); 
	mesh->iDrawMode=count;
	
	artGELog.Write("Effect %s assinged to Surface %s",vFXManager[mesh->iCgFXIndex].cName,mesh->cName);
	fclose(pDatei);
	

	return true;

};



void CartGE::LoadMap(char *p_Filename,CMap *map)
{
	FILE		*pDatei;
    int			count,i, i2, i3;
    float		x, y, z, w;
	char		aName[128];
	D3DXMATRIX matSave, matTrans, matRot;

	pDatei = fopen(p_Filename, "r");
    
	lstrcpyn(&map->cName[0], p_Filename, 128);

	fscanf(pDatei, "%i\n",&count); 
	
	map->iSectors = count;
	map->p_Sectors = new CSector[count];	
	

	for (i=0;i<map->iSectors;i++)
	{
		fscanf(pDatei, "%i\n",&count); 
		map->p_Sectors[i].iEntities=count;
		map->p_Sectors[i].p_Entities = new CEntity[count];
		fscanf(pDatei, "%i\n",&count); 
		map->p_Sectors[i].iAlphaEntities=count;
		map->p_Sectors[i].p_AlphaEntities = new CEntity[count];
		fscanf(pDatei, "%i\n",&count); 
		map->p_Sectors[i].iPortals=count;
		map->p_Sectors[i].p_Portals = new CPortal[count];
		
		for (i2=0;i2<map->p_Sectors[i].iEntities;i2++)
		{
			fscanf(pDatei, "%s\n",&aName); 
			LoadMesh(aName,&map->p_Sectors[i].p_Entities[i2].Mesh);
			fscanf(pDatei, "%f %f %f %f\n",&x,&y,&z, &w); 
			D3DXMatrixIdentity(&matSave);
			D3DXMatrixTranslation(&matTrans,x,y,z);
			D3DXMatrixRotationY(&matRot,D3DXToRadian(w));
			D3DXMatrixMultiply(&matSave,&matSave,&matRot);
			D3DXMatrixMultiply(&matSave,&matSave,&matTrans);
			map->p_Sectors[i].p_Entities[i2].matWorld=matSave;
		}

		for (i2=0;i2<map->p_Sectors[i].iAlphaEntities;i2++)
		{
			fscanf(pDatei, "%s\n",&aName); 
			LoadMesh(aName,&map->p_Sectors[i].p_AlphaEntities[i2].Mesh);
			fscanf(pDatei, "%f %f %f %f\n",&x,&y,&z, &w); 
			D3DXMatrixIdentity(&matSave);
			D3DXMatrixTranslation(&matTrans,x,y,z);
			D3DXMatrixRotationY(&matRot,D3DXToRadian(w));
			D3DXMatrixMultiply(&matSave,&matSave,&matRot);
			D3DXMatrixMultiply(&matSave,&matSave,&matTrans);
			map->p_Sectors[i].p_AlphaEntities[i2].matWorld=matSave;
		}


		for (i2=0;i2<map->p_Sectors[i].iPortals;i2++)
		{
			for (i3=0;i3<5;i3++)
			{
				fscanf(pDatei, "%f %f %f\n",&x,&y,&z); 
				map->p_Sectors[i].p_Portals[i2].p_Points[i3].x=x;
				map->p_Sectors[i].p_Portals[i2].p_Points[i3].y=y;
				map->p_Sectors[i].p_Portals[i2].p_Points[i3].z=z;
			}
			fscanf(pDatei, "%i\n",&count);
			map->p_Sectors[i].p_Portals[i2].iCgFXIndex = this->LoadFX("shader/portal.fx");
			map->p_Sectors[i].p_Portals[i2].iToSector=count;
		}
	}
	fclose(pDatei);
};

void CartGE::RenderShadowedMap(CMap *map)
{
	Camera test;
	static float u = 0.005f;
	static float m=0.6f;
	//if (m>=2.2f) u=-u;
	//if (m<0.6f) u=-u;
    //m=m+u;
	test.Create(0.0f, 0.0f, 0.0f,"HANS");
	test.Rotate(0.015f,0.0f,0.0f);			
	test.Translate(0.0f,-2.0f,-20.0f);
	test.Update();


	D3DVIEWPORT9 oldViewport;
	D3DXMATRIX oldMat, matShadow;


	lightDir=test.vDir;

	oldMat=matView;
	matView=test.matView;
    matViewS=test.matView;
	// Save old render taget
	pDevice->GetRenderTarget(0, &pOldBackBuffer );
	pDevice->GetDepthStencilSurface( &pOldZBuffer );
	// Save old viewport
	pDevice->GetViewport( &oldViewport );

	// Set new render target
	pDevice->SetRenderTarget(0, aShadowMapSurf);
	pDevice->SetDepthStencilSurface(aShadowMapZ);

	// Setup shadow map viewport
	D3DVIEWPORT9 shadowViewport;
	shadowViewport.X = 0;
	shadowViewport.Y = 0;
	shadowViewport.Width  = SMT_FOMAT_X;
	shadowViewport.Height = SMT_FOMAT_Y;
	shadowViewport.MinZ = 0.0f;
	shadowViewport.MaxZ = 1.0f;
	pDevice->SetViewport( &shadowViewport );

	// Clear viewport
    pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0L );
	
	shadowViewport.X = 1;
	shadowViewport.Y = 1;
	shadowViewport.Width  = SMT_FOMAT_X - 2;
	shadowViewport.Height = SMT_FOMAT_Y - 2;
	shadowViewport.MinZ = 0.0f;
	shadowViewport.MaxZ = 1.0f;
   	pDevice->SetViewport( &shadowViewport );

	RenderBla(map);
	
	matView=oldMat;

	pDevice->SetRenderTarget(0, pOldBackBuffer);
	pDevice->SetDepthStencilSurface( pOldZBuffer );
	pOldBackBuffer->Release();
	pOldZBuffer->Release();

	// Restore old viewport
	pDevice->SetViewport( &oldViewport );
	
};

void CartGE::RenderMap(CMap *map)
{
	
	/*fFrame=fFrame+(0.02f*Timer.fTimeElapsed);
	Frame=fFrame;
	if (Frame>=11)
	fFrame=0.0f;
	Frame=fFrame;*/
	//pDevice->GetRenderTarget(0, &pOldBackBuffer );
	//pDevice->SetRenderTarget(0, aRTSurf0);
	//pDevice->SetRenderTarget(1, aRTSurf1);
	//pDevice->SetRenderTarget(2, aRTSurf2);
	//pDevice->SetRenderTarget(3, aRTSurf3);
    pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );
	int i,i2;
	for (i=0;i<map->iSectors;i++)
	{
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		for (i2=0;i2<map->p_Sectors[i].iEntities;i2++)
		{
			
			matWorld=map->p_Sectors[i].p_Entities[i2].matWorld;
			RenderMesh(&map->p_Sectors[i].p_Entities[i2].Mesh);

		}
	//	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	    pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		for (i2=0;i2<map->p_Sectors[i].iPortals;i2++)
		{
			
			RenderPortal(&map->p_Sectors[i].p_Portals[i2]);
		}

		for (i2=0;i2<map->p_Sectors[i].iAlphaEntities;i2++)
		{
			
			matWorld=map->p_Sectors[i].p_AlphaEntities[i2].matWorld;
			pDevice->SetTexture(0,NULL);
			RenderMesh(&map->p_Sectors[i].p_AlphaEntities[i2].Mesh);
		}

	}

//	pDevice->SetRenderTarget(0, pOldBackBuffer);
//	pDevice->SetRenderTarget(1, NULL);
//	pDevice->SetRenderTarget(2, NULL);
//	pDevice->SetRenderTarget(3, NULL);
   //RenderQuad();
};

void CartGE::RenderBla(CMap *map)
{
	
	int i,i2;
	for (i=0;i<map->iSectors;i++)
	{
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		for (i2=0;i2<map->p_Sectors[i].iEntities;i2++)
		{
			
			matWorld=map->p_Sectors[i].p_Entities[i2].matWorld;
			RenderShadowMesh(&map->p_Sectors[i].p_Entities[i2].Mesh);

		}
		
		for (i2=0;i2<map->p_Sectors[i].iAlphaEntities;i2++)
		{
			
			matWorld=map->p_Sectors[i].p_AlphaEntities[i2].matWorld;
			pDevice->SetTexture(0,NULL);
			RenderShadowMesh(&map->p_Sectors[i].p_AlphaEntities[i2].Mesh);
		}

	}
};

void CartGE::RenderPortal(CPortal *portal)
{
	
	UINT uPasses;


			

	vFXManager[portal->iCgFXIndex].pEffect->Begin(&uPasses, 0 );		
		
	for(UINT uPass = 0; uPass < uPasses; uPass++)
    {
        
		vFXManager[portal->iCgFXIndex].pEffect->Pass(uPass);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,
									2,
									portal->p_Points,
									sizeof(D3DXVECTOR3));

    }
 
    vFXManager[portal->iCgFXIndex].pEffect->End();
};

// GameDev.net sein Dank =)
void CartGE::TakeScreenShot(char* file_name, int screenx, int screeny)
{
   IDirect3DSurface9* frontbuf; //this is our pointer to the memory location containing our copy of the
                                //front buffer

   //now we create the image that our screen shot will be copied into
   //NOTE: Surface format of the front buffer is D3DFMT_A8R8G8B8 when it is returned
   pDevice->CreateOffscreenPlainSurface(screenx, screeny, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH , &frontbuf, NULL);

   //now we copy the front buffer into our surface
   HRESULT hr = pDevice->GetFrontBufferData(NULL, frontbuf);

   //error checking
   if(hr != D3D_OK)
   {
      //do error handling etc...
      frontbuf->Release(); //release the surface so there is no memory leak
	  artGELog.Write("Screenshot failed");
      return;
   }

   //now write our screen shot to a bitmap file
   //the last 2 params are NULL because we want the entire front buffer and no palette
   D3DXSaveSurfaceToFile(file_name, D3DXIFF_BMP, frontbuf, NULL, NULL);
   artGELog.Write("Screenshot taken");
   //release the surface so there is no memory leak
   frontbuf->Release();
};



void CartGE::RenderShadowMesh(CMesh *mesh)
{
	

	vFXManager[0/*mesh->iCgFXIndex*/].pEffect->SetMatrix("World", &matWorld);
	
	vFXManager[0/*mesh->iCgFXIndex*/].pEffect->SetMatrix("View", &matView);
		
	vFXManager[0].pEffect->SetMatrix("Projection", &matProjectionS);
	UINT uPasses;

	
	vFXManager[0/*mesh->iCgFXIndex*/].pEffect->Begin(&uPasses, 0 );		
		
	for(UINT uPass = 0; uPass < uPasses; uPass++)
    {
        
		vFXManager[0/*mesh->iCgFXIndex*/].pEffect->Pass(uPass);
		pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)(mesh->iDrawMode+3),
									0,
									mesh->iNumVertices,
									mesh->iTriangles,
									mesh->p_Indices,
									D3DFMT_INDEX16,
									mesh->p_Vertices,
									sizeof(CVertex));

    }
 
    vFXManager[0/*mesh->iCgFXIndex*/].pEffect->End();


};

void CartGE::RenderQuad(void)
{

	CaVertex vertices[] =
	{
		{  1.0f,  1.0f, 0.5f, 1.0f,  0.0f, 0.0f,0.0f, 0.0f},
		{  800.0f,1.0f, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{  800.0f,600.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
		{  1.0f,   600.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f}

	};

	
	
	UINT uPasses;

	
    //vFXManager[QuadShader].pEffect->SetMatrix("View", &matView);

	//vFXManager[QuadShader].pEffect->SetMatrix("Projection", &matProjection);

	vFXManager[QuadShader].pEffect->SetTexture("tex1", aRTTex2);
	vFXManager[QuadShader].pEffect->SetTexture("tex2", aRTTex0);
	
	vFXManager[1].pEffect->SetValue("vec0", (void*)(FLOAT*)lightDir, sizeof(D3DXVECTOR3));

	vFXManager[QuadShader].pEffect->Begin(&uPasses, 0 );		
		
	for(UINT uPass = 0; uPass < uPasses; uPass++)
    {
        
		vFXManager[QuadShader].pEffect->Pass(uPass);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,
									2,
									&vertices,
									sizeof(CaVertex));


    }
 
    vFXManager[QuadShader].pEffect->End();


};

void CartGE::RenderMesh(CMesh *mesh)
{
	
	/*if (vFXManager[mesh->iCgFXIndex].iFrames==1)
		pDevice->SetTexture(0,vFXManager[mesh->iCgFXIndex].aTextur[0]);
			
	else
	{
		pDevice->SetTexture(0,vFXManager[mesh->iCgFXIndex].aTextur[Frame]);
	}
	*/
	
		vFXManager[mesh->iCgFXIndex].pEffect->SetMatrix("World", &matWorld);
	
		vFXManager[mesh->iCgFXIndex].pEffect->SetMatrix("View", &matView);

		D3DXMATRIX matShadowWVP, matLightWVP;
		
		D3DXMatrixMultiply(&matShadowWVP,&matWorld,&matViewS);
    
		


		D3DXMatrixMultiply(&matLightWVP,&matShadowWVP,&matProjection);
		
		D3DXMatrixMultiply(&matShadowWVP,&matShadowWVP,&matProjectionS);
	
		FLOAT fTexelOffsX = (0.5f / SMT_FOMAT_X);
		FLOAT fTexelOffsY = (0.5f / SMT_FOMAT_Y);
		D3DXMATRIX matTexAdj( 0.5,    0, 0, 0,
						    	0, -0.5, 0, 0,
							    0,    0, 0, 0,
						      0.5+fTexelOffsX,  0.5+fTexelOffsY, 1, 1);

		
		D3DXMatrixMultiply(&matShadowWVP,&matShadowWVP,&matTexAdj);


		vFXManager[mesh->iCgFXIndex].pEffect->SetMatrix("ShadowWVP", &matShadowWVP);
		vFXManager[mesh->iCgFXIndex].pEffect->SetMatrix("LightWVP", &matLightWVP);
	
	
	UINT uPasses;

	vFXManager[mesh->iCgFXIndex].pEffect->SetTexture("tex1", aShadowMap);
	
	vFXManager[mesh->iCgFXIndex].pEffect->SetValue("vec0", (void*)(FLOAT*)lightDir, sizeof(D3DXVECTOR3));

	vFXManager[mesh->iCgFXIndex].pEffect->Begin(&uPasses, 0 );		
		
	for(UINT uPass = 0; uPass < uPasses; uPass++)
    {
        
		vFXManager[mesh->iCgFXIndex].pEffect->Pass(uPass);
		pDevice->DrawIndexedPrimitiveUP((D3DPRIMITIVETYPE)(mesh->iDrawMode+3),
									0,
									mesh->iNumVertices,
									mesh->iTriangles,
									mesh->p_Indices,
									D3DFMT_INDEX16,
									mesh->p_Vertices,
									sizeof(CVertex));

    }
 
    vFXManager[mesh->iCgFXIndex].pEffect->End();


};




void CartGE::FrameBegin(void)
{
	pDevice->BeginScene();
		
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0 );
	

};
void CartGE::FrameEnd(void)
{
	RECT rDest0 = {0, 0, 150, 150};
	RECT rDest1 = {150, 0, 300,150};
	RECT rDest2 = {0, 150, 150, 300};
	RECT rDest3 = {150, 150, 300, 300};
	pDevice->StretchRect(aShadowMapSurf, NULL, pOldBackBuffer, &rDest0, D3DTEXF_NONE);
	
	//pDevice->StretchRect(aRTSurf0, NULL, pOldBackBuffer, &rDest0, D3DTEXF_NONE);
	//pDevice->StretchRect(aRTSurf1, NULL, pOldBackBuffer, &rDest1, D3DTEXF_NONE);
	//pDevice->StretchRect(aRTSurf2, NULL, pOldBackBuffer, &rDest2, D3DTEXF_NONE);
	//pDevice->StretchRect(aRTSurf3, NULL, pOldBackBuffer, &rDest3, D3DTEXF_NONE);
	
	this->Print(20, 20, "FPS: %i",this->FrameRate());
	
	pDevice->EndScene();

	pDevice->Present(NULL, NULL, NULL, NULL);
};

bool CartGE::Init(void)
{
   D3DVERTEXELEMENT9 vertDecl[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	{0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};



	D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI / 4, 800/600, 1.0f, 5000.0f);
    D3DXMatrixPerspectiveFovLH(&matProjectionS,  D3DX_PI / 4, 1, 1.0f, 5000.0f);
	

	//D3DXMatrixOrthoLH(&matProjectionS, 512.0f, 512.0f, 1.0f, 5000.0f);
	
	pDevice->CreateVertexDeclaration(vertDecl, &m_pVertDecl);
    pDevice->SetVertexDeclaration(m_pVertDecl);		

	D3DXMatrixIdentity(&matWorld);
	

	D3DXMatrixIdentity(&matView);
	

	pDevice->CreateTexture(SMT_FOMAT_X, SMT_FOMAT_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &aShadowMap, NULL);
    
	pDevice->CreateTexture(SMT_FOMAT_X, SMT_FOMAT_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &aRTTex0, NULL);
	pDevice->CreateTexture(SMT_FOMAT_X, SMT_FOMAT_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &aRTTex1, NULL);
	pDevice->CreateTexture(SMT_FOMAT_X, SMT_FOMAT_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &aRTTex2, NULL);
	pDevice->CreateTexture(SMT_FOMAT_X, SMT_FOMAT_Y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &aRTTex3, NULL);

	aShadowMap->GetSurfaceLevel( 0, &aShadowMapSurf );

	aRTTex0->GetSurfaceLevel( 0, &aRTSurf0 );
	aRTTex1->GetSurfaceLevel( 0, &aRTSurf1 );
	aRTTex2->GetSurfaceLevel( 0, &aRTSurf2 );
	aRTTex3->GetSurfaceLevel( 0, &aRTSurf3 );
	
	//QuadShader = LoadFX("shader/quad.fx");

	pDevice->CreateDepthStencilSurface(SMT_FOMAT_X, SMT_FOMAT_Y, 
		D3DFMT_D16, D3DMULTISAMPLE_NONE, NULL, FALSE, &aShadowMapZ, NULL);

    pDevice->SetSamplerState (0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState (0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState (1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState (1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	
	
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
	pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_AMBIENT, 0x55555555);
	pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	//pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	
	HFONT hFont = CreateFont(42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, "Verdana");
	D3DXCreateFont(pDevice, hFont, &pFont); 

    g_font=new CD3DFont("Arial", //The name of the font we want to load
                       26,      //The size of the font
                     D3DFONT_BOLD); //Flags supported are:D3DFONT_ITALIC and D3DFONT_BOLD

	 //This set us SOME of the required objects
      g_font->InitDeviceObjects(pDevice);

   //This sets up the rest of the required objects.  It's not immediately obvious
   //that you need to call RestoreDeviceObjects before youuse it the first time, but
   //you do.  The reason is that the objects set up in InitDeviceObjects will survive
   //a device reset where as these ones won't.  Bad design in my opinion, I think it'd
   //be better to have InitDeviceObjects call RestoreDeviceObjects, but what do I know?
   g_font->RestoreDeviceObjects();
	return true;
};

bool CartGE::Create(HWND hWnd, int width, int height, bool g_bFullscreen)
{
	pDirect3D=Direct3DCreate9(D3D_SDK_VERSION); // Create Direct3D
	if (pDirect3D==NULL) // Check if it succeeded, if it didn't, return failure
	{	return false;
	}
	

	D3DDISPLAYMODE displayMode; // Structure to hold display mode info, like the resolution
	if (g_bFullscreen==false) // If it's not in fullscreen, we fill that structure with the current display mode
	{
		// Get the current display mode on the default display adapter
		g_hr=pDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &displayMode); 
		if (FAILED(g_hr)) { // If the above call failed, return failure
							
							return false;
							} 
	    					
	}
	else
	{
		displayMode.Width=width; // Set the screen width to the window width
		displayMode.Height=height; // Set the screen height to the window height
		displayMode.RefreshRate=0; // Set the refresh rate to default
		displayMode.Format=D3DFMT_A8R8G8B8; // Set the color format to 565 16 bit
		
	}


	D3DPRESENT_PARAMETERS presentParameters; // Used to explain to Direct3D how it will present things on the screen
	memset(&presentParameters, 0, sizeof(D3DPRESENT_PARAMETERS)); // We need to fill it with 0's first

	if (g_bFullscreen==false) // If the user answered "no" when we asked whether they'd like to use fullscreen
	{ 
		presentParameters.Windowed   = TRUE; // Then set windowed to true
	}

	else // Otherwise
	{
		presentParameters.Windowed   = FALSE; // Set windowed to false
		presentParameters.FullScreen_RefreshRateInHz      =	D3DPRESENT_RATE_DEFAULT;
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Tells Direct3D to present the graphics to the screen the quickest way possible 
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	presentParameters.BackBufferFormat = displayMode.Format; // The color format
	presentParameters.BackBufferWidth = width;//displayMode.Width; // The back buffer width
	presentParameters.BackBufferHeight = height;//displayMode.Height; // The back buffer height
	presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	
	
	

	
	// Create the device
	g_hr=pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &presentParameters, &pDevice ); 
	if (FAILED(g_hr)) // If it failed, return failure
	{
		return false; 
	}


return true; 

}