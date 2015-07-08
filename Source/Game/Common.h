////////////////////////////////////////////////////////////////////////////////

#ifndef __Common_h_included__
#define __Common_h_included__

////////////////////////////////////////////////////////////////////////////////

#define USE_MEMORY_MANAGER

////////////////////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

//==============================================================================
//	Tip: Shared
//==============================================================================
#include "_Shared/Base.h"
#include "_Shared/Memory.h"

//------------------------------------------------------------------------------
//	Tip: Templates
//------------------------------------------------------------------------------
#include "_Shared/Templates/Container.h"
#include "_Shared/Templates/String.h"
#include "_Shared/Templates/SafePointer.h"
#include "_Shared/Templates/Vector.h"
#include "_Shared/Templates/HashTable.h"

#include "_Shared/Math/3dmath.h"

//------------------------------------------------------------------------------
//	Tip: API
//------------------------------------------------------------------------------
#include "_Shared/API/ISystem.h"
#include "_Shared/API/IVfs.h"
#include "_Shared/API/IRenderer.h"
#include "_Shared/API/IInput.h"
#include "_Shared/API/IEngine.h"
#include "_Shared/API/IGame.h"
#include "_Shared/API/ISoundMgr.h"
#include "_Shared/API/IResManager.h"

#include "_Shared/Engine/UI.h"

#include "Core/Game.h"

//==============================================================================
// Tip: Global Vars
//==============================================================================
extern IRenderer *	g_pRenderer;
extern IInput *		g_pInput;
extern IVfs *		g_pVfs;
extern ISoundMgr *	g_pSoundMgr;

extern IEngine *		g_pEngine;
extern IUIManager *		g_pUIManager;
extern IEntityManager *	g_pEM;
extern IWorldIter *		g_pWorld;
extern IVarStorage *	g_pVarStorage;
extern IResManager *	g_pResManager;


////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Common_h_included__

////////////////////////////////////////////////////////////////////////////////