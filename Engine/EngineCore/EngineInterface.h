#pragma once 
namespace EngineFramework {
	__interface IDevice {
		virtual ComPtr<ID3D12Device> GetDevice() const PURE;
		virtual ComPtr<IDXGIFactory4> GetDxgiFactory() const PURE;
	};

	__interface ICommandQueue {
		virtual ComPtr<ID3D12CommandQueue> GetCommandQueue() const PURE;
		virtual ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const PURE;
		virtual ComPtr<ID3D12GraphicsCommandList> GetCommandList() const PURE;
		virtual void FlushCommandQueue() const PURE;
	};

	__interface ISwapChain {
		virtual ComPtr<IDXGISwapChain> GetDxgiSwapChain() const PURE;
		virtual const D3D12_VIEWPORT* GetViewPort() const PURE;
		virtual const D3D12_RECT* GetSissorRect() const PURE;
		virtual ComPtr<ID3D12Resource> GetCurrentBackBuffer() const PURE;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const PURE;
		virtual D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const PURE;
		virtual void SwapBuffer() const PURE;
	};

	__interface IShader {
		virtual D3D12_SHADER_BYTECODE GetShaderByteCode(ShaderType ShaderType) const PURE;
	};
}