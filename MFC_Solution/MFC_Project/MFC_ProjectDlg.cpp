
// MFC_ProjectDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MFC_Project.h"
#include "MFC_ProjectDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include<fstream>
#include <cstdlib>
#include <ctime>
#include <afxdlgs.h>

using std::cout;
using std::endl;
using std::srand;
using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStratup /subsystem:console")
#endif

constexpr int g_nWidth = 500;
constexpr int g_nHeight = 400;
constexpr int g_nBpixel = 8;

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


// CMFC_ProjectDlg 대화 상자



CMFC_ProjectDlg::CMFC_ProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_PROJECT_DIALOG, pParent)
	, m_image{}, m_fm(nullptr)
	, m_nStartX(0), m_nStartY(0)
	, m_nEndX(0), m_nEndY(0)
	, m_nRadius(0), m_color(0)
	, m_strPath(""), m_bitmap{}
	,m_points{}, m_bLoad(false)
	, m_strPathPoints{}
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_ProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_START_X, m_nStartX);
	DDV_MinMaxInt(pDX, m_nStartX, 100, 300);
	DDX_Text(pDX, IDC_INPUT_START_Y, m_nStartY);
	DDV_MinMaxInt(pDX, m_nStartY, 100, 300);
	DDX_Text(pDX, IDC_INPUT_END_X, m_nEndX);
	DDV_MinMaxInt(pDX, m_nEndX, 100, 300);
	DDX_Text(pDX, IDC_INPUT_END_Y, m_nEndY);
	DDV_MinMaxInt(pDX, m_nEndY, 100, 300);
}

BEGIN_MESSAGE_MAP(CMFC_ProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DRAW, &CMFC_ProjectDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDOK, &CMFC_ProjectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CMFC_ProjectDlg::OnBnClickedBtnAction)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMFC_ProjectDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CMFC_ProjectDlg 메시지 처리기

BOOL CMFC_ProjectDlg::OnInitDialog()
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
	init();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFC_ProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFC_ProjectDlg::OnPaint()
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
HCURSOR CMFC_ProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFC_ProjectDlg::init()
{
	createImage();
	setDirect();
	memset(m_fm, 255, (g_nWidth * g_nHeight));
}

void CMFC_ProjectDlg::createImage()
{
	if (m_image)
	{
		m_image.Destroy();
	}

	if (m_image == nullptr)
	{
		m_image.Create(g_nWidth, -g_nHeight, g_nBpixel);
		m_fm = static_cast<unsigned char*>(m_image.GetBits());

		if (g_nBpixel == 8)
		{
			static RGBQUAD rgb[256];
			for (size_t i = 0; i < 256; i++)
			{
				rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
				m_image.SetColorTable(0, 256, rgb);
			}
		}

	}
}
void CMFC_ProjectDlg::updateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);

	if (m_bLoad)
	{
		std::ifstream inPos(m_strPathPoints, std::ios::binary);

		int posX;
		int posY;

		if (inPos.is_open())
		{
			inPos.read((char*)(&posX), sizeof(posX));
			inPos.read((char*)(&posY), sizeof(posY));
		}

		CString strText;
		strText.Format(_T("x   (%d,%d)"), posX, posY);
		 
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(100, 100, 100)); // 텍스트 색상 (검정색)
		dc.TextOut(posX - 4, posY - 6, strText);
		//dc.SetBkMode(OPAQUE);
	}
}

void CMFC_ProjectDlg::OnBnClickedBtnDraw()
{
	memset(m_fm, 255, (g_nWidth * g_nHeight));
	UpdateData(TRUE);

	int* randSeed = new int;
	srand((unsigned int)(randSeed));

	m_nRadius = rand() % 50 + 10;
	m_color = 0;

	// startX, startY는 원의 중심 좌표
	drawCircle(m_fm, m_nStartX, m_nStartY, m_nRadius, m_color);

	if (randSeed != nullptr)
	{
		delete randSeed;
	}

	updateDisplay();
}
void CMFC_ProjectDlg::drawCircle(unsigned char * fm, int nCenterX, int nCenterY, int nRadius, int nColor)
{
	int x = nCenterX - nRadius;
	int y = nCenterY - nRadius;
	int nPitch = m_image.GetPitch();

	for (size_t i = y; i < y + nRadius * 2; i++)
	{
		for (size_t j = x; j < x + nRadius * 2; j++)
		{
			if (inCircle(j, i, nCenterX, nCenterY, nRadius))
			{
				fm[i * nPitch + j] = nColor;
			}
		}
	}
}
bool CMFC_ProjectDlg::inCircle(int x, int y, int CenterX, int CenterY, int radius)
{
	double dX = x - CenterX;
	double dY = y - CenterY;
	double dDist = dX * dX + dY * dY;

	return dDist <= radius * radius;
}

