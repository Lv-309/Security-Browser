#pragma once
#include "afxcmn.h"
#include "File.h"
#include "afxwin.h"

class CMFCApplication1Dlg : public CDialogEx
{
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);

	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

	CListCtrl m_List;
	CComboBox m_ComboBox;

	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void CMFCApplication1Dlg::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void CMFCApplication1Dlg::OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo1();

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog(CString);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	File file;
	CString m_file_path;
	int current_item = 0;

	void InsertColumns();
};
