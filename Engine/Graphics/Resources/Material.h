namespace EngineFramework {
	namespace Resource {

		template<typename... Args>
		concept StringArgs = (std::same_as<Args, std::tstring> && ...);

		class Material {
		public:
			Material();
			template<typename... Args>
			Material(Args...) requires StringArgs<Args...>;
			~Material();
		private:
			ComPtr<ID3D12DescriptorHeap> m_d3dDescriptorHeap{ nullptr };
			std::vector<CD3DX12_DESCRIPTOR_RANGE> m_d3dDescriptorRange{};
			UINT m_nDescriptorHandleSize{};
			const D3D12_DESCRIPTOR_RANGE_TYPE m_d3dDescriptorType{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV };
		public:
		private:
		};


		template<typename ...Args>
		inline Material::Material(Args ...TexArgs) requires StringArgs<Args...>{
			UINT NumofDescriptor{ sizeof...(TexArgs) };

			m_d3dDescriptorRange.emplace_back(CD3DX12_DESCRIPTOR_RANGE{ m_d3dDescriptorType,NumofDescriptor,0 });




		}


	}
}