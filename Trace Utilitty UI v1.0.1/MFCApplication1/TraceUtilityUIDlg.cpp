#include "stdafx.h"
#include "TraceUtilityUI.h"
#include "TraceUtilityUIDlg.h"
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
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SIZE()
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
	for (auto i = 0; i < 8; i++)
		m_ComboBox.DeleteString(i);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	InsertColumns();

	int nItem;

	CT2CA pszConvertedAnsiString(path);
	std::string s_path(pszConvertedAnsiString);
	
	this->m_file.OpenFile(s_path);
	this->m_current_item = 0;
	std::vector<std::string> vec = this->m_file.ReadFromFile();
	this->m_file.CloseFile();

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
		int x = (rect.Width() - cxIcon + 100) / 2;
		int y = (rect.Height() - cyIcon + 100) / 2;

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
		CString strPathname = fileDlg.GetPathName();
		this->m_file_path = strPathname;
		OnInitDialog(strPathname);
	}
}

void CMFCApplication1Dlg::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	SetWindowLong(this->m_hWnd, GWL_STYLE, GetWindowLong(this->m_hWnd, GWL_STYLE) | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;

	case (CDDS_ITEMPREPAINT | CDDS_SUBITEM):
	{
		if (0 == pLVCD->iSubItem)
		{
			CString strText = m_List.GetItemText(pLVCD->nmcd.dwItemSpec, 0);

			if (strText == "[Fatal]")
				pLVCD->clrTextBk = RGB(255, 0, 0);
			else if (strText == "[Error]")
				pLVCD->clrTextBk = RGB(253, 106, 2);
			else if (strText == "[Warning]")
				pLVCD->clrTextBk = RGB(213, 253, 14);
			else
				pLVCD->clrTextBk = RGB(255, 255, 255);
		}
	}
	break;
	}
}

void CMFCApplication1Dlg::OnCbnSelchangeCombo1()
{
	CString str;
	m_ComboBox.GetLBText(m_ComboBox.GetCurSel(), str);

	CT2CA pszConvertedAnsiString(this->m_file_path);
	std::string s_path(pszConvertedAnsiString);

	this->m_file.OpenFile(s_path);
	std::vector<std::string> vec = this->m_file.ReadFromFile();
	this->m_file.CloseFile();

	if (str == "Debug")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k < vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Debug]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Error")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k<vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Error]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Fatal")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k < vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Fatal]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Unspecified")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k < vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Unspecified]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Warning")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k<vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Warning]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Trace")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k<vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Trace]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Info")
	{
		m_List.DeleteAllItems();

		int nItem;

		for (auto k = vec.begin(); k<vec.end(); k++)
		{
			auto v = *k;

			if (v == "[Info]")
			{
				CA2T temp(v.c_str());
				LPCTSTR lelem = temp;

				auto v2 = *(k + 1);
				CA2T temp2(v2.c_str());
				LPCTSTR lelem2 = temp2;

				auto v3 = *(k + 2);
				CA2T temp3(v3.c_str());
				LPCTSTR lelem3 = temp3;

				auto v4 = *(k + 3);
				CA2T temp4(v4.c_str());
				LPCTSTR lelem4 = temp4;

				int s = 1;

				nItem = m_List.InsertItem(0, lelem);
				m_List.SetItemText(nItem, s, lelem2);
				m_List.SetItemText(nItem, s + 1, lelem3);
				m_List.SetItemText(nItem, s + 2, lelem4);
			}
		}
	}
	if (str == "Cancel")
	{
		m_List.DeleteAllItems();

		int nItem;
		int i = 0;
		for (auto k = vec.begin(); k < vec.end(); k++)
		{
			auto v = *k;
			CA2T temp(v.c_str());
			LPCTSTR lelem = temp;

			int s = i % 4;

			if (!(i % 4))
				nItem = m_List.InsertItem(0, lelem);
			else
				m_List.SetItemText(nItem, s, lelem);

			i++;
		}
	}
	RedrawWindow();
}

void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CListCtrl* ct = NULL;
	CRect cr;
	GetClientRect(cr);
	ct = (CListCtrl*)GetDlgItem(IDC_LIST1);
	if (ct) ct->MoveWindow(0, 100, cr.right, cr.bottom);
}

void CMFCApplication1Dlg::InsertColumns()
{
	m_List.InsertColumn(0, L"Severity", LVCFMT_LEFT, 80);
	m_List.InsertColumn(1, L"Time", LVCFMT_CENTER, 150);
	m_List.InsertColumn(2, L"File", LVCFMT_LEFT, 530);
	m_List.InsertColumn(3, L"Text", LVCFMT_LEFT, 170);
}
