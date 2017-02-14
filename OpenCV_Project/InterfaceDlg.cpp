// InterfaceDlg.cpp : ���� ����
//
#pragma comment(lib, "vfw32.lib")

#include "stdafx.h"
#include "afxdialogex.h"
#include "Interface.h"
#include "InterfaceDlg.h"

/* OpenCV */
#include "DetectionMng.h"

/* �� ��ǿ� ���� ���α׷� ���� ��ȭ */
enum eCommand
{
	Detection_START = 100,
	Detection_STOP = 200
	//Image_SAVE		= 300,
	//Program_EXIT		= 400
};

/* �������� ����� */
int g_eCommand = Detection_START;
bool g_bSelectFeature = false;
bool g_bOpenCam = false;
bool g_bSaveImage = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CInterfaceDlg ��ȭ ����



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


// CInterfaceDlg �޽��� ó����
// �ʱ�ȭ ó��
BOOL CInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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


// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.
void CInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/* ���� ��� */
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


/* Ÿ�̸� */
void CInterfaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	DetectionMng dMng;

	/* �̹��� ��Ʈ���� */
	Mat srcFrame;
	Mat dstFrame;
	Mat defaultFrame(320, 240, CV_8U, Scalar::all(0));

	if (g_bOpenCam)
	{
		/* ���� �̹����� ���´� */
		capture->read(srcFrame);
		dstFrame = srcFrame.clone();

		// ����� ����
		if (g_eCommand == Detection_START)
		{
			// ���� �����ϱ� �� �������� ó�� �۾�
			cvtColor(srcFrame, srcFrame, CV_RGB2GRAY);
			GaussianBlur(srcFrame, srcFrame, Size(7, 7), 0.0, 0.0);
			Canny(srcFrame, srcFrame, 30, 50);

			// Ư¡�� ����
			vector<Point2f> v_temp = dMng.DetectFeaturePoints(srcFrame);
			dMng.DrawCircleOnPoints(v_temp, dstFrame);

			// �簢�� ��� ���� ���� �׸���
			dMng.DrawMaskRect(srcFrame);
		}

		// ���� ���
		ShowImage(dstFrame);

		// �̹��� ����
		vector<int> compression_params;
		if (g_bSaveImage)
		{
			Mat resizedFrame;
			resize(dstFrame, resizedFrame, Size(960, 540));
			imwrite("Image.png", resizedFrame, compression_params);
			g_bSaveImage = false;
			MessageBox(
				L"�̹����� ����Ǿ����ϴ�.",
				L"Message");
		}
	}
	else
	{
		ShowImage(defaultFrame);
	}
	CDialogEx::OnTimer(nIDEvent);
}


/* ��ư �̺�Ʈ */
void CInterfaceDlg::OnBnClickedOn()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	capture = new VideoCapture(0);
	if (!capture->isOpened())
		AfxMessageBox(L"THREAD OPEN ERROR");

	g_bOpenCam = true;
}
void CInterfaceDlg::OnBnClickedOff()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	g_bOpenCam = false;

	if (capture->isOpened())
		capture->release();
}
void CInterfaceDlg::OnBnClickedExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ��ư�� Ŭ���Ͽ� ���α׷��� �����Ѵ�.
	if (MessageBox(
		L"���α׷��� �����Ͻðڽ��ϱ�?",
		L"Message", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		capture->release();
		SendMessage(WM_CLOSE, NULL, NULL);
	}
}
void CInterfaceDlg::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ��ư�� Ŭ���Ͽ� �̹����� �����Ѵ�.
	if (MessageBox(
		L"�̹����� �����Ͻðڽ��ϱ�?",
		L"Message", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		g_bSaveImage = true;
	}
}


/* �� ��� ó���� */
int CInterfaceDlg::GetCommand()
{
	/* �� ������κ��� Ŀ�ǵ带 ���´�  */
	int result = 0;
	// �� ��� ��ü ����
	// result = ���޹��� ��
	return result;
}
void CInterfaceDlg::SetCommand(int command)
{
	/* �� ������κ��� ���� Ŀ�ǵ带 g_eCommand�� �����Ѵ�. */
	g_eCommand = command;
}


/* �� ��� ������ */
UINT CInterfaceDlg::ThreadFunc(LPVOID param)
{
	CInterfaceDlg *infd = (CInterfaceDlg*)param;
	CInterfaceDlg* pDlg = (CInterfaceDlg*)AfxGetApp()->m_pMainWnd;
	
	while (1) 
	{
		Sleep(10000);
		AfxMessageBox(L"������ ���� �� ... ...");
	}
	return 0;
}