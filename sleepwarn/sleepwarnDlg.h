
// sleepwarnDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "CvvImage.h"


// CsleepwarnDlg 대화 상자
class CsleepwarnDlg : public CDialogEx
{
// 생성입니다.
public:
	CsleepwarnDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLEEPWARN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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

	CStatic m_eyesPic;
	IplImage* m_EyesImage;
	CvvImage m_cEyesImage;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSoundStop();
	afx_msg void OnBnClickedSoundSelect();
	CEdit sound_name;
	afx_msg void OnBnClickedSoundPlay();	
	CEdit m_alarm;
	CEdit m_normal;	
};
