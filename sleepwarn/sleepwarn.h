
// sleepwarn.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CsleepwarnApp:
// �� Ŭ������ ������ ���ؼ��� sleepwarn.cpp�� �����Ͻʽÿ�.
//

class CsleepwarnApp : public CWinApp
{
public:
	CsleepwarnApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CsleepwarnApp theApp;