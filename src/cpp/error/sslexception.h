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

#pragma once
#include "error/exception.h"
#include "types/string.h"
#include "openssl/err.h"
#include "openssl/ssl.h"
#include <crtdbg.h>

namespace NkError
{
	class CSSLException : public CException
	{
	public:
		CSSLException();
		CSSLException(const char* pszFunctionName, int error, const char* lpszfile, int line);
		CSSLException(const CSSLException& d);
		CSSLException &operator =(const CSSLException& d);

		HRESULT error() const;
		virtual int base_error() const;
		virtual const wchar_t* error_text(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
		virtual void report(DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));

		static void check_result(int ret, const char* pszFunctionName, const char* lpszfile, int line);
		static void check_create(const void* ret, const char* pszFunctionName, const char* lpszfile, int line);

	private:
		NkType::CString m_strFunctionName;
		NkType::CString m_strSourceFile;
		NkType::CString m_strErrorString;
		int m_Error;
		int m_nSourceLine;
	};

	inline void CSSLException::check_result(int ret, const char* pszFunctionName, const char* lpszfile, int line)
	{	
		if (ret <= 0) {
			_ASSERT(0);
			throw CSSLException(pszFunctionName, ERR_get_error(), lpszfile, line);
		}
	}

	inline void CSSLException::check_create(const void* ret, const char* pszFunctionName, const char* lpszfile, int line)
	{
		_ASSERT(ret != 0);
		if (ret == 0) {
			throw CSSLException(pszFunctionName, ERR_get_error(), lpszfile, line);
		}
	}
}