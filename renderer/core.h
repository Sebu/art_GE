/******************************************************************************************

    >>artGE RenderCore<<
	>>Main Renderer Header

    Project: artGE

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 02.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 02.11.2002
*/



#ifndef __CORE_H__
#define __CORE_H__

#include "global.h"

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif





class API_EXPORT CartGE
{
public:
	D3DXVECTOR3 lightDir;

	HRESULT g_hr; //RESULT_HANDLE
	IDirect3D9 *pDirect3D; 
	IDirect3DDevice9 *pDevice;
	DWORD m_hVertexShader;
	ID3DXFont *pFont;  //TEXT SYSTEM
	CD3DFont *g_font;
	CLog artGELog; // LOG_FILE
	vector< CFX > vFXManager;
	int Frame;
	float fFrame;
	
	
	D3DXMATRIX  matWorld, matView, matViewS, matProjection, matProjectionS;

	LPDIRECT3DTEXTURE9		aShadowMap;
	LPDIRECT3DTEXTURE9		aRTTex0,aRTTex1,aRTTex2,aRTTex3;
	LPDIRECT3DSURFACE9		aRTSurf0,aRTSurf1,aRTSurf2,aRTSurf3;
	LPDIRECT3DSURFACE9		aShadowMapSurf;
	LPDIRECT3DSURFACE9		aShadowMapZ;
	LPDIRECT3DSURFACE9 pOldBackBuffer, pOldZBuffer;
    int QuadShader;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertDecl;

	//DynamicVB< CVertex > *t_DynamicVB;
	//DynamicIB< WORD >	 *t_DynamicIB;
	//REV_ShaderFrame		 *p_Frames;

	
public:
	CTimer	Timer;
	//CartGE(void);
	~CartGE(void);
	
	bool Create(HWND hWnd, int width, int height, bool g_bFullscreen);
	bool Init(void);
	int FrameRate(void);
	void Print(int x, int y, const char *fmt, ...);
	void FrameBegin(void);
	void FrameEnd(void);
	
	

	int LoadFX(char *pFilename);
//	void HSR(CScene *scene);
//	void Render(CScene *scene);

	void RenderMesh(CMesh *mesh);
	void RenderQuad(void);
	void RenderShadowMesh(CMesh *mesh);
	void RenderMap(CMap *map);
	void RenderBla(CMap *map);
	void RenderShadowedMap(CMap *map);
	void RenderPortal(CPortal *portal);

	void CameraSet(D3DXMATRIX *matView);
	bool LoadMesh(char *p_Filename,CMesh *mesh);
	void LoadMap(char *p_Filename,CMap *map);
	
	void TakeScreenShot(char* file_name, int screenx, int screeny);
	void TextureLoad(char *p_Filename, LPDIRECT3DTEXTURE9 *p_Textur);
};

//#define D3DFVF_VERTEX (D3DFVF_XYZ |  D3DFVF_NORMAL | D3DFVF_TEX1)
//#define D3DFVF_PORTAL (D3DFVF_XYZ)



void SetFrustumMatrix( D3DMATRIX& mat, 
                                     FLOAT fLeft, FLOAT fRight, FLOAT fTop,
                                     FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );

HRESULT SetAdjustedProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,
                                     FLOAT fNearPlane, FLOAT fFarPlane,
                                     FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
                                     FLOAT fVPHeight );



#define D3DFVF_CAVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

#endif __CORE_H__