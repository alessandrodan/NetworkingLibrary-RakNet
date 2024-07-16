#pragma once

#ifdef _WIN32
#	ifdef _DEBUG
#		pragma comment(lib, "SLikeNet_LibStatic_Debug_Win32.lib")
#	else
#		pragma comment(lib, "SLikeNet_LibStatic_Release_Win32.lib")
#	endif
#endif
