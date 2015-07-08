////////////////////////////////////////////////////////////////////////////////

#ifndef __Common_h_included__
#define __Common_h_included__

////////////////////////////////////////////////////////////////////////////////

#define ENGINE_IMPL
#define USE_MEMORY_MANAGER

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//	Tip: Win32
//------------------------------------------------------------------------------
#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

//------------------------------------------------------------------------------
//	Tip: Shared
//------------------------------------------------------------------------------
#include "_Shared/Base.h"
#include "_Shared/Math/3dmath.h"
#include "_Shared/Memory.h"

//------------------------------------------------------------------------------
//	Tip: Templates
//------------------------------------------------------------------------------
#include "_Shared/Templates/SafePointer.h"
#include "_Shared/Templates/Container.h"
#include "_Shared/Templates/String.h"
#include "_Shared/Templates/Vector.h"
#include "_Shared/Templates/HashTable.h"
#include "_Shared/Templates/Stack.h"

//------------------------------------------------------------------------------
//	Tip: API
//------------------------------------------------------------------------------
#include "_Shared/API/IVFS.h"
#include "_Shared/API/IRenderer.h"
#include "_Shared/API/IEngine.h"
#include "_Shared/API/IInput.h"
#include "_Shared/API/IXMLWrapper.h"
#include "_Shared/API/ISoundMgr.h"
#include "_Shared/API/IResManager.h"

#include "_Shared/UnitTest.h"



//------------------------------------------------------------------------------
//	Tip: Engine
//------------------------------------------------------------------------------
#include "Core/Engine.h"
#include "_Shared/Engine/UI.h"

////////////////////////////////////////////////////////////////////////////////

extern IXMLWrapper	* g_pXMLWrapper;
extern ISoundMgr	* g_pSoundMgr;
extern IResManager	* g_pResManager;

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Common_h_included__

////////////////////////////////////////////////////////////////////////////////