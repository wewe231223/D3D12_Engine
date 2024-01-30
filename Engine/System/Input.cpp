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

	}

	void Input::Update() {
	}

	void Input::Terminate() {
	}


};
