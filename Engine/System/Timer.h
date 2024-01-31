#pragma once
namespace System {
	class Timer {
	public:
		Timer(HWND hWnd);
	private:
		HWND m_hWnd{};

		float m_fSecondsPerCount{};
		float m_fDeltaTime{ -1.f };

		INT64 m_nBaseTime{};
		INT64 m_nPausedTime{};
		INT64 m_nStopedTime{};
		INT64 m_nPrevTime{};
		INT64 m_nCurrentTime{};

		bool m_bStopped{ false };

		float m_fFpsTimeElapsed{ 0.f };
		int m_nFrameCount{ 0 };


	public:
		float GetTimeElapsed() const;
		float GetDeltaTime() const;


		void Reset();
		void Start();
		void Stop();
		void Update();
	};

}