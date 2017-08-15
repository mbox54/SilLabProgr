// MCUProgrammerDlg.h : header file
//

#if !defined(AFX_MCUPROGRAMMERDLG_H__5A2DBFBF_9DA9_4046_A818_F44A2CEB231A__INCLUDED_)
#define AFX_MCUPROGRAMMERDLG_H__5A2DBFBF_9DA9_4046_A818_F44A2CEB231A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LOG_RIGHT_OFFSET		27
#define LOG_BOTTOM_OFFSET		25

#define STATIC_RIGHT_OFFSET		13
#define STATIC_BOTTOM_OFFSET	15

#define DISABLE_DIALOGS			1
#define TIMER_PLUGCHECK			0x01

#include "DlgSelectReader.h"
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerDlg dialog

class CMCUProgrammerDlg : public CDialog
{
// Construction
public:
	CMCUProgrammerDlg(CString filename, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMCUProgrammerDlg)
	enum { IDD = IDD_MCUPROGRAMMER_DIALOG };
	CRichEditCtrl	m_Log;
	BOOL	m_EraseCodeSpace;
	BOOL	m_FlashPersist;
	BOOL	m_LockCodeSpace;
//	BOOL	m_SerializeParts;
//	BOOL	m_UnicodeFormat;
	CString	m_CheckSum;
	CString	m_DebugAdapter;
	CString	m_HexFileNotBanked;
//	CString	m_HexFileBank1;
//	CString	m_HexFileBank2;
//	CString	m_HexFileBank3;
	CString	m_PartNumber;
//	CString	m_CurrentSerialNumberString;
//	CString	m_SerialNumberIncrement;
//	BOOL	m_bBigEndianFormat;
	CString m_fileName;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCUProgrammerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	PROGRAM_SETTINGS m_ProgramSettings;
	BOOL m_SettingsHaveBeenProgrammed;
	UINT m_CurrentSerialNumber;

	// Timer
	UINT_PTR m_nTimer;						// timer controler

	// Universal Reader
	HID_SMBUS_DEVICE m_hidSmbus;

	BYTE m_exitState;						// exit on selecting
	BYTE m_gpio_latch;						// plugged devices
	BYTE m_LED_GREEN_STATE;
	BYTE m_LED_RED_STATE;

	CBrush br_Led_Green_Light;
	CBrush br_Led_Green_Dark;
	CBrush br_Led_Red_Light;
	CBrush br_Led_Red_Dark;

	// > Methods
	// Service
	void UpdateWindowProgramSettings();
	void SetCurrentApplicationDirectory();

	void PromptLoadFile();
	void PromptSaveFile();
	BOOL ValidateSettings(PROGRAM_SETTINGS settings);	

	// Timer
	void StartTimer();
	void EditTimer();
	void StopTimer();

	// Universal Reader
	void CheckUniversalReader();
	void LatchController();


	// Generated message map functions
	//{{AFX_MSG(CMCUProgrammerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAboutbox();
	afx_msg void OnProgrammenuLoadsettings();
	afx_msg void OnProgrammenuSavesettings();
	afx_msg void OnProgrammenuConfigureprogramminginformation();
	afx_msg void OnProgrammenuExit();
	afx_msg void OnButtonProgramdevice();
	afx_msg void OnDestroy();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_Static_Logo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCUPROGRAMMERDLG_H__5A2DBFBF_9DA9_4046_A818_F44A2CEB231A__INCLUDED_)
