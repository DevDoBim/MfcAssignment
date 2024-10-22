
// MFC_ProjectDlg.h : ��� ����
//

#pragma once
#include <vector>

// CMFC_ProjectDlg ��ȭ ����
class CMFC_ProjectDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMFC_ProjectDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_PROJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


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
	afx_msg void OnBnClickedBtnDraw();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedBtnOpen();

private:
	CImage m_image;
	CString m_strPath;
	CString m_strPathPoints;
	CBitmap m_bitmap;
	unsigned char* m_fm;
	std::vector<CPoint> m_points;

	int m_nStartX;
	int m_nStartY;
	int m_nEndX;
	int m_nEndY;
	int m_nRadius;
	int m_color;

	bool m_bLoad;

private:
	void init();
	void createImage();
	void updateDisplay();
	void saveDlg(int nCenterX, int nCenterY);
	void setDirect();

private:
	void drawCircle(unsigned char* fm, int nCenterX, int nCenterY, int nRadius, int nColor);
	bool inCircle(int x, int y, int CenterX, int CenterY, int radius);
	void moveCircle();
	int  lerpInt(int begin, int end, float alpha);
	
};
