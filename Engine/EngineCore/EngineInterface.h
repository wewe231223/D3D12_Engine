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

	__interface IRootSignature {
		virtual ComPtr<ID3D12RootSignature> GetRootSignature() const PURE;
		virtual UINT GetBufferIndex() const PURE;
	};

	__interface IDescriptorTable {
		virtual DescriptorID CreateDescriptor(D3D12_DESCRIPTOR_RANGE_TYPE,UINT) PURE;
		virtual void SetDescriptor(const IDevice*,DescriptorID,D3D12_CPU_DESCRIPTOR_HANDLE) const PURE;
	};


}