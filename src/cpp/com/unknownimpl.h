#pragma once
#include <unknwn.h>
//#include "com/module.h"

namespace NkCom
{
	class CModule;
	class CUnknownImpl : public IUnknown
	{
	public:
		CUnknownImpl() : m_cRef(0), m_pModule(0) {}
		CUnknownImpl(CModule* pModule) : m_cRef(0), m_pModule(pModule)
		{
			//m_pModule->LockServer(TRUE);
		}
		~CUnknownImpl() {}
		ULONG m_cRef;
		CModule* m_pModule;
	};

#define NK_ADDREF_RELEASE \
	STDMETHOD_(ULONG, AddRef)() { return ++m_cRef; } \
	STDMETHOD_(ULONG, Release)() { if (--m_cRef != 0)  \
	return m_cRef; delete this; return 0; }

#define NK_QUERYINTERFACE_BEGIN STDMETHOD(QueryInterface) \
	(REFGUID iid, void **outObject) {
#define NK_QUERYINTERFACE_ENTRY(i) if (iid == IID_ ## i) \
		{ *outObject = (void *)(i *)this; AddRef(); return S_OK; }

#define NK_QUERYINTERFACE_ENTRY_UNKNOWN(i) if (iid == IID_IUnknown) \
		{ *outObject = (void *)(IUnknown *)(i *)this; AddRef(); return S_OK; }

#define NK_QUERYINTERFACE_END return E_NOINTERFACE; }

#define NK_UNKNOWN_IMP_SPEC(i) \
	NK_QUERYINTERFACE_BEGIN \
	i \
	NK_QUERYINTERFACE_END \
	NK_ADDREF_RELEASE

#define NK_UNKNOWN_IMP NK_QUERYINTERFACE_BEGIN \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(IUnknown) \
	NK_QUERYINTERFACE_END \
	NK_ADDREF_RELEASE

#define NK_UNKNOWN_IMP1(i) NK_UNKNOWN_IMP_SPEC( \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(i) \
	NK_QUERYINTERFACE_ENTRY(i) \
	)

#define NK_UNKNOWN_IMP2(i1, i2) NK_UNKNOWN_IMP_SPEC( \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(i1) \
	NK_QUERYINTERFACE_ENTRY(i1) \
	NK_QUERYINTERFACE_ENTRY(i2) \
	)

#define NK_UNKNOWN_IMP3(i1, i2, i3) NK_UNKNOWN_IMP_SPEC( \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(i1) \
	NK_QUERYINTERFACE_ENTRY(i1) \
	NK_QUERYINTERFACE_ENTRY(i2) \
	NK_QUERYINTERFACE_ENTRY(i3) \
	)

#define NK_UNKNOWN_IMP4(i1, i2, i3, i4) NK_UNKNOWN_IMP_SPEC( \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(i1) \
	NK_QUERYINTERFACE_ENTRY(i1) \
	NK_QUERYINTERFACE_ENTRY(i2) \
	NK_QUERYINTERFACE_ENTRY(i3) \
	NK_QUERYINTERFACE_ENTRY(i4) \
	)

#define NK_UNKNOWN_IMP5(i1, i2, i3, i4, i5) NK_UNKNOWN_IMP_SPEC( \
	NK_QUERYINTERFACE_ENTRY_UNKNOWN(i1) \
	NK_QUERYINTERFACE_ENTRY(i1) \
	NK_QUERYINTERFACE_ENTRY(i2) \
	NK_QUERYINTERFACE_ENTRY(i3) \
	NK_QUERYINTERFACE_ENTRY(i4) \
	NK_QUERYINTERFACE_ENTRY(i5) \
	)
}