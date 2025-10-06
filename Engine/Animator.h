/*****************************************************************//**
 * \file   Animator.h
 * \brief  �A�j���[�V�����R���|�[�l���g
 * 
 * \author �e�r���l
 * \date   2025/10/6
 *********************************************************************/
#pragma once

#include "Component.h"
#include "sprite_anim.h"
#include <memory>

namespace Engine {

class Animator : public Component {
public:
    Animator();
    virtual ~Animator() = default;

    void SetAnimationPattern(AnimPattern* pattern);
    void Play();
    void Stop();
    void Pause();
    
    virtual void Update(double deltaTime) override;
    virtual void Draw() override;

    bool IsPlaying() const { return m_isPlaying; }

private:
    std::unique_ptr<AnimPatternPlayer> m_player;
    AnimPattern* m_pattern = nullptr;
    bool m_isPlaying = false;
};

} // namespace Engine