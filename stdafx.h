// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <include/base/cef_build.h>
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_command_line.h>

#pragma comment(lib, "libcef.lib")
#if defined _DEBUG || defined DEBUG
#pragma comment(lib, "libcef_dll_wrapperd.lib")
#else
#pragma comment(lib, "libcef_dll_wrapper.lib")
#endif

// TODO: 在此处引用程序需要的其他头文件
