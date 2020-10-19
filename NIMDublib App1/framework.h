// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <NIM_Duilib/NIM_Duilib.h>
#pragma comment(lib, "NIM_Duilib/base")
#pragma comment(lib, "NIM_Duilib/duilib")
#pragma comment(lib, "NIM_Duilib/ui_components")

#include <curl/curl.h>
#pragma comment(lib, "libcurl_imp.lib")

#include <cpr/cpr.h>
#pragma comment(lib, "cpr")

#include <zlib/zlib.h>
#pragma comment(lib, "zlib")
#include <libpng/png.h>
#pragma comment(lib, "libpng16")

#include<zxing/ReadBarcode.h>
#include <zxing/TextUtfEncoding.h>
#pragma comment(lib, "ZXingCore")
using namespace ZXing;
#include <iostream>
using namespace std;
#include <json/json.h>
#pragma comment(lib, "json_vc71_libmt")
using namespace Json;

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>

#pragma comment(lib, "log4cpp")

#include "Define.h"
