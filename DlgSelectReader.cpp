// DlgSelectReader.cpp : implementation file
//

#include "stdafx.h"
#include "MCUProgrammer.h"
#include "DlgSelectReader.h"
#include "afxdialogex.h"


// CDlgSelectReader dialog

IMPLEMENT_DYNAMIC(CDlgSelectReader, CDialogEx)

CDlgSelectReader::CDlgSelectReader(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SELECT_READER, pParent)
{

}

CDlgSelectReader::CDlgSelectReader(HID_SMBUS_DEVICE * p_hidSmbus, BYTE * p_ucExitState, CWnd * pParent)
	: CDialogEx(IDD_DIALOG_SELECT_READER, pParent)
	, p_hidSmbus(p_hidSmbus)
	, p_ucExitState(p_ucExitState)
{

}

CDlgSelectReader::~CDlgSelectReader()
{
}

void CDlgSelectReader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_READER_LIST, m_comboDeviceList);
}

// Universal Reader
void CDlgSelectReader::UpdateDeviceList()
{
	// Only update the combo list when the drop down list is closed
	if (!m_comboDeviceList.GetDroppedState())
	{
		int						sel;
		CString					serial;
		DWORD					numDevices;
		HID_SMBUS_DEVICE_STR	deviceString;

		// Get previous combobox string selection
		GetSelectedDevice(serial);

		// Remove all strings from the combobox
		m_comboDeviceList.ResetContent();

		// Get number of HID devices with matching VID/PID (0/0 means not filtered)
		if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
		{
			// Iterate through each HID device with matching VID/PID
			for (DWORD i = 0; i < numDevices; i++)
			{
				// Add serial strings to the combobox
				if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
				{
					m_comboDeviceList.AddString(CString(deviceString));
				}
			}
		}

		sel = m_comboDeviceList.FindStringExact(-1, serial);

		// Select first combobox string
		if (sel == CB_ERR)
		{
			m_comboDeviceList.SetCurSel(0);
		}
		// Restore previous combobox selection
		else
		{
			m_comboDeviceList.SetCurSel(sel);
		}
	}
}

BOOL CDlgSelectReader::GetSelectedDevice(CString & serial)
{
	BOOL selected = FALSE;

	int			sel;
	CString		selText;

	// Get current selection index or CB_ERR(-1)
	// if no device is selected
	sel = m_comboDeviceList.GetCurSel();

	if (sel != CB_ERR)
	{
		// Get the selected device string
		m_comboDeviceList.GetLBText(sel, selText);
		selected = TRUE;
		serial = selText;
	}

	return selected;
}

BOOL CDlgSelectReader::FindDevice(CString serial, DWORD & deviceNum)
{
	BOOL					found = FALSE;
	DWORD					numDevices;
	HID_SMBUS_DEVICE_STR	deviceString;

	if (HidSmbus_GetNumDevices(&numDevices, VID, PID) == HID_SMBUS_SUCCESS)
	{
		for (DWORD i = 0; i < numDevices; i++)
		{
			if (HidSmbus_GetString(i, VID, PID, deviceString, HID_SMBUS_GET_SERIAL_STR) == HID_SMBUS_SUCCESS)
			{
				if (serial.CompareNoCase(CString(deviceString)) == 0)
				{
					found = TRUE;
					deviceNum = i;
					break;
				}
			}
		}
	}

	return found;
}

BOOL CDlgSelectReader::Connect()
{
	BOOL		connected = FALSE;
	CString		serial;
	DWORD		deviceNum;

	// Get selected device serial string
	if (GetSelectedDevice(serial))
	{
		// Find the selected device number
		if (FindDevice(serial, deviceNum))
		{
			HID_SMBUS_STATUS status = HidSmbus_Open(p_hidSmbus, deviceNum, VID, PID);

			CString statusText;
			//			statusText.Format(_T("HidSmbus_Open(): %s"), HidSmbus_DecodeErrorStatus(status).GetString());
			//			SetStatusText(statusText);

			// Attempt to open the device
			if (status == HID_SMBUS_SUCCESS)
			{
				connected = TRUE;
			}
			else
			{
				// [FAILURE]

				CString msg;
				msg.Format(_T("Failed to connect to %s, Error code: %02d"), serial.GetString(), status);
				MessageBox(msg, 0, MB_ICONWARNING);
			}
		}
	}

	// > Set OP FLAG
	// Connected
	if (connected)
	{
		// set Connection FLAG
		this->m_bConnect = 1;

		// Disable the device combobox
		m_comboDeviceList.EnableWindow(FALSE);
	}
	// Disconnected
	else
	{
		// set Connection FLAG
		this->m_bConnect = 0;

		// Enable the device combobox
		m_comboDeviceList.EnableWindow(TRUE);
	}


	return connected;
}


BOOL CDlgSelectReader::Disconnect()
{
	return 0;
}


BEGIN_MESSAGE_MAP(CDlgSelectReader, CDialogEx)
	ON_CBN_CLOSEUP(IDC_COMBO_READER_LIST, &CDlgSelectReader::OnCbnCloseupComboReaderList)
	ON_CBN_DROPDOWN(IDC_COMBO_READER_LIST, &CDlgSelectReader::OnCbnDropdownComboReaderList)
	ON_CBN_SELCHANGE(IDC_COMBO_READER_LIST, &CDlgSelectReader::OnCbnSelchangeComboReaderList)
	ON_BN_CLICKED(IDCANCEL, &CDlgSelectReader::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSelectReader message handlers


void CDlgSelectReader::OnCbnCloseupComboReaderList()
{
	CString		serial;
	DWORD		deviceNum;

	if (GetSelectedDevice(serial))
	{
		// If the selected device has been removed
		if (!FindDevice(serial, deviceNum))
		{
			// Then update the device list
			UpdateDeviceList();
			//UpdateDeviceInformation(FALSE);
		}
	}
}


void CDlgSelectReader::OnCbnDropdownComboReaderList()
{
	// Automatically update the device list when the
	// list is opened/dropped down
	UpdateDeviceList();
}


void CDlgSelectReader::OnCbnSelchangeComboReaderList()
{
	// Disconnecting
	if (m_bConnect)
	{
		Disconnect();
	}
	// Connecting
	else
	{
		Connect();

		if (m_bConnect)
		{
			// Close
			OnOK();
		}
		
	}
}


BOOL CDlgSelectReader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// init with Empty connection
	m_bConnect = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSelectReader::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	// Set exit state
	*p_ucExitState = 1;

	CDialogEx::OnCancel();
}
