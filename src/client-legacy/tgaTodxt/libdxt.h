// ==========================================================================
// Conglin 2008-04-16
// ==========================================================================
#ifndef LIBDXT_INC
#define LIBDXT_INC

#ifdef DXT_EXPORT
	#define __QuDllExport __declspec( dllexport )
#else
	#define __QuDllExport __declspec( dllimport )
#endif

#define GetRectWidth( rect )	((rect).right - (rect).left)		// 得到矩形的宽度
#define GetRectHeight( rect )	((rect).bottom - (rect).top)		// 得到矩形的高度

#if defined( __cplusplus )
extern "C"
{
#endif
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
 __QuDllExport bool GetTgaImageMinRect( void* pD3Device8, const char* pFileName, RECT* pOutRect, const RECT* pClipRect = 0 );

// ==========================================================================
// 功能:	获得TGA图片的目标区域的DXT3纹理格式大小[字节]
// 参数:	pD3Device8[D3D8设备指针],
//			pFileName[Tga文件名称], 
//			pDstRect[目标区域]
// 返回:	返回大小字节
//			返回0,[失败],
//			返回非0,[成功]
// ==========================================================================
__QuDllExport unsigned int GetTgaImageDXT3Size( void* pD3Device8, const char* pFileName, const RECT* pDstRect );

__QuDllExport unsigned int GetDXT3Pitch( void* pD3Device8, const char* pFileName, const RECT* pDstRect );
__QuDllExport unsigned int GetDXT3RowCount( void* pD3Device8, const char* pFileName, const RECT* pDstRect );
__QuDllExport unsigned int GetDXT3RowSize( void* pD3Device8, const char* pFileName, const RECT* pDstRect );

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
__QuDllExport bool GetTgaImageDXT3Data( void* pD3Device8, const char* pFileName, const RECT* pDstRect, unsigned char* pOutData );

#if defined( __cplusplus )
};
#endif
#endif