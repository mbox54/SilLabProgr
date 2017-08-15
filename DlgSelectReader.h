#pragma once
#include "afxwin.h"
#include "SLABCP2112.h"

#define VID 0
#define PID 0

// CDlgSelectReader dialog

class CDlgSelectReader : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSelectReader)

public:
	// standard constructor
	CDlgSelectReader(CWnd* pParent = NULL);   
	CDlgSelectReader(HID_SMBUS_DEVICE * p_hidSmbus, BYTE * p_ucExitState, CWnd* pParent = NULL);


	virtual ~CDlgSelectReader();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_READER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// > Properties
	HID_SMBUS_DEVICE * p_hidSmbus;

	BOOL m_bConnect;

	// > Methods
	// Universal Reader
	void UpdateDeviceList();
	BOOL GetSelectedDevice(CString& serial);
	BOOL FindDevice(CString serial, DWORD& deviceNum);

	BOOL Connect();
	BOOL Disconnect();


	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_comboDeviceList;

	// sign to parent to do exit
	BYTE * p_ucExitState;	

	afx_msg void OnCbnCloseupComboReaderList();
	afx_msg void OnCbnDropdownComboReaderList();
	afx_msg void OnCbnSelchangeComboReaderList();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
