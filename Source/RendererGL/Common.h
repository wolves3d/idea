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

#include <GL/gl.h>
#include <GL/glu.h>
#include "Core/Extensions/GLext.h"
#include "Core/Extensions/Extensions.h"

//------------------------------------------------------------------------------
//	Tip: Shared
//------------------------------------------------------------------------------
#include "Base.h"
#include "API/ISystem.h"
#include "Memory.h"
#include "Math/3dmath.h"

//------------------------------------------------------------------------------
//	Tip: Templates
//------------------------------------------------------------------------------
#include "Templates/String.h"
#include "Templates/SafePointer.h"
#include "Templates/Container.h"
#include "Templates/Vector.h"

#include "API/IVfs.h"
#include "API/IRenderer.h"
#include "API/IEngine.h"
#include "API/IXMLWrapper.h"

//------------------------------------------------------------------------------
//	Tip: Renderer_GL
//------------------------------------------------------------------------------
#include "Renderer_GL.h"

////////////////////////////////////////////////////////////////////////////////

#endif // #ifndef __Common_h_included__

////////////////////////////////////////////////////////////////////////////////