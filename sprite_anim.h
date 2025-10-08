/*****************************************************************//**
 * \file   sprite_anim.h
 * \brief  
 * 
 * \author 菊池凌斗
 * \date   2025/6/13
 *********************************************************************/
#pragma once

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

#include <DirectXMath.h>

class AnimPattern {
private:
	int m_texid;
	int m_patternMax;
	double m_secondPerPattern;
	DirectX::XMUINT2 m_startPosition;
	DirectX::XMUINT2 m_patternSize;	// x->w y->h
	bool m_isLoop;

public:
	AnimPattern() = delete;
	AnimPattern(int texid, int patternMax, double secondPerPattern,
		DirectX::XMUINT2 startPosition, DirectX::XMUINT2 patternSize)
		: m_texid(texid)
		, m_patternMax(patternMax)
		, m_secondPerPattern(secondPerPattern)
		, m_startPosition(startPosition)
		, m_patternSize(patternSize)
		//, m_isLoop(isLoop)
	{
	}

	int GetPatternMax() { return m_patternMax; }
	double GetSecondPerPattern() { return m_secondPerPattern; };
	bool IsLoop() { return m_isLoop; }

	void Draw(float dx, float dy, float dw, float dh, int patternNum, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
};

class AnimPatternPlayer {
private:
	int m_pattern = 0;	// 再生中のパターン番号
	double m_accumulated_time = 0.0;	// 蓄積時間
	AnimPattern* m_pAnimPattern = nullptr;

public:
	AnimPatternPlayer() = delete;
	AnimPatternPlayer(AnimPattern* pAnimPattern) : m_pAnimPattern(pAnimPattern) {

	}

	void Update(double elapsed_time);

	void Draw(float dx, float dy, float dw, float dh, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// アニメーションパターンのゲッター・セッター
	AnimPattern* GetCurrentPattern() const { return m_pAnimPattern; }
	
	void SetPattern(AnimPattern* pAnimPattern) {
		if (m_pAnimPattern != pAnimPattern) {
			m_pAnimPattern = pAnimPattern;
			m_pattern = 0;  // 新しいアニメーションは最初から開始
			m_accumulated_time = 0.0;  // 時間もリセット
		}
	}
	
	// アニメーションの状態を取得するメソッド
	int GetCurrentFrame() const { return m_pattern; }
	double GetAccumulatedTime() const { return m_accumulated_time; }
	
	// アニメーションの再生状態をリセット
	void Reset() {
		m_pattern = 0;
		m_accumulated_time = 0.0;
	}
	
	// アニメーションが有効かチェック
	bool IsValid() const { return m_pAnimPattern != nullptr; }
};

#endif// SPRITE_ANIM_H
