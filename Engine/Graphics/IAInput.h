#pragma once 
#include <boost/hana.hpp>
#include <boost/hana/for_each.hpp>
#include <typeinfo>
#include <typeindex>
namespace EngineFramework {

    class Interface {
    public:
        virtual constexpr D3D12_INPUT_LAYOUT_DESC GetLayout() const PURE;
    };

    template<typename T>
    class InputLayout : public Interface{ 
    private:
        template<typename MemberType>
        constexpr DXGI_FORMAT GetFormat() {
            if constexpr (std::is_same<MemberType, DirectX::XMFLOAT2>::value) {
                return DXGI_FORMAT_R32G32_FLOAT;
            }
            else if constexpr (std::is_same<MemberType, DirectX::XMFLOAT3>::value) {
                return DXGI_FORMAT_R32G32B32_FLOAT;
            }
            else if constexpr (std::is_same<MemberType, DirectX::XMFLOAT4>::value) {
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            }
            else {
                static_assert(std::_Always_false<MemberType>, "aaa");
            }
        }
    public:
         constexpr InputLayout(D3D12_INPUT_CLASSIFICATION InputClassification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA) {
            UINT index{ 0 };

            boost::hana::for_each(boost::hana::keys(T{}), [&](auto key) {
                using MemberType = typename std::remove_reference<decltype(boost::hana::at_key(T{}, key))>::type;
                m_inputElements[index++] = { boost::hana::to<char const*>(key), 0, GetFormat<MemberType>(), 0, D3D12_APPEND_ALIGNED_ELEMENT, InputClassification, 0 };

                });
         }
        constexpr ~InputLayout() {};
        
        virtual constexpr D3D12_INPUT_LAYOUT_DESC GetLayout() const override{
            return D3D12_INPUT_LAYOUT_DESC{ m_inputElements.data(),static_cast<UINT>(m_inputElements.size()) };
        }
    private:

        std::array <D3D12_INPUT_ELEMENT_DESC, boost::hana::size(boost::hana::keys(T{}))> m_inputElements{};
    };

}