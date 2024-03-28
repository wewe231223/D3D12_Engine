
struct ImGuiIO;

namespace EngineFramework {
	namespace UI {
		class MainUI {
		public:
			MainUI(HWND hWnd, IDevice* pDevice);
			~MainUI();
		private:
			std::unique_ptr<ImGuiIO> m_guiIO{ nullptr };
		};




	}
}