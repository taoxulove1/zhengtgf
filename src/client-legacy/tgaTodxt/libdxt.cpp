// ==========================================================================
// Conglin 2008-04-16
// ==========================================================================
#include <windows.h>
#include <set>
#include <assert.h>
#include <d3d8.h>
#include <d3dx8math.h>
#include <dxerr8.h>
#include "libdxt.h"

#pragma comment( lib, "d3d8.lib" )
#pragma comment( lib, "D3dx8.lib" )
#pragma comment( lib, "Dxerr8.lib" )

#pragma comment( lib, "d3dxof.lib" )
#pragma comment( lib, "dxguid.lib" )

#define ROUNDNUM2(value,num) ( ((value) + (num-1)) & (~(num-1)) )	// [绕指定数num的2幂对齐][Conglin]

inline DWORD GetWidth4(DWORD dwWidth)
{
	return ROUNDNUM2(dwWidth,4);
}

IDirect3DTexture8* LoadTexture( IDirect3DDevice8* pDevice8, D3DFORMAT vFormat, const char* pFileName )
{
	if( pDevice8 == NULL ) return NULL;

	IDirect3DTexture8* tpTexture = NULL;
	D3DXIMAGE_INFO	tSrcInfo;
	HRESULT tResult = D3DXCreateTextureFromFileEx( pDevice8, 
		pFileName,
		0,
		0,
		1,
		0,
		vFormat,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&tSrcInfo,
		0,
		&tpTexture );

	if( FAILED( tResult ) )
	{
		return NULL;
	}

	return tpTexture;
}

IDirect3DTexture8* CreateTexture( IDirect3DDevice8* pDevice8, D3DFORMAT vFormat, int vWidth, int vHeight )
{
	if( pDevice8 == NULL ) return NULL;
	IDirect3DTexture8* tpTexture = NULL;
	
	HRESULT tResult = D3DXCreateTexture( pDevice8,
		vWidth,
		vHeight,
		1,
		0,
		vFormat,
		D3DPOOL_MANAGED,
		&tpTexture );

	if( FAILED( tResult ) )
	{		
		return NULL;
	}
	return tpTexture;
}


void ReleaseTex( IDirect3DTexture8*& rpTexture )
{
	if( rpTexture != NULL )
	{
		rpTexture->Release( );
		rpTexture = NULL;
	}
}

void* LockTex( IDirect3DTexture8*& rpTexture, const RECT* pRect, int* pOutPitch )
{
	if( rpTexture == NULL ) return NULL;

	D3DLOCKED_RECT tLocked;
	HRESULT tResult = rpTexture->LockRect( 0, &tLocked, pRect, D3DLOCK_READONLY );	 
	
	if( SUCCEEDED( tResult ) )
	{
		if( pOutPitch != NULL )
			*pOutPitch = tLocked.Pitch;	
		return tLocked.pBits;
	}
	return NULL;
}

void UnlockTex( IDirect3DTexture8*& rpTexture )
{
	if( rpTexture == NULL ) return;
	rpTexture->UnlockRect( 0 );
}

unsigned int GetTexWidth( IDirect3DTexture8*& rpTexture )
{
	D3DSURFACE_DESC tDesc;
	HRESULT tResult = rpTexture->GetLevelDesc( 0, &tDesc );

	if( SUCCEEDED( tResult  ) )
		return tDesc.Width;
	else
		return 0;
}

unsigned int GetTexHeight( IDirect3DTexture8*& rpTexture )
{
	D3DSURFACE_DESC tDesc;
	HRESULT tResult = rpTexture->GetLevelDesc( 0, &tDesc );

	if( SUCCEEDED( tResult  ) )
		return tDesc.Height;
	else
		return 0;
}

RECT GetTexMinRect( IDirect3DTexture8*& rpTexture, const RECT* pRect )
{
	RECT tRect = { -1, -1, -1, -1 };
	if( rpTexture == NULL ) return tRect;

	int tWidth	= GetTexWidth( rpTexture );
	int tHeight = GetTexHeight( rpTexture );

	if( pRect != NULL )
	{
		tWidth	= GetRectWidth( *pRect );
		tHeight = GetRectHeight( *pRect );
	}

	int		tPadding	= 0;
	DWORD*	tpData		= (DWORD*)LockTex( rpTexture, pRect, &tPadding );
	tPadding /= 4;	

	for( int y = 0; y < tHeight;  )
	{
		for( int x1 = 0; x1 < tWidth; x1 ++ )
		{
			if( (tpData[ y * tPadding + x1 ] & 0xff000000) != 0 )
			{
				if( tRect.top == -1 )
					tRect.top = y;

				if( tRect.left == -1 || x1 < tRect.left )
					tRect.left = x1;

				for( int x2 = tWidth - 1; x2 >= x1; x2 -- )
				{
					if( (tpData[ y * tPadding + x2 ] & 0xff000000) != 0 )
					{
						if( tRect.bottom == -1 || tRect.bottom <= y )
							tRect.bottom = y + 1; // y

						if( tRect.right == -1 || x2 >= tRect.right )
							tRect.right = x2 + 1; // x2
						goto __NEXT_LINE;
					}
				}
			}
		}
__NEXT_LINE:
		y ++;
	}
	UnlockTex( rpTexture );	
	return tRect;
}


