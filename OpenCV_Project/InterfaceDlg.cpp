// InterfaceDlg.cpp : 구현 파일
//
#pragma comment(lib, "vfw32.lib")

#include "stdafx.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "InterfaceDlg.h"

/* OpenCV */
#include "DetectionMng.h"

/* 손 모션에 따라 프로그램 동작 변화 */
enum eCommand
{
	Detection_START = 100,
	Detection_STOP = 200
	//Image_SAVE		= 300,
	//Program_EXIT		= 400
};

/* 전역변수 선언부 */
int g_eCommand = Detection_START;
bool g_bSelectFeature = false;
bool g_bOpenCam = false;
bool g_bSaveImage = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CInterfaceDlg 대화 상자



CInterfaceDlg::CInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INTERFACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PIC_FRAME, hPIC_FRAME);
}

BEGIN_MESSAGE_MAP(CInterfaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(BTN_ON, &CInterfaceDlg::OnBnClickedOn)
	ON_BN_CLICKED(BTN_OFF, &CInterfaceDlg::OnBnClickedOff)
	ON_BN_CLICKED(BTN_EXIT, &CInterfaceDlg::OnBnClickedExit)
	ON_BN_CLICKED(BTN_SAVE, &CInterfaceDlg::OnBnClickedSave)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CInterfaceDlg 메시지 처리기
// 초기화 처리
BOOL CInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	/* Open WebCam */
	capture = new VideoCapture(0);

	/* WebCam Size 320x240 */
	capture->set(CAP_PROP_FRAME_WIDTH, 320);
	capture->set(CAP_PROP_FRAME_HEIGHT, 240);
	cimage_mfc = NULL;

	/* Timer Setting */
	SetTimer(1000, 30, NULL);

	/* Thread */
	hThread = AfxBeginThread(ThreadFunc, this);
	if (hThread == NULL)
		AfxMessageBox(L"THREAD OPEN ERROR");

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


// 
void CInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.
void CInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/* 영상 출력 */
void CInterfaceDlg::ShowImage(Mat frame)
{
	int bpp = 8 * frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (frame.cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = frame;
	}

	RECT r;
	hPIC_FRAME.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	if (cimage_mfc)
	{
		cimage_mfc->ReleaseDC();
		delete cimage_mfc;
		cimage_mfc = nullptr;
	}

	cimage_mfc = new CImage();
	cimage_mfc->Create(winSize.width, winSize.height, 24);

	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = bpp;
	bitInfo.bmiHeader.biWidth = mat_temp.cols;
	bitInfo.bmiHeader.biHeight = -mat_temp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	// Image is bigger or smaller than into destination rectangle
	// we use stretch in full rect

	if (mat_temp.cols == winSize.width  && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc->GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, &bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc->GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	cimage_mfc->Draw(::GetDC(hPIC_FRAME.m_hWnd), 0, 0, cimage_mfc->GetWidth(), cimage_mfc->GetHeight());

	if (cimage_mfc)
	{
		cimage_mfc->ReleaseDC();
		delete cimage_mfc;
		cimage_mfc = nullptr;
	}
}


/* 타이머 */
void CInterfaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DetectionMng dMng;

	/* 이미지 매트릭스 */
	Mat srcFrame;
	Mat dstFrame;
	Mat defaultFrame(320, 240, CV_8U, Scalar::all(0));

	if (g_bOpenCam)
	{
		/* 영상 이미지를 얻어온다 */
		capture->read(srcFrame);
		dstFrame = srcFrame.clone();

		// 검출기 실행
		if (g_eCommand == Detection_START)
		{
			// 검출 시작하기 전 여러가지 처리 작업
			cvtColor(srcFrame, srcFrame, CV_RGB2GRAY);
			GaussianBlur(srcFrame, srcFrame, Size(7, 7), 0.0, 0.0);
			Canny(srcFrame, srcFrame, 30, 50);

			// 특징점 검출
			vector<Point2f> v_temp = dMng.DetectFeaturePoints(srcFrame);
			dMng.DrawCircleOnPoints(v_temp, dstFrame);

			// 사각형 모양 검출 범위 그리기
			dMng.DrawMaskRect(srcFrame);
		}

		// 영상 출력
		ShowImage(dstFrame);

		// 이미지 저장
		vector<int> compression_params;
		if (g_bSaveImage)
		{
			Mat resizedFrame;
			resize(dstFrame, resizedFrame, Size(960, 540));
			imwrite("Image.png", resizedFrame, compression_params);
			g_bSaveImage = false;
			MessageBox(
				L"이미지가 저장되었습니다.",
				L"Message");
		}
	}
	else
	{
		ShowImage(defaultFrame);
	}
	CDialogEx::OnTimer(nIDEvent);
}


/* 버튼 이벤트 */
void CInterfaceDlg::OnBnClickedOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	capture = new VideoCapture(0);
	if (!capture->isOpened())
		AfxMessageBox(L"THREAD OPEN ERROR");

	g_bOpenCam = true;
}
void CInterfaceDlg::OnBnClickedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_bOpenCam = false;

	if (capture->isOpened())
		capture->release();
}
void CInterfaceDlg::OnBnClickedExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 버튼을 클릭하여 프로그램을 종료한다.
	if (MessageBox(
		L"프로그램을 종료하시겠습니까?",
		L"Message", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		capture->release();
		SendMessage(WM_CLOSE, NULL, NULL);
	}
}
void CInterfaceDlg::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 버튼을 클릭하여 이미지를 저장한다.
	if (MessageBox(
		L"이미지를 저장하시겠습니까?",
		L"Message", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		g_bSaveImage = true;
	}
}


/* 손 모션 처리기 */
int CInterfaceDlg::GetCommand()
{
	/* 손 모션으로부터 커맨드를 얻어온다  */
	int result = 0;
	// 손 모션 객체 생성
	// result = 전달받은 값
	return result;
}
void CInterfaceDlg::SetCommand(int command)
{
	/* 손 모션으로부터 얻은 커맨드를 g_eCommand에 대입한다. */
	g_eCommand = command;
}


/* 손 모션 쓰레드 */
UINT CInterfaceDlg::ThreadFunc(LPVOID param)
{
	CInterfaceDlg *infd = (CInterfaceDlg*)param;
	CInterfaceDlg* pDlg = (CInterfaceDlg*)AfxGetApp()->m_pMainWnd;
	
	while (1) 
	{
		Sleep(10000);
		AfxMessageBox(L"쓰레드 실행 중 ... ...");
	}
	return 0;
}