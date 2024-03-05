#include "Enginepch.h"
#include "Exeption.h"

namespace System {
	Exeption::Exeption(HRESULT hErrorCode, const std::tstring& ctsFuctionName, const std::tstring& ctsFileName, const int nLineNumber)
	:m_hrErrorCode(hErrorCode),m_tsFuctionName(ctsFuctionName),m_tsFileName(ctsFileName),m_nLineNumber(nLineNumber){
	}

	Exeption::Exeption(const std::tstring& ctsMessage) : m_tsMessage(ctsMessage){
		
	}

	std::tstring Exeption::ToString() const {
		if (m_nLineNumber == -1) return m_tsMessage;
		_com_error err(m_hrErrorCode);
		std::tstring errmsg = err.ErrorMessage();
		return m_tsFuctionName + _T("\nFailed in : ") + m_tsFileName + _T("\nLine : ") + std::to_wstring(m_nLineNumber) + _T("\nError : ") + errmsg;
	}
}

