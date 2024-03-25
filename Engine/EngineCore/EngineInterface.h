#pragma once 
namespace EngineFramework {
	__interface IDevice {
		virtual ComPtr<ID3D12Device> GetDevice() const PURE;
		virtual ComPtr<IDXGIFactory4> GetDxgiFactory() const PURE;
	};

	__interface ICommandQueue {
		virtual ComPtr<ID3D12CommandQueue> GetCommandQueue() const PURE;
		virtual void Execute(const __interface ICommandList*) const PURE;
		virtual void Execute(ID3D12CommandList*,UINT) const PURE;
		virtual void Sync() const PURE;
	};

	__interface ICommandList {
		virtual void Open() const PURE;
		virtual void Close() const PURE;
		virtual ComPtr<ID3D12GraphicsCommandList> GetCommandList() const PURE;
	};

	__interface ISwapChain {
		virtual ComPtr<IDXGISwapChain> GetDxgiSwapChain() const PURE;
		virtual const D3D12_VIEWPORT* GetViewPort() const PURE;
		virtual const D3D12_RECT* GetSissorRect() const PURE;
		virtual ComPtr<ID3D12Resource> GetCurrentBackBuffer() const PURE;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const PURE;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const PURE;
		virtual void SwapBuffer() const PURE;
		virtual void Present() const PURE;
	};


	__interface IShader {
		virtual D3D12_SHADER_BYTECODE GetShaderByteCode(ShaderType) const PURE;
	};

	enum class RegisterType {
		T = 1,
		B,
		U,
		S,
		SRV = T,
		CBV = B,
		UAV = U,
		SAMPLER = S,
		END
	};


	enum class ResourceType {
		Root32BitConstants = 100U,
		RootCBV,
		RootSRV,
		RootUAV,
		DescriptorTable
	};
	/*
	* Root32bitConstants
	* RootDescriptor ( RootCBV RootSRV RootUAV )
	* DescriptorTable
	*/
	
	class RegisterBlock {
	public:
		RegisterBlock() = default;
		RegisterBlock(RegisterType Registertype, UINT RegisterStart, UINT RegisterCount) : m_regType(Registertype), m_regStart(RegisterStart), m_regCount(RegisterCount) {};
	public:
		RegisterType m_regType{};
		ResourceType m_resourceType{};
		UINT m_regStart{};
		UINT m_regCount{};
	public:
		bool operator==(const RegisterBlock&& other) const {
			if (
				m_regType == other.m_regType	and 
				m_regStart == other.m_regStart	and 
				m_regCount == other.m_regCount) {
				return true;
			} 
			return false;
		}
	};

	__interface IRootSignature {
		virtual ComPtr<ID3D12RootSignature> GetRootSignature() const PURE;
		virtual UINT GetBufferIndex() const PURE;
	};

	__interface IDescriptorTable {
		virtual DescriptorID CreateDescriptor(D3D12_DESCRIPTOR_RANGE_TYPE, UINT) PURE;
		virtual void SetDescriptor(const IDevice*, DescriptorID, D3D12_CPU_DESCRIPTOR_HANDLE) const PURE;
	};


	class DescriptorObject {
	public:
		DescriptorObject() = delete;
		DescriptorObject(D3D12_CPU_DESCRIPTOR_HANDLE HeapHandle, DescriptorID DescID) : m_d3dDescriptorHandle(HeapHandle), m_nTableID(DescID) {};
	public:
		D3D12_CPU_DESCRIPTOR_HANDLE m_d3dDescriptorHandle{};
		DescriptorID m_nTableID{};
	};


	__interface DescriptorResourceContainer {
	public:
		virtual DescriptorObject Clone() const PURE;
		virtual void GetDescriptorID(IDescriptorTable* pDescriptorTable, UINT nShaderRegister) PURE;
	};


}