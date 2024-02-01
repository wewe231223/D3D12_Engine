
namespace System {
	class Engine abstract{
	public:
		Engine() = default;
		virtual ~Engine() PURE;
	public:
		virtual void Render() PURE;
		virtual void Update() PURE;
		virtual void Resize() PURE;
	};

	class DirectXEngine : public Engine {
	public:
		DirectXEngine() = default;
		~DirectXEngine() override;
	public:
		virtual void Render() override;
		virtual void Update() override;
		virtual void Resize() override;
	};
}