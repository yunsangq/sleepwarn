
// sleepwarnDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "CvvImage.h"


// CsleepwarnDlg ��ȭ ����
class CsleepwarnDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CsleepwarnDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLEEPWARN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);	
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnBnClickedCamStart();
	afx_msg void OnBnClickedCamStop();
	afx_msg void OnPaint();
	CStatic m_ctrlPic;
	IplImage* m_Image;
	CvvImage m_cImage;
	CvCapture* m_capture;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSoundStop();
	afx_msg void OnBnClickedSoundSelect();
};
