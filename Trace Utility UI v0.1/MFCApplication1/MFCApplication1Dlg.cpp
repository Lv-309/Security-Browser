#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CMFCApplication1Dlg::OnCustomdrawMyList)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMFCApplication1Dlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

BOOL CMFCApplication1Dlg::OnInitDialog(CString path)
{
	while (m_List.DeleteColumn(0));
	m_List.DeleteAllItems();
	
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_List.InsertColumn(0, L"Severity", LVCFMT_LEFT, 80);
	m_List.InsertColumn(1, L"Time", LVCFMT_CENTER, 150);
	m_List.InsertColumn(2, L"File", LVCFMT_LEFT, 530);
	m_List.InsertColumn(3, L"Text", LVCFMT_LEFT, 170);

	int nItem;

	CT2CA pszConvertedAnsiString(path);
	std::string s_path(pszConvertedAnsiString);
	
	file.OpenFile(s_path);
	current_item = 0;
	std::vector<std::string> vec = file.ReadFromFile();
	file.CloseFile();

	int i = 0;

	for (auto v : vec)
	{
		CA2T temp(v.c_str());
		LPCTSTR lelem = temp;

		int s = i % 4;

		if (!(i % 4))
			nItem = m_List.InsertItem(0, lelem);
		else
			m_List.SetItemText(nItem, s, lelem);

		i++;
	}

	return TRUE;
}

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;
}

void CMFCApplication1Dlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	char szFilters[] = "Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";

	CFileDialog fileDlg(TRUE, _T("log"), _T("*.log"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, CA2W(szFilters), this);

	if (fileDlg.DoModal() == IDOK)
	{
		CString m_strPathname = fileDlg.GetPathName();

		OnInitDialog(m_strPathname);
	}
}

void CMFCApplication1Dlg::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;
	
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		CString strText;
		COLORREF crBkgnd;

		strText = m_List.GetItemText(current_item++, 0);

		if (current_item == m_List.GetItemCount())
			current_item = 0;

		if (strText == "[Fatal]")
			crBkgnd = RGB(255, 0, 0);
		else if (strText == "[Error]")
			crBkgnd = RGB(253, 106, 2);
		else if (strText == "[Warning]")
			crBkgnd = RGB(213, 253, 14);
		else
			crBkgnd = RGB(255, 255, 255);

		pLVCD->clrTextBk = crBkgnd;

		*pResult = CDRF_DODEFAULT;
	}
}


void CMFCApplication1Dlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}
