#pragma once

namespace System {
	class Exeption {
	public:
		Exeption() = default;
		Exeption(HRESULT hErrorCode, const std::tstring& ctsFuctionName, const std::tstring& ctsFileName, const int nLineNumber);

	private:
		HRESULT m_hrErrorCode{ S_OK };
		std::tstring m_tsFuctionName{};
		std::tstring m_tsFileName{};
		int m_nLineNumber{};
	public:
		std::tstring ToString() const;
	};

}

#define CheckFailed(hr) {														\
	HRESULT hr_ = (hr);															\
	if (FAILED(hr_)) { throw System::Exeption(hr_, L#hr, _T(__FILE__), __LINE__); }	\
}
