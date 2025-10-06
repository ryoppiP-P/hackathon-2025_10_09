#include "Input.h"

namespace Engine {

bool Input::s_currentKeys[256] = {false};
bool Input::s_previousKeys[256] = {false};

void Input::Update() {
    // ‘OƒtƒŒ[ƒ€‚Ìó‘Ô‚ğ•Û‘¶
    for (int i = 0; i < 256; ++i) {
        s_previousKeys[i] = s_currentKeys[i];
    }
    
    // Œ»İ‚Ìó‘Ô‚ğæ“¾
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