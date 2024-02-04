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
		std::unique_ptr<class Device> m_pDevice{ nullptr };
		std::unique_ptr<class CommandQueue> m_pCommandQueue{ nullptr };
	public:
		virtual void Initialize() override;
		virtual void Render() override;
		virtual void Update() override;
		virtual void Resize() override;
	};
}