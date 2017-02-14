// InterfaceDlg.h : ��� ����
//
#pragma once

#include "afxwin.h"
#include <afxwin.h>
#include "resource.h"
#include "opencv.hpp"

using namespace cv;

// CInterfaceDlg ��ȭ ����
class CInterfaceDlg : public CDialogEx
{
	// �����Դϴ�.
public:
	CInterfaceDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERFACE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

public:
	CImage *cimage_mfc;
	VideoCapture *capture; // �̹��� ������
	static UINT ThreadFunc(LPVOID param); // ������
	CWinThread *hThread = NULL; // ������ ������

	// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
