
// CarCon.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once
#include "pch.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// CCarConApp:
// 有关此类的实现，请参阅 CarCon.cpp
//

enum CALIBRATE_CONTROL {
	SKIP = -1,
	SAVE = -2,
	CANCEL = -3,

	WHEEL = 0,
	ACCELERATOR = 1,
	BRAKE = 2,
	CLUTCH = 3,

	X_AXIS = 1,
	Y_AXIS = 2,
	Z_AXIS = 3,
	SLIDER_0 = 4,
	SLIDER_1 = 5,
	X_ROT = 6,
	Y_ROT = 7,
	Z_ROT = 8,
};
class CCarConApp : public CWinApp
{
public:
	CCarConApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCarConApp theApp;
