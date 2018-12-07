#ifndef _DX11_INCLUDE_H_
#define _DX11_INCLUDE_H_

#include "../../Define/Define.h"
#include "WICTextureLoader.h"

#pragma warning (disable : 4005)
#pragma warning (disable : 4838)
#include <xnamath.h>
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib, "d3d11.lib")
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "d3dx11d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#endif

#endif // _DX11_INCLUDE_H