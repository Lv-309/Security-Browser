#pragma once
#include "afxcmn.h"
#include "File.h"

class CMFCApplication1Dlg : public CDialogEx
{
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);

	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog(CString);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_List;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void CMFCApplication1Dlg::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult);

private:
	File file;

	int current_item = 0;
public:
	afx_msg void OnCbnSelchangeCombo1();
};
