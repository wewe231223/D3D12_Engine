namespace EngineFramework {
	class Engine abstract{
	public:
		Engine() = default;
		virtual ~Engine() PURE;
	public:
		virtual void Initialize() PURE;
		virtual void Render() PURE;
		virtual void Update() PURE;
		virtual void Resize() PURE;
	};

	class DirectXEngine : public Engine {
	public:
		DirectXEngine();
		virtual ~DirectXEngine() override;
	private:
		ComPtr<ID3D12Device> m_d3dDevice{ nullptr };
	public:
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Update() override;
		virtual void Resize() override;
	};
}