#ifndef METEOR_H
#define METEOR_H

#include "sprite_anim.h"
#include <windows.h>
#include "sound.h"

class Meteor {
public:
    float x, y;
    float vx, vy;
    bool isExploding = false;
    double explosionTime = 0.0;
    static constexpr double EXPLOSION_DURATION = 0.5;
    int texidMeteor; // ← アニメーションではなく、テクスチャIDだけ
    AnimPatternPlayer playerExplosion;

    Meteor(float sx, float sy, float vx_, float vy_,
        int texid, AnimPattern* aExpl)
        : x(sx), y(sy), vx(vx_), vy(vy_),
        texidMeteor(texid), playerExplosion(aExpl)
    {
    }

    void Update(double elapsed) {
        if (!isExploding) {
            x += vx * (float)elapsed;
            y += vy * (float)elapsed;
            if (y > 500.0f) {   // 爆発判定
                isExploding = true;
                explosionTime = 0.0;
                if (isExploding) {
                    PlaySoundEffect(L"resource/audio/explosion.wav", 80);
                }
            }
        } else {
            x += 200.0f * (float)elapsed; // 右へ
            explosionTime += elapsed;
            playerExplosion.Update(elapsed);
        }
    }

    void Draw() {
        if (!isExploding) {
            Sprite_Draw(texidMeteor, x, y, 32, 32, 0, 0, 1600, 1600); // 静止画
        } else if (explosionTime < EXPLOSION_DURATION) {
            playerExplosion.Draw(x - 100, y - 100, 256, 256); // 爆発はアニメ
        }
    }
    bool IsFinished() const {
        return isExploding && explosionTime >= EXPLOSION_DURATION;
    }
};

#endif  // METEOR_H