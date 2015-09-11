
// sleepwarnDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "sleepwarn.h"
#include "sleepwarnDlg.h"
#include "afxdialogex.h"

#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>

#include "constants.h"
#include "findEyeCenter.h"
#include "findEyeCorner.h"
using namespace std;
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Mat detect(Mat frame);
String face_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "C:\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml";
//String eyes_cascade_name = "C:\\Users\\Administrator\\Desktop\\CascadeTrainer\\test_recognition\\cascade.xml";
String close_cascade_name = "";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier close_cascade;

CString strPathName = NULL;
int alarm_cnt = 0;
int normal_cnt = 0;
string face_window_name = "Capture - Face";
string main_window_name = "Capture - Face detection";
RNG rng(12345);
Mat debugImage;
Mat skinCrCbHist = Mat::zeros(Size(256, 256), CV_8UC1);
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


// CsleepwarnDlg ��ȭ ����



CsleepwarnDlg::CsleepwarnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLEEPWARN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsleepwarnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM, m_ctrlPic);
	DDX_Control(pDX, IDC_EDIT1, sound_name);
	DDX_Control(pDX, IDC_ALARM_CNT, m_alarm);
	DDX_Control(pDX, IDC_NORMAL_CNT, m_normal);
}

BEGIN_MESSAGE_MAP(CsleepwarnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_BN_CLICKED(IDC_CAM_START, &CsleepwarnDlg::OnBnClickedCamStart)
	ON_BN_CLICKED(IDC_CAM_STOP, &CsleepwarnDlg::OnBnClickedCamStop)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SOUND_STOP, &CsleepwarnDlg::OnBnClickedSoundStop)
	ON_BN_CLICKED(IDC_SOUND_SELECT, &CsleepwarnDlg::OnBnClickedSoundSelect)
	ON_BN_CLICKED(IDC_SOUND_PLAY, &CsleepwarnDlg::OnBnClickedSoundPlay)
END_MESSAGE_MAP()


// CsleepwarnDlg �޽��� ó����

BOOL CsleepwarnDlg::OnInitDialog()
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
	/*
	m_capture = cvCreateCameraCapture(0);
	if (!m_capture)
		AfxMessageBox(_T("ī�޶� �����ϴ�."));
	*/
	m_capture = cvCreateFileCapture("C:\\Users\\Administrator\\Documents\\test.mp4");

	if (!face_cascade.load(face_cascade_name)) AfxMessageBox(_T("Error loading face"));
	if (!eyes_cascade.load(eyes_cascade_name)) AfxMessageBox(_T("Error loading eyes"));
	
	namedWindow(face_window_name, CV_WINDOW_NORMAL);
	moveWindow(face_window_name, 10, 100);
	namedWindow(main_window_name, CV_WINDOW_NORMAL);
	moveWindow(main_window_name, 400, 100);
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CsleepwarnDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsleepwarnDlg::OnPaint()
{	
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	CDC *pDC;
	CRect rect;
	pDC = m_ctrlPic.GetDC();
	m_ctrlPic.GetClientRect(rect);

	m_cImage.CopyOf(m_Image);
	m_cImage.DrawToHDC(pDC->m_hDC, rect);

	ReleaseDC(pDC);
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CsleepwarnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsleepwarnDlg::OnBnClickedCamStart()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (strPathName == ""){
		AfxMessageBox(_T("�˶� ��ΰ� �������� �ʾҽ��ϴ�."));
	}
	else{
		SetTimer(1, 30, NULL);
	}	
}


void CsleepwarnDlg::OnBnClickedCamStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}


void CsleepwarnDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	Mat frame;	

	frame = cvQueryFrame(m_capture);
	flip(frame, frame, 1);
	frame.copyTo(debugImage);
	
	m_Image = new IplImage(detect(frame));

	imshow(main_window_name, debugImage);

	CString str;
	str.Format(L"%d", alarm_cnt);
	m_alarm.SetWindowTextW(str);
	CString str1;
	str1.Format(L"%d", normal_cnt);
	m_normal.SetWindowTextW(str1);
	Invalidate(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}


void CsleepwarnDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);

	if (m_capture)
		cvReleaseCapture(&m_capture);
}


Mat findSkin(Mat &frame) {
	Mat input;
	Mat output = Mat(frame.rows, frame.cols, CV_8U);

	cvtColor(frame, input, CV_BGR2YCrCb);

	for (int y = 0; y < input.rows; ++y) {
		const Vec3b *Mr = input.ptr<Vec3b>(y);
		Vec3b *Or = frame.ptr<Vec3b>(y);
		for (int x = 0; x < input.cols; ++x) {
			Vec3b ycrcb = Mr[x];
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0) {
				Or[x] = Vec3b(0, 0, 0);
			}
		}
	}
	return output;
}


void findEyes(Mat frame_gray, Rect face) {
	Mat faceROI = frame_gray(face);
	Mat debugFace = faceROI;

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, Size(0, 0), sigma);
	}

	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	Rect leftEyeRegion(face.width*(kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);
	Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0), eye_region_top, eye_region_width, eye_region_height);


	Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");

	Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	rectangle(debugFace, leftRightCornerRegion, 200);
	rectangle(debugFace, leftLeftCornerRegion, 200);
	rectangle(debugFace, rightLeftCornerRegion, 200);
	rectangle(debugFace, rightRightCornerRegion, 200);
	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers
	circle(debugFace, rightPupil, 3, 1234);
	circle(debugFace, leftPupil, 3, 1234);

	if (leftPupil.y > leftEyeRegion.y && leftPupil.y < leftEyeRegion.y + 5 || rightPupil.y > rightEyeRegion.y && rightPupil.y < rightEyeRegion.y +5 ){
		alarm_cnt++;
	}
	else{
		normal_cnt++;
	}
	if (normal_cnt > 100){
		normal_cnt = 0;
		alarm_cnt = 0;
	}
	
	if (kEnableEyeCorner) {		
		Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;
		Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		rightLeftCorner.x += rightLeftCornerRegion.x;
		rightLeftCorner.y += rightLeftCornerRegion.y;
		Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;
		circle(faceROI, leftRightCorner, 3, 200);
		circle(faceROI, leftLeftCorner, 3, 200);
		circle(faceROI, rightLeftCorner, 3, 200);
		circle(faceROI, rightRightCorner, 3, 200);
	}

	imshow(face_window_name, faceROI);
}

Mat detect(Mat input_frame) {
	vector<Rect> faces;	
	Mat frame=input_frame;

	std::vector<Mat> rgbChannels(3);
	split(frame, rgbChannels);
	Mat frame_gray = rgbChannels[2];

	if (alarm_cnt > 100){
		alarm_cnt = 0;
		PlaySound(strPathName, AfxGetInstanceHandle(), SND_ASYNC);
	}

	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, Size(150, 150));	
	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(debugImage, faces[i], 1234);
	}

	if (faces.size() > 0) {
		findEyes(frame_gray, faces[0]);
	}
	else if (faces.size() == 0){
		normal_cnt = 0;
		alarm_cnt++;
	}
	return frame;
}

void CsleepwarnDlg::OnBnClickedSoundStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	PlaySound(NULL, AfxGetInstanceHandle(), NULL);
	alarm_cnt = 0;
}


void CsleepwarnDlg::OnBnClickedSoundSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	TCHAR szFilter[] = _T("Sound (*.wav)|*.wav|*.*|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		strPathName = dlg.GetPathName();
		sound_name.SetWindowTextW(strPathName);		
	}
}


void CsleepwarnDlg::OnBnClickedSoundPlay()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (strPathName == ""){
		AfxMessageBox(_T("�˶� ��ΰ� �������� �ʾҽ��ϴ�."));
	}
	else{
		PlaySound(strPathName, AfxGetInstanceHandle(), SND_ASYNC);
	}	
}