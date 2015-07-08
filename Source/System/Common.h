////////////////////////////////////////////////////////////////////////////////

#ifndef __Common_h_included__
#define __Common_h_included__

////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE
#define USE_MEMORY_MANAGER

////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
//	Tip: Shared
//------------------------------------------------------------------------------
#define SYSTEM_IMPLEMENTATION
#include "_Shared/Base.h"
#include "_Shared/Math/3dmath.h"

#include "_Shared/Templates/String.h"
#include "_Shared/Templates/Container.h"
#include "_Shared/Templates/SafePointer.h"

#include "_Shared/API/IVfs.h"
//#include "_Shared/API/IRenderer.h"
//#include "_Shared/API/IEngine.h"
#include "_Shared/API/IInput.h"

#include "_Shared/Math/3dmath.h"

//------------------------------------------------------------------------------
//	Tip: System
//------------------------------------------------------------------------------
#include "Core/System.h"

////////////////////////////////////////////////////////////////////////////////

extern IVarStorage * g_pVarStorage;

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Common_h_included__

////////////////////////////////////////////////////////////////////////////////