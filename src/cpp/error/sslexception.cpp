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
#include "sslexception.h"
#include "trace/trace.h"

namespace NkError
{
	CSSLException::CSSLException() : m_Error(ERROR_SUCCESS), m_nSourceLine(-1)
	{
	}

	CSSLException::CSSLException(const char* pszFunctionName, int error, const char* lpszfile, int line) : m_strFunctionName(pszFunctionName)
		, m_Error(error)
		, m_nSourceLine(line)
	{
		m_strSourceFile.file_name(lpszfile);
	}

	CSSLException::CSSLException(const CSSLException& d)
	{
		*this = d;
	}

	CSSLException & CSSLException::operator=(const CSSLException& d)
	{
		m_strFunctionName = d.m_strFunctionName;
		m_strSourceFile = d.m_strSourceFile;
		m_strErrorString = d.m_strErrorString;
		m_Error = d.m_Error;
		m_nSourceLine = d.m_nSourceLine;
		return *this;
	}

	HRESULT CSSLException::error() const
	{
		return E_FAIL;
	}

	const wchar_t* CSSLException::error_text(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		char errbuffer[1024];
		ERR_error_string_n(m_Error, errbuffer, sizeof(errbuffer));

		m_strErrorString = errbuffer;
		return m_strErrorString;
	}

	void CSSLException::report(DWORD dwLanguageId /*= MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)*/)
	{
		NkTrace::CTrace::Instance().Trace(NkTrace::CTrace::TraceError
			, L"%s(): (%d) %s file: %s line: %d", m_strFunctionName.data()
			, m_Error, error_text(dwLanguageId), m_strSourceFile.data(), m_nSourceLine);
	}

	int CSSLException::base_error() const
	{
		return 0;
	}
}