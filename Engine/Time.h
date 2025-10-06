/*****************************************************************//**
 * \file   Time.h
 * \brief  ���ԊǗ��V�X�e��
 * 
 * \author �e�r���l
 * \date   2025/10/6
 *********************************************************************/
#pragma once

namespace Engine {

class Time {
public:
    static double GetDeltaTime() { return s_deltaTime; }
    static double GetTime() { return s_totalTime; }
    static void Update(double deltaTime);

private:
    static double s_deltaTime;
    static double s_totalTime;
};

} // namespace Engine