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
#include "comfarunknown.h"
#include "error/baseexception.h"

namespace NkCom
{
	CComFarUnknown::CComFarUnknown(CServer* pSrv, ULONG32* pid)
		: CUnknownImpl()
		, m_pSrv(pSrv)
		, m_cookie_git(0)
	{
		NkError::CBaseException::check_pointer(pSrv, __FILE__, __LINE__);

		m_id = pSrv->register_instance(this);

		if (pid) {
			(*pid) = m_id;
		}
	}

	CComFarUnknown::CComFarUnknown(CServer* pSrv, IUnknown* p, ULONG32* pid)
		: CUnknownImpl()
		, m_pSrv(pSrv)
		, m_cookie_git(0)
	{
		NkError::CBaseException::check_pointer(pSrv, __FILE__, __LINE__);

		IGlobalInterfaceTable* pGIT = m_pSrv->get_git();
		if (pGIT) {
			HRESULT hr = pGIT->RegisterInterfaceInGlobal(p, IID_IUnknown, &m_cookie_git);
			NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
		}
		else {
			m_spUnknown = p;
		}

		m_id = pSrv->register_instance(this);

		if (pid) {
			(*pid) = m_id;
		}
	}

	CComFarUnknown::CComFarUnknown(CServer* pSrv, REFCLSID rclsid, DWORD dwClsContext, REFIID riid, ULONG32* pid)
		: CUnknownImpl()
		, m_pSrv(pSrv)
		, m_cookie_git(0)
	{
		NkError::CBaseException::check_pointer(pSrv, __FILE__, __LINE__);
		NkError::CBaseException::check_result(m_spUnknown.CoCreateInstance(rclsid, dwClsContext, IID_IUnknown), __FILE__, __LINE__);

		m_id = pSrv->register_instance(this);

		if (pid) {
			(*pid) = m_id;
		}
	}

	CComFarUnknown::~CComFarUnknown()
	{
		try {
			if (m_cookie_git)
			{
				IGlobalInterfaceTable* pGIT = m_pSrv->get_git();
				if (pGIT) {
					HRESULT hr = pGIT->RevokeInterfaceFromGlobal(m_cookie_git);
					NkError::CBaseException::check_result(hr, __FILE__, __LINE__);
				}
			}
		}
		catch (NkError::CException& e) {
			e.report();
		}

		try {
			m_pSrv->unregister_instance(m_id);
		}
		catch (...) {}
	}

	ULONG32 CComFarUnknown::id()
	{
		return m_id;
	}

	HRESULT CComFarUnknown::internal_query_interface(REFIID iid)
	{
		if (iid == IID_IUnknown)
		{
			return S_OK;
		}
		else if (m_cookie_git > 0)
		{
			IGlobalInterfaceTable* pGIT = m_pSrv->get_git();
			NkError::CBaseException::check_pointer(pGIT, __FILE__, __LINE__);

			CComPtr<IUnknown> sp;
			return pGIT->GetInterfaceFromGlobal(m_cookie_git, iid, reinterpret_cast<void**>(&sp));
		}
		else
		{
			CComPtr<IUnknown> sp;
			return m_spUnknown->QueryInterface(iid, reinterpret_cast<void**>(&sp));
		}
	}

	
}