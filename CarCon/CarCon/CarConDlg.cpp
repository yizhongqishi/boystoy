
// CarConDlg.cpp: 实现文件
//
#include "pch.h"
#include "CarCon.h"
#include "CarConDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCarConDlg 对话框



CCarConDlg::CCarConDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CARCON_DIALOG, pParent), m_isTimerActive(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCarConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPEED, speed);
	DDX_Control(pDX, IDC_DIR, dir);
}

BEGIN_MESSAGE_MAP(CCarConDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INIT, &CCarConDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_SHUTDOWN, &CCarConDlg::OnBnClickedShutdown)
	ON_BN_CLICKED(IDC_CAM, &CCarConDlg::OnBnClickedCam)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCarConDlg 消息处理程序

BOOL CCarConDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	camClosed = TRUE;
	index_ = 0;
	// TODO: 在此添加额外的初始化代码
	// 
	//显示控制台
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CON", "w", stdout);
	GetDlgItem(IDC_INIT)->SetFocus();

	SetTimer(1, 1000 / 30, NULL);
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCarConDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCarConDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCarConDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCarConDlg::OnBnClickedInit()
{
	// TODO: 在此添加控件通知处理程序代码
	camClosed = TRUE;
	speed.SetWindowTextW(L"0");
	dir.SetWindowTextW(L"0");
	LogiSteeringInitializeWithWindow(true, m_hWnd);
	sender.initialization("192.168.2.103");
}

void CCarConDlg::OnBnClickedShutdown()
{
	// TODO: 在此添加控件通知处理程序代码
	sender.close();
	LogiSteeringShutdown();
	camClosed = TRUE;
}



void CCarConDlg::OnBnClickedCam()
{
	if (!camClosed) return;
	// TODO: 在此添加控件通知处理程序代码
	cv::VideoCapture cap;
	camClosed = FALSE;
	cap.open("http://192.168.2.103:81");
	cv::Mat cam;
	if (!cap.isOpened()) {
		return;
	}
	cv::namedWindow("picView", cv::WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("picView");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	while (!camClosed) {
		cap >> cam;
		if (cam.empty()) break;
		imshow("picView", cam);
		cv::waitKey(30);
	}
	::DestroyWindow(hParent);
	cap.release();
}


void CCarConDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	camClosed = TRUE;
	sender.close();
	LogiSteeringShutdown();
	CDialogEx::OnClose();

}

long CCarConDlg::GetControlValue(int device, int control)
{
	if (control == WHEEL)
	{
		return m_DIJoyState2Device->lX;
	}
	if (control == ACCELERATOR)
	{
		return m_DIJoyState2Device->lY;
	}
	if (control == BRAKE)
	{
		return m_DIJoyState2Device->lRz;
	}
	if (control == CLUTCH)
	{
		return m_DIJoyState2Device->rglSlider[0];
	}

	return 0;
}

void CCarConDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UNREFERENCED_PARAMETER(nIDEvent);

	int index_ = 0;
	float speedParam = 0.0f;
	float brakeParam = 0.0f;
	long direction = 0;

	TCHAR sBuffer_[128];
	TCHAR deviceConnected_[128];

	int counter_ = 0;
	// Update the input device every timer message.

	//if the return value is false, means that the application has not been initialized yet and there is no hwnd available
	if (!LogiUpdate()) return;

	//if the timer is not active return immediately
	if (!m_isTimerActive) return;

	//// call this every frame in case a wheel gets plugged in.
	LogiGenerateNonLinearValues(0, -40);
	LogiGenerateNonLinearValues(1, 80);

	//TCHAR deviceConnected_[128];

	if (LogiIsConnected(index_))
	{
		// Find out if axes are separate or not. If combined, or
		// if we fail, use Y axis for gas and brake.
		wchar_t deviceName[128];
		LogiGetFriendlyProductName(0, deviceName, 128);
		wsprintf(deviceConnected_, deviceName);

		m_DIJoyState2Device = LogiGetState(index_);
		
		if (LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_LOGITECH) && LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL))
		{
			LogiControllerPropertiesData propertiesData_;
			ZeroMemory(&propertiesData_, sizeof(propertiesData_));

			LogiGetCurrentControllerProperties(index_, propertiesData_);



			// calculate normalized speed parameter. In a real
			// game the parameter could go from 0 at a stop to 1
			// at a speed of about 50 to 80 miles/hour.
			if (propertiesData_.combinePedals)
			{
				speedParam = __max(((-(float)(GetControlValue(index_, ACCELERATOR))) / 32767), 0);
				brakeParam = __max((((float)(GetControlValue(index_, BRAKE))) / 32767), 0);
			}
			else
			{
				speedParam = ((-(float)(GetControlValue(index_, ACCELERATOR))) / 65535) + float(0.5);
				brakeParam = ((-(float)(GetControlValue(index_, BRAKE))) / 65535) + float(0.5);
			}
		}
		else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL) && LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_MICROSOFT))
		{
			// Microsoft wheel
			wsprintf(deviceConnected_, TEXT("Steering wheel, combined pedals"));
			speedParam = __max(((-(float)(GetControlValue(index_, ACCELERATOR))) / 32767), 0);
			brakeParam = __max((((float)(GetControlValue(index_, BRAKE))) / 32767), 0);
		}
		else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_WHEEL) && LogiIsManufacturerConnected(index_, LOGI_MANUFACTURER_OTHER) && m_DIJoyState2Device != NULL)
		{
			// Immersion wheel in combined mode
			if (m_DIJoyState2Device->lRz == 32767
				&& m_DIJoyState2Device->lY == 0
				&& m_DIJoyState2Device->rglSlider[0] != 0
				&& m_DIJoyState2Device->lZ == 0)
			{
				wsprintf(deviceConnected_, TEXT("Steering wheel, combined pedals"));
				speedParam = __max(((-(float)(m_DIJoyState2Device->rglSlider[0])) / 32767), 0);
				brakeParam = __max((((float)(m_DIJoyState2Device->rglSlider[0])) / 32767), 0);
			}

			// Immersion wheel in separate mode
			else if (m_DIJoyState2Device->lRz == 32767
				&& m_DIJoyState2Device->lY != 0
				&& m_DIJoyState2Device->rglSlider[0] != 0
				&& m_DIJoyState2Device->lZ == 0)
			{
				wsprintf(deviceConnected_, TEXT("Steering wheel, separate pedals"));
				speedParam = ((-(float)(m_DIJoyState2Device->lY)) / 65535) + float(0.5);
				// TODO: brake
			}
		}
		else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_JOYSTICK))
		{
			speedParam = __max(((-(float)(m_DIJoyState2Device->rglSlider[0])) / 32767), 0);
			brakeParam = __max((((float)(m_DIJoyState2Device->rglSlider[0])) / 32767), 0);
		}

		// Game pad
		else if (LogiIsDeviceConnected(index_, LOGI_DEVICE_TYPE_GAMEPAD))
		{
			speedParam = __max((((float)(m_DIJoyState2Device->lY)) / 32767), 0);
			brakeParam = __max(((-(float)(m_DIJoyState2Device->lY)) / 32767), 0);
		}
		else
		{
			wsprintf(deviceConnected_, TEXT("Steering wheel"));
			speedParam = 0;
			brakeParam = 0;
		}

		LogiPlayLeds(index_, speedParam, 0.1f, 1.0f);

		// Play spring force
		//LogiPlaySpringForce(index_, 0, int(70 * speedParam_), int(70 * speedParam_));

		// Play Damper Force
		//LogiPlayDamperForce(index_, int(80 * (1 - speedParam_)));

		// Play Collisions

		//sender.SendtoC(m_DIJoyState2Device[index_]->lX, int(1000 * speedParam_[index_]), int(1000 * brakeParam_[index_]));

		direction = m_DIJoyState2Device->lX;

		sender.SendtoC_PWM(direction, int(1000 * speedParam), int(1000 * brakeParam));

		// speed
		wsprintf(sBuffer_, TEXT("%ld"), int(1000 * speedParam));
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_SPEED), sBuffer_);

		// brake
		wsprintf(sBuffer_, TEXT("%ld"), int(1000 * brakeParam));
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_BRAKE), sBuffer_);

		// Axes
		wsprintf(sBuffer_, TEXT("%ld"), direction);
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_DIR), sBuffer_);

		// Device connected
		::SetWindowText(::GetDlgItem(m_hWnd, IDC_DEVICE), deviceConnected_);
	}
	CDialogEx::OnTimer(nIDEvent);
}
