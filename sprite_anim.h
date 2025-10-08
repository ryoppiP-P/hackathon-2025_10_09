/*****************************************************************//**
 * \file   sprite_anim.h
 * \brief  
 * 
 * \author �e�r���l
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
	int m_pattern = 0;	// �Đ����̃p�^�[���ԍ�
	double m_accumulated_time = 0.0;	// �~�ώ���
	AnimPattern* m_pAnimPattern = nullptr;

public:
	AnimPatternPlayer() = delete;
	AnimPatternPlayer(AnimPattern* pAnimPattern) : m_pAnimPattern(pAnimPattern) {

	}

	void Update(double elapsed_time);

	void Draw(float dx, float dy, float dw, float dh, DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// �A�j���[�V�����p�^�[���̃Q�b�^�[�E�Z�b�^�[
	AnimPattern* GetCurrentPattern() const { return m_pAnimPattern; }
	
	void SetPattern(AnimPattern* pAnimPattern) {
		if (m_pAnimPattern != pAnimPattern) {
			m_pAnimPattern = pAnimPattern;
			m_pattern = 0;  // �V�����A�j���[�V�����͍ŏ�����J�n
			m_accumulated_time = 0.0;  // ���Ԃ����Z�b�g
		}
	}
	
	// �A�j���[�V�����̏�Ԃ��擾���郁�\�b�h
	int GetCurrentFrame() const { return m_pattern; }
	double GetAccumulatedTime() const { return m_accumulated_time; }
	
	// �A�j���[�V�����̍Đ���Ԃ����Z�b�g
	void Reset() {
		m_pattern = 0;
		m_accumulated_time = 0.0;
	}
	
	// �A�j���[�V�������L�����`�F�b�N
	bool IsValid() const { return m_pAnimPattern != nullptr; }
};

#endif// SPRITE_ANIM_H
