#pragma once
namespace EngineFramework {
	class CommandQueue : public ICommandQueue {
	public:
		CommandQueue();
		~CommandQueue();
	private:
		ComPtr<ID3D12CommandQueue> m_d3dCommandQueue{ nullptr };
		ComPtr<ID3D12Fence> m_d3dFence{ nullptr };
		HANDLE m_hEventHandle{ nullptr };
		mutable UINT64 m_nFenceValue{ 0 };
		
	public:
		void Initialize(const IDevice* pDevice);
	public: // Interface 
		virtual ComPtr<ID3D12CommandQueue> GetCommandQueue() const override { return m_d3dCommandQueue; }
		virtual void Execute(const ICommandList* pCommandLists) const override;
		virtual void Execute(ID3D12CommandList* pCommandLists,UINT nCommandLists) const override;
		virtual void Sync() const override;
	};
}