
// Interface.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CInterfaceApp:
// �� Ŭ������ ������ ���ؼ��� Interface.cpp�� �����Ͻʽÿ�.
//

class CInterfaceApp : public CWinApp
{
public:
	CInterfaceApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CInterfaceApp theApp;