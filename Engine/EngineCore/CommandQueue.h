namespace Engine {
	class CommandQueue {
	public:
		CommandQueue() = default;
		~CommandQueue();
	private:
		ComPtr<ID3D12CommandQueue> m_d3dCommandQueue{ nullptr };
		ComPtr<ID3D12CommandAllocator> m_d3dCommandAllocator{ nullptr };
		ComPtr<ID3D12GraphicsCommandList> m_d3dGraphicsCommandList{ nullptr };

		ComPtr<ID3D12Fence> m_d3dFence{ nullptr };

	public:
		void Initialize();

	};
}