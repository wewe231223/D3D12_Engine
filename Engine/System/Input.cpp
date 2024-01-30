#include "EnginePch.h"
#include "Input.h"

namespace System {
	Input* Input::m_pInstance = nullptr;

	Input::Input(){

	}

	Input::~Input(){
	}

	Input* Input::GetInstance(){
		if (!Input::m_pInstance) Input::m_pInstance = new Input();
		return Input::m_pInstance;
	}
	const KEY_STATE Input::GetKey(int key) const {

		return m_keyboardState[key];
	}

	const KEY_STATE Input::GetMouseButton(int key) const {

		return m_mouseState[key];
	}

	const KEY_STATE Input::GetMouseButton(MOUSE_BUTTON key) const {
		return m_mouseState[static_cast<int>(key)];
	}

	const LONG Input::GetDeltaMouseX() const {
		return m_nDeltaMouseX;
	}

	const LONG Input::GetDeltaMouseY() const {
		return m_nDeltaMouseY;
	}

	void Input::Init(HWND hWnd, HINSTANCE Instance) {
		m_keyboardState = new KEY_STATE[256];
		m_mouseState = new KEY_STATE[3];
		for (auto i = 0; i < 256; ++i) {
			m_keyboardState[i] = KEY_STATE::NONE;
		}
		for(auto i = 0; i < 3; ++i){
			m_mouseState[i] = KEY_STATE::NONE;
		}
		CheckFailed(DirectInput8Create(Instance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pDirectInput), NULL));
		
		CheckFailed(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoardDevice, NULL));
		m_pKeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
		CheckFailed(m_pKeyBoardDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY));
		while (m_pKeyBoardDevice->Acquire() == DIERR_INPUTLOST);

		CheckFailed(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL));
		m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
		CheckFailed(m_pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND));
		while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
	}

	void Input::Update() {
		BYTE KeyState[256]{};
		DIMOUSESTATE MouseState{};
		HRESULT hr{ S_OK };


		if (FAILED(m_pKeyBoardDevice->GetDeviceState(256, KeyState))) {
			while (m_pKeyBoardDevice->Acquire() == DIERR_INPUTLOST);
		}

		if (FAILED(m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &MouseState))) {
			while(m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
		}

		
		for (UINT i = 0; i < 256; ++i) {
			if (KeyState[i] & 0x80) {
				if (m_keyboardState[i] == KEY_STATE::NONE or m_keyboardState[i] == KEY_STATE::RELEASE) {
					m_keyboardState[i] = KEY_STATE::DOWN;
				}
				else if (m_keyboardState[i] == KEY_STATE::DOWN) {
					m_keyboardState[i] = KEY_STATE::PRESS;
				}
			}
			else {
				if (m_keyboardState[i] == KEY_STATE::PRESS or m_keyboardState[i] == KEY_STATE::DOWN) {
					m_keyboardState[i] = KEY_STATE::RELEASE;
				}
				else if (m_keyboardState[i] == KEY_STATE::RELEASE) {
					m_keyboardState[i] = KEY_STATE::NONE;
				}
			}
		}
		/*
		* 0 = Left Mouse Button
		* 1 = Right Mouse Button
		* 2 = Middle Mouse Button
		* 3 = Side Mouse Button( 하지만 이 키는 마우스별로 인식되는 버튼이 다를 수 있다. 따라서 이 키는 되도록 사용하지 말 것 )
		*/
		for (UINT i = 0; i < 4; ++i) {
			if (MouseState.rgbButtons[i] & 0x80) {
				if (m_mouseState[i] == KEY_STATE::NONE or m_mouseState[i] == KEY_STATE::RELEASE) {
					m_mouseState[i] = KEY_STATE::DOWN;
				}
				else if (m_mouseState[i] == KEY_STATE::DOWN) {
					m_mouseState[i] = KEY_STATE::PRESS;
				}
			}
			else {
				if (m_mouseState[i] == KEY_STATE::PRESS or m_mouseState[i] == KEY_STATE::DOWN) {
					m_mouseState[i] = KEY_STATE::RELEASE;
				}
				else if (m_mouseState[i] == KEY_STATE::RELEASE) {
					m_mouseState[i] = KEY_STATE::NONE;
				}
			}
		}

		/*
		* 위에서 아래로 Y+ 아래에서 위로 Y-
		* 왼쪽에서 오른쪽으로 X+ 오른쪽에서 왼쪽으로 X-
		*/

		m_nDeltaMouseX = MouseState.lX;
		m_nDeltaMouseY = MouseState.lY;

	}

	/// <summary>
	/// 싱글톤 객체는 항상 Terminate 를 호출할것
	/// </summary>
	void Input::Terminate() {
		if (m_keyboardState) delete[] m_keyboardState;
		m_keyboardState = nullptr;

		if (m_mouseState) delete[] m_mouseState;
		m_mouseState = nullptr;

		if (m_pMouseDevice) {
			m_pMouseDevice->Unacquire();
			m_pMouseDevice->Release();
			m_pMouseDevice = NULL;
		}

		if (m_pKeyBoardDevice) {
			m_pKeyBoardDevice->Unacquire();
			m_pKeyBoardDevice->Release();
			m_pKeyBoardDevice = NULL;
		}

		if (m_pDirectInput) {
			m_pDirectInput->Release();
			m_pDirectInput = NULL;
		}
		delete Input::m_pInstance;
	}


};
