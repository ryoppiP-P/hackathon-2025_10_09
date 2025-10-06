/*****************************************************************//**
 * \file   Input.h
 * \brief  “ü—ÍŠÇ—ƒVƒXƒeƒ€
 * 
 * \author ‹e’r—½“l
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include <Windows.h>

namespace Engine {

class Input {
public:
    static void Update();
    
    static bool GetKey(int keyCode);
    static bool GetKeyDown(int keyCode);
    static bool GetKeyUp(int keyCode);

private:
    static bool s_currentKeys[256];
    static bool s_previousKeys[256];
};

} // namespace Engine