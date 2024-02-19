#pragma once 

namespace EngineFramework {
	namespace Constants {
		template<typename Ty>
		class RootDescriptor {
		public:
			RootDescriptor();
			~RootDescriptor();
		private:
			ComPtr<ID3D12Resource> m_d3dBuffer{ nullptr };
			Ty* m_buffer{ nullptr };
			UINT m_nRootSigantureIndex{ 0 };
			size_t m_nBufferElementSize{ 0 };
			size_t m_nBufferSize{ 0 };
		public:
			void Initialize(const IDevice* pDevice,const IRootSignature* pRootSignature);
			void CommitData(const Ty& Data);
			void BindCommandList(const ICommandQueue* pCommandQueue);
		};
		template<typename Ty>
		inline RootDescriptor<Ty>::RootDescriptor(){

		}

		template<typename Ty>
		inline RootDescriptor<Ty>::~RootDescriptor(){
			if (m_buffer and m_d3dBuffer) m_d3dBuffer->Unmap(0, nullptr);
		}

		template<typename Ty>
		inline void RootDescriptor<Ty>::Initialize(const IDevice* pDevice, const IRootSignature* pRootSignature){
			m_nRootSigantureIndex = pRootSignature->GetBufferIndex();
			// 굳이 함수로?
			m_nBufferElementSize = (sizeof(Ty) + 0xff) & ~0xff;
			m_nBufferSize = sizeof(Ty);

			CD3DX12_HEAP_PROPERTIES HeapProperties{ D3D12_HEAP_TYPE_UPLOAD };
			CD3DX12_RESOURCE_DESC ResourceDesc{ CD3DX12_RESOURCE_DESC::Buffer(m_nBufferElementSize) };

			CheckFailed(pDevice->GetDevice()->CreateCommittedResource(
				&HeapProperties,
				D3D12_HEAP_FLAG_NONE,
				&ResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				IID_PPV_ARGS(m_d3dBuffer.GetAddressOf())
			));
			
			m_buffer = m_d3dBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_buffer));
		}

		template<typename Ty>
		inline void RootDescriptor<Ty>::CommitData(const Ty& Data){
			if (m_buffer) {
				::memcpy(m_buffer, &Data, m_nBufferSize);
			}
		}

		template<typename Ty>
		inline void RootDescriptor<Ty>::BindCommandList(const ICommandQueue* pCommandQueue){
			pCommandQueue->GetCommandList()->SetGraphicsRootConstantBufferView(m_nRootSigantureIndex, m_d3dBuffer->GetGPUVirtualAddress());
		}

	}
}