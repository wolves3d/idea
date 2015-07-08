////////////////////////////////////////////////////////////////////////////////

#ifndef __Common_h_included__
#define __Common_h_included__

////////////////////////////////////////////////////////////////////////////////

#define USE_MEMORY_MANAGER
#define RENDERER_IMPL

////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//	Tip: Shared
//------------------------------------------------------------------------------
#include "_Shared/Base.h"
#include "_Shared/API/ISystem.h"
#include "_Shared/Memory.h"
#include "_Shared/Math/3dmath.h"

//------------------------------------------------------------------------------
//	Tip: Templates
//------------------------------------------------------------------------------
#include "_Shared/Templates/String.h"
#include "_Shared/Templates/SafePointer.h"
#include "_Shared/Templates/Container.h"
#include "_Shared/Templates/Vector.h"

#include "_Shared/API/IRenderer.h"
#include "_Shared/API/IResManager.h"
#include "_Shared/API/IVfs.h"
#include "_Shared/API/IXMLWrapper.h"

//------------------------------------------------------------------------------
//	Tip: RendererD3D
//------------------------------------------------------------------------------
#include "RendererD3D.h"

extern IResManager * g_pResManager;
extern IXMLWrapper * g_pXMLWrapper;

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Common_h_included__

////////////////////////////////////////////////////////////////////////////////