void CMFC_ProjectDlg::OnBnClickedBtnAction()
{
	UpdateData(TRUE);
	moveCircle();
}

void CMFC_ProjectDlg::moveCircle()
{
	static int nX = m_nStartX;
	static int nY = m_nStartY;
	
	static float alpha = 0.0f;
	bool bFlag = false;

	while (bFlag == false)
	{
		memset(m_fm, 255, (g_nWidth * g_nHeight));

		if (alpha >= 1)
		{
			bFlag = true;
			alpha = 1.0f;
		}
		
		nX = lerpInt(m_nStartX, m_nEndX, alpha);
		nY = lerpInt(m_nStartY, m_nEndY, alpha);

		drawCircle(m_fm, nX, nY, m_nRadius, m_color);
		updateDisplay();

		saveDlg(nX, nY);

		alpha += 1.0f / 5.0f;
		Sleep(1100);
	}

	alpha = 0.0f;

	nX = m_nStartX;
	nY = m_nStartY;
}

int CMFC_ProjectDlg::lerpInt(int begin, int end, float alpha)
{
	return (end - begin) * alpha + begin;
}

void CMFC_ProjectDlg::saveDlg(int nCenterX, int nCenterY)
{
	time_t timer = time(NULL);
	tm calTime;

	// 받아온 현재 시스템 시간을 변환 (localtime_s 사용)
	localtime_s(&calTime, &timer);

	// CString에 시간을 저장
	CString time_current;

	time_current.Format
	(
		_T("%02d_%02d_%02d"),
		calTime.tm_hour,         
		calTime.tm_min,          
		calTime.tm_sec
	);       

	CString extension = _T(".bmp");
	CString extension_txt = _T(".txt");

	CString FullPath = m_strPath + time_current + extension;
	CString FullPath_Points = m_strPath + time_current + extension_txt;

	m_image.Save(FullPath);

	// 중심 좌표를 바이너리 파일로 저장
	std::ofstream outFile(CT2A(FullPath_Points), std::ios::binary);
	if (outFile.is_open())
	{
		// x, y 값을 파일에 저장
		outFile.write((char*)(&nCenterX), sizeof(nCenterX));
		outFile.write((char*)(&nCenterY), sizeof(nCenterY));

		outFile.close(); 
	}
}

void CMFC_ProjectDlg::setDirect()
{
	CString currentDir;
	GetCurrentDirectory(MAX_PATH, currentDir.GetBuffer(MAX_PATH));
	currentDir.ReleaseBuffer();

	CString relativePath = _T("\\res\\save\\");

	m_strPath = currentDir + relativePath;
}

void CMFC_ProjectDlg::OnBnClickedBtnOpen()
{
	CFileDialog fileDlg(TRUE); // TRUE: 파일 열기 대화 상자, FALSE: 파일 저장 대화 상자

	// 대화 상자 제목 설정
	fileDlg.m_ofn.lpstrTitle = _T("파일 열기");

	// 초기 경로 설정
	fileDlg.m_ofn.lpstrInitialDir = m_strPath;

	// 파일 필터 설정
	fileDlg.m_ofn.lpstrFilter = _T("BMP (*.bmp)|*.bmp|");

	// 대화 상자 실행
	if (fileDlg.DoModal() == IDOK)
	{
		// 사용자가 파일을 선택한 경우
		CString selectedFilePath = fileDlg.GetPathName(); // 선택된 파일의 경로
		CString pointsFilePath = selectedFilePath.Left(selectedFilePath.GetLength() - 3);

		CString extenstion = _T("txt");
		pointsFilePath += extenstion;
		m_strPathPoints = pointsFilePath;

		//std::wcout << pointsFilePath.GetString();
		 // 선택된 파일 경로 출력
		//AfxMessageBox(selectedFilePath);

		if (m_image != nullptr)
		{
			m_image.Destroy();
		}

		m_image.Load(selectedFilePath);
		
		m_bLoad = true;

		updateDisplay();
		createImage();

		m_bLoad = false;

	}
	else {
		AfxMessageBox(_T("파일 선택이 취소되었습니다."));
	}
}

void CMFC_ProjectDlg::OnBnClickedOk()
{
	CDialogEx::OnCancel();
}