// ==========================================================================
// 功能:	获得TGA图片的最小有效矩形区域.
// 参数:	pD3Device8[D3D8设备指针],
//			pFileName[Tga文件名称], 
//			pOutRect[输出图片最小有效矩形区域]
//			pClipRect[限制裁剪区域], 如pClipRect==0,则取得整个图片有效区域
// 返回:	返回布尔型.
//			false[失败]
//			true[成功]
// ==========================================================================
 bool GetTgaImageMinRect( void* pD3Device8, const char* pFileName, RECT* pOutRect, const RECT* pClipRect )
 {
	if( pFileName == NULL ) return false;
	IDirect3DTexture8* tpTexture = LoadTexture( (IDirect3DDevice8*)pD3Device8, D3DFMT_A8R8G8B8, pFileName );
	if( tpTexture == NULL ) return false;

	RECT tRect = GetTexMinRect( tpTexture, pClipRect );
	if( pOutRect != NULL )
		*pOutRect = tRect;
	return true;
 }

// ==========================================================================
// 功能:	获得TGA图片的目标区域的DXT3纹理格式大小[字节]
// 参数:	pD3Device8[D3D8设备指针],
//			pFileName[Tga文件名称], 
//			pDstRect[目标区域]
// 返回:	返回大小字节
//			返回0,[失败],
//			返回非0,[成功]
// ==========================================================================
unsigned int GetTgaImageDXT3Size( void* pD3Device8, const char* pFileName, const RECT* pDstRect )
{
	if( pDstRect != NULL )
	{
		int tW = GetWidth4( GetRectWidth( *pDstRect ) );
		int tH = GetWidth4( GetRectHeight( *pDstRect ) );
		//return tW * tH;
		return (tW / 4) * (tH / 4) * 16;
	}
	else
	{
		if( pD3Device8 == NULL ) return 0;
		if( pFileName == NULL ) return 0;
		IDirect3DTexture8* tpTexture = LoadTexture((IDirect3DDevice8*)pD3Device8, D3DFMT_A8R8G8B8, pFileName );
		if( tpTexture == NULL ) return 0;
		int tW = GetWidth4( GetTexWidth( tpTexture ) );
		int tH = GetWidth4( GetTexHeight( tpTexture ) );
		return (tW / 4) * (tH / 4) * 16;
	}
	
	return 0;
}

unsigned int GetDXT3Pitch( void* pD3Device8, const char* pFileName, const RECT* pDstRect )
{
	return GetDXT3RowSize( pD3Device8, pFileName, pDstRect );
}

unsigned int GetDXT3RowCount( void* pD3Device8, const char* pFileName, const RECT* pDstRect )
{
	if( pDstRect != NULL )
	{		
		int tH = GetWidth4( GetRectHeight( *pDstRect ) );	
		return (tH / 4);
	}
	else
	{
		if( pD3Device8 == NULL ) return 0;
		if( pFileName == NULL ) return 0;
		IDirect3DTexture8* tpTexture = LoadTexture((IDirect3DDevice8*)pD3Device8, D3DFMT_A8R8G8B8, pFileName );
		if( tpTexture == NULL ) return 0;		
		int tH = GetWidth4( GetTexHeight( tpTexture ) );
		return (tH / 4);
	}
	return 0;
}

unsigned int GetDXT3RowSize( void* pD3Device8, const char* pFileName, const RECT* pDstRect )
{
	if( pDstRect != NULL )
	{
		int tW = GetWidth4( GetRectWidth( *pDstRect ) );		
		return (tW / 4) * 16;
	}
	else
	{
		if( pD3Device8 == NULL ) return 0;
		if( pFileName == NULL ) return 0;
		IDirect3DTexture8* tpTexture = LoadTexture((IDirect3DDevice8*)pD3Device8, D3DFMT_A8R8G8B8, pFileName );
		if( tpTexture == NULL ) return 0;		
		int tW = GetWidth4( GetTexWidth( tpTexture ) );
		return (tW / 4) * 16;
	}
	return 0;
}

// ==========================================================================
// 功能:	获得TGA图片的目标区域的DXT3纹理格式数据
// 参数:	pD3Device8[D3D8设备指针],
//			pFileName[Tga文件名称], 
//			pDstRect[目标区域]
//			pOutData[输出数据指针]
// 返回:	返回布尔型.
//			false[失败]
//			true[成功]
// ==========================================================================
bool GetTgaImageDXT3Data( void* pD3Device8, const char* pFileName, const RECT* pDstRect, unsigned char* pOutData )
{
	IDirect3DTexture8* tpTexture = LoadTexture((IDirect3DDevice8*)pD3Device8, D3DFMT_DXT3, pFileName );
	if( tpTexture == NULL ) return false;

	int tPitch = 0;
	BYTE* tpSrcData = (BYTE*)LockTex( tpTexture, pDstRect, &tPitch );
	if( tpSrcData != NULL )
	{	
		unsigned int tTexRowSize = GetDXT3RowSize( pD3Device8, pFileName, pDstRect );
		unsigned int tTexRowCount = GetDXT3RowCount( pD3Device8, pFileName, pDstRect );

		BYTE* tpDstData = pOutData;
		for( unsigned int row = 0; row < tTexRowCount; row++ )
		{
			memcpy( tpDstData, tpSrcData, tTexRowSize );
			tpDstData	+= tTexRowSize;
			tpSrcData	+= tPitch;
		}
		UnlockTex( tpTexture );		
	}

	return true;
}
