
// CarConDlg.h: 头文件
//
#define DIRECTINPUT_VERSION 0x0800
#pragma once
#include "LogitechSteeringWheelLib.h"
#pragma comment(lib, "LogitechSteeringWheelLib.lib")
#include "map"

#include "framework.h"
#include "afxdialogex.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "Send.h"


// CCarConDlg 对话框
class CCarConDlg : public CDialogEx
{
// 构造
public:
	CCarConDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CARCON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();

public:
	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedShutdown();
	afx_msg void OnBnClickedCam();
	afx_msg void OnClose();
	CStatic speed;
	CStatic dir;
	Sender sender;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool camClosed;
	int index_;
	long GetControlValue(int device, int control);
	DIJOYSTATE2* m_DIJoyState2Device;
	bool m_isTimerActive;
};
