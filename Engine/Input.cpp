#include "Input.h"

namespace Engine {

bool Input::s_currentKeys[256] = {false};
bool Input::s_previousKeys[256] = {false};

void Input::Update() {
    // �O�t���[���̏�Ԃ�ۑ�
    for (int i = 0; i < 256; ++i) {
        s_previousKeys[i] = s_currentKeys[i];
    }
    
    // ���݂̏�Ԃ��擾
    for (int i = 0; i < 256; ++i) {
        s_currentKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
}

bool Input::GetKey(int keyCode) {
    return s_currentKeys[keyCode];
}

bool Input::GetKeyDown(int keyCode) {
    return s_currentKeys[keyCode] && !s_previousKeys[keyCode];
}

bool Input::GetKeyUp(int keyCode) {
    return !s_currentKeys[keyCode] && s_previousKeys[keyCode];
}

} // namespace Engine