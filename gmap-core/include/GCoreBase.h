#include "GTypes.h"
#include "GVector2d.h"
#include "GRectangle2d.h"
#include "GLine.h"
#include "GMathEngine.h"
#include "GRange.h"


#include <Windows.h>
#include <gdiplus.h>
using namespace Gdiplus;

#ifdef GMAPCORE_EXPORTS
	#define GMAPCORE_API __declspec(dllexport)
#else
	#define GMAPCORE_API __declspec(dllimport)
#endif

