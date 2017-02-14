// InterfaceDlg.h : 헤더 파일
//
#pragma once

#include "afxwin.h"
#include <afxwin.h>
#include "resource.h"
#include "opencv.hpp"

using namespace cv;

// CInterfaceDlg 대화 상자
class CInterfaceDlg : public CDialogEx
{
	// 생성입니다.
public:
	CInterfaceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERFACE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	CImage *cimage_mfc;
	VideoCapture *capture; // 이미지 포인터
	static UINT ThreadFunc(LPVOID param); // 쓰레드
	CWinThread *hThread = NULL; // 쓰레드 포인터

	// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic hPIC_FRAME;
	afx_msg void OnBnClickedOn();
	afx_msg void OnBnClickedOff();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedSave();
	afx_msg void ShowImage(Mat frame);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int GetCommand();
	void SetCommand(int command);
};
