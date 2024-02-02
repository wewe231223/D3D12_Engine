namespace System {
	class Engine {
	public:
		Engine();
		virtual ~Engine();
	public:
		virtual void Initialize();
		virtual void Render();
		virtual void Update();
		virtual void Resize();
	};
}