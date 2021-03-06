/*	This file is part of NKOpcTunnel.
*
*	Copyright (c) Henryk Anschuetz 
*	Berlin, Germany
*
*	mailto:uzuul23@online.de
*
*	NKOpcTunnel is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   NKOpcTunnel is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with NKOpcTunnel.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "stdafx.h"
#include "NkTnlClientCnf.h"
#include "NkTnlEditDlg.h"
#include "afxdialogex.h"
#include "NkTnlBrowseOPCServerDlg.h"
#include "PassDlg.h"
#include <WinCred.h>

IMPLEMENT_DYNAMIC(CNkTnlEditDlg, CDialog)

CNkTnlEditDlg::CNkTnlEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNkTnlEditDlg::IDD, pParent)
	, m_strTunnelName(_T("NewTunnel"))
	, m_dwRemoteServerPort(NKOPCTnl::DefaultPort)
	, m_dwRemoteServerIPAddress(MAKELONG(MAKEWORD(1, 0), MAKEWORD(0,127)))
	, m_RemoteServerCLSID(GUID_NULL)
	, m_bRemoteServerDA1(FALSE)
	, m_bRemoteServerDA2(FALSE)
	, m_bRemoteServerDA3(FALSE)
	, m_bUseSSL(TRUE)
	, m_bVerifyServer(TRUE)
	, m_bClientCertificate(TRUE)
{
}

CNkTnlEditDlg::~CNkTnlEditDlg()
{
}

void CNkTnlEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTunnelName);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwRemoteServerPort);
	DDX_Text(pDX, IDC_EDIT_CLSID_NAME, m_strRemoteServerUserType);
	DDX_Text(pDX, IDC_EDIT_CLSID, m_strRemoteServerProgID);
	DDX_IPAddress(pDX, IDC_IPADDRESS, m_dwRemoteServerIPAddress);
	DDX_Text(pDX, IDC_EDIT_GUID, m_RemoteServerCLSID);
	DDX_Check(pDX, IDC_CHECK_DA1, m_bRemoteServerDA1);
	DDX_Check(pDX, IDC_CHECK_DA2, m_bRemoteServerDA2);
	DDX_Check(pDX, IDC_CHECK_DA3, m_bRemoteServerDA3);
	DDX_Control(pDX, IDC_STATIC_UNIQUE, m_wndStaticError);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Check(pDX, IDC_CHECK_USE_SSL, m_bUseSSL);
	DDX_Check(pDX, IDC_CHECK_VERIFY_SERVER, m_bVerifyServer);
	DDX_Check(pDX, IDC_CHECK_CLIENT_CERT, m_bClientCertificate);
	DDX_Control(pDX, IDC_CHECK_VERIFY_SERVER, m_wndCheckVerifyServer);
	DDX_Control(pDX, IDC_CHECK_CLIENT_CERT, m_wndCheckClientCert);
}

BEGIN_MESSAGE_MAP(CNkTnlEditDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_NAME, &CNkTnlEditDlg::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_OPC_SERVER, &CNkTnlEditDlg::OnBnClickedButtonSelectOpcServer)
	ON_BN_CLICKED(IDC_BUTTON_PASS, &CNkTnlEditDlg::OnBnClickedButtonPass)
	ON_BN_CLICKED(IDC_CHECK_USE_SSL, &CNkTnlEditDlg::OnBnClickedCheckUseSsl)
END_MESSAGE_MAP()

BOOL CNkTnlEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strOldTunnelName = m_strTunnelName;

	UpdateState();
	
	return TRUE; 
}

void CNkTnlEditDlg::UpdateState()
{
	m_wndCheckClientCert.EnableWindow(m_bUseSSL);
	m_wndCheckVerifyServer.EnableWindow(m_bUseSSL);
}

void CNkTnlEditDlg::OnEnChangeEditName()
{
	UpdateData();

	for (INT_PTR loop = 0; loop < m_strNames.GetCount(); ++loop) {
		if (m_strNames[loop] == m_strTunnelName) {
			m_wndStaticError.ShowWindow(SW_SHOW);
			m_wndOK.EnableWindow(FALSE);
			return;
		}
	}
	m_wndStaticError.ShowWindow(SW_HIDE);
	m_wndOK.EnableWindow(TRUE);
}

void CNkTnlEditDlg::OnBnClickedCheckUseSsl()
{
	UpdateData();
	UpdateState();
}

void CNkTnlEditDlg::OnBnClickedButtonSelectOpcServer()
{
	UpdateData();
	CNkTnlBrowseOPCServerDlg dlg;
	dlg.m_pvecPass = &m_vecPass;

	NkType::to_Addr<CStringA>(m_dwRemoteServerIPAddress
		, m_dwRemoteServerPort, dlg.m_strServerAddr);

	dlg.m_bUseSSL = m_bUseSSL;
	dlg.m_bVerifyServer = m_bVerifyServer;
	dlg.m_bClientCertificate = m_bClientCertificate;

	if (dlg.DoModal() == IDOK) {
		CNkTnlBrowseOPCServerDlg::CServerInfo* pInfo = dlg.GetSelServerInfo();
		if (pInfo) {
			m_RemoteServerCLSID = pInfo->m_CLSID;
			m_strRemoteServerUserType = pInfo->m_strUserType;
			m_strRemoteServerProgID = pInfo->m_strProgID;
			m_bRemoteServerDA1 = pInfo->m_bOPCDAServer10;
			m_bRemoteServerDA2 = pInfo->m_bOPCDAServer20;
			m_bRemoteServerDA3 = pInfo->m_bOPCDAServer30;
		}
	}
	UpdateData(FALSE);
}

void CNkTnlEditDlg::OnBnClickedButtonPass()
{
	CString strTargetName;
	strTargetName.Format(NKOPCTnl::TunnelRealNameFormat, m_strTunnelName);

	CString strMessageText;
	strMessageText.LoadString(IDS_ENTER_CREDENTIALS);

	try {
		NkUtil::CPassVault pass(strTargetName);
		pass.readIf();
		if (pass.cred_dlg(strMessageText, strTargetName, 0, m_hWnd)) {
			pass.write();
		}
	} catch (NkError::CException& e) {  
		e.report();
	}
}

void CNkTnlEditDlg::OnOK()
{
	if (m_strOldTunnelName != m_strTunnelName) {
		CString strOldTargetName;
		strOldTargetName.Format(NKOPCTnl::TunnelRealNameFormat, m_strOldTunnelName);
		CString strTargetName;
		strTargetName.Format(NKOPCTnl::TunnelRealNameFormat, m_strTunnelName);

		try {
			NkUtil::CPassVault pass(strOldTargetName);
			if (pass.readIf()) {
				pass.rename(strTargetName);
			}
		}
		catch (NkError::CException& e) {
			e.report();
		}
	}
	__super::OnOK();
}


