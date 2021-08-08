#pragma once

#define BUF_LENGTH 44100

#include <math.h>

#include "CMyfilter.h"

// --------------------------------------------------------------------------------
// LowPassGate
// --------------------------------------------------------------------------------
class FilterGate
{
private:
	// �t�B���^�̌W��
	float a0, a1, a2, b0, b1, b2;
	// �Q�[�g
	float threshold;
	// ����
	float decay;

	// �o�b�t�@
	float out1, out2;
	float in1, in2;


public:
	inline FilterGate();

	// ���͐M���Ƀt�B���^��K�p����֐�
	inline float Process(float in);

	// �Q�[�g�����֐�
	inline float Gate(float in);

	// �t�B���^�W�����v�Z���郁���o�[�֐�
	inline void LowPassGate(float freq, float q, float samplerate = 44100.0f);
	inline void HiPassGate(float freq, float q, float samplerate = 44100.0f);
	inline void BandPassGate(float freq, float q, float samplerate = 44100.0f);

};

// --------------------------------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------------------------------
FilterGate::FilterGate()
{
	// �����o�[�ϐ���������
	a0 = 1.0f; // 0�ȊO�ɂ��Ă����Ȃ��Ə��Z�ŃG���[�ɂȂ�
	a1 = 0.0f;
	a2 = 0.0f;
	b0 = 1.0f;
	b1 = 0.0f;
	b2 = 0.0f;

	threshold = 0.0f;
	decay = 0.5f;

	in1 = 0.0f;
	in2 = 0.0f;

	out1 = 0.0f;
	out2 = 0.0f;
	
}

// --------------------------------------------------------------------------------
// ���͐M���Ƀt�B���^��K�p����֐�
// --------------------------------------------------------------------------------
float FilterGate::Process(float in)
{
	// ���͐M���Ƀt�B���^��K�p���A�o�͐M���ϐ��ɕۑ��B
	float out = b0 / a0 * in + b1 / a0 * in1 + b2 / a0 * in2
		- a1 / a0 * out1 - a2 / a0 * out2;

	in2 = in1; // 2�O�̓��͐M�����X�V
	in1 = in;  // 1�O�̓��͐M�����X�V

	out2 = out1; // 2�O�̏o�͐M�����X�V
	out1 = out;  // 1�O�̏o�͐M�����X�V

	// �Q�[�g����
	// gate�l���Ⴂ�ꍇ�M����OFF��
	out = Gate(out);
	
	// �o�͐M����Ԃ�
	return out;
}

// --------------------------------------------------------------------------------
// �Q�[�g�����֐�
// --------------------------------------------------------------------------------

float FilterGate::Gate(float in) {

	// 
	float ratio = 0.01f;

	// ���[�p�X�t�B���^�[��ݒ�
	static CMyFilter envfilter;   // ���������m���邽�߂Ɏg�����[�p�X�t�B���^
	static CMyFilter gainfilter; // �}���ȉ��ʕω�������邽�߂̃��[�p�X�t�B���^
	// �J�b�g�I�t���g���������قǉ����ω��ɕq���ɂȂ�B�ڈ���10�`50Hz���x
	envfilter.LowPass(30.0f, 1.0);
	// �J�b�g�I�t���g���������قǋ}���ȉ��ʕω��ɂȂ�B�ڈ���5�`50Hz���x
	gainfilter.LowPass(5.0f, 1.0);

	float gain = 1.0f;

	// ���͐M���̐�Βl���Ƃ������̂����[�p�X�t�B���^�ɂ����ĉ��������m����
	float tmp = envfilter.Process(abs(in));
	if (tmp < threshold)
	{
		gain = gainfilter.Process(tmp) * ratio;
	}
	// ����(�Q�C��)���}���ɕω����Ȃ��悤���[�p�X�t�B���^��ʂ�
	gain = gainfilter.Process(gain);

	return gain * in;

}

// --------------------------------------------------------------------------------
// �t�B���^�W�����v�Z���郁���o�[�֐�
// --------------------------------------------------------------------------------

void FilterGate::LowPassGate(float freq, float q, float samplerate)
{
	// �t�B���^�W���v�Z�Ŏg�p���钆�Ԓl�����߂�B
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// �t�B���^�W�������߂�B
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = (1.0f - cos(omega)) / 2.0f;
	b1 = 1.0f - cos(omega);
	b2 = (1.0f - cos(omega)) / 2.0f;

	// ���g������Q�[�g�������l�̃Z�b�g
	// ����g�ɂȂ�΃Q�[�g�̂������l���オ��悤�ɂȂ邽��
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}

void FilterGate::HiPassGate(float freq, float q, float samplerate)
{
	// �t�B���^�W���v�Z�Ŏg�p���钆�Ԓl�����߂�B
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// �t�B���^�W�������߂�B
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = (1.0f + cos(omega)) / 2.0f;
	b1 = - (1.0f + cos(omega));
	b2 = (1.0f + cos(omega)) / 2.0f;

	// ���g������Q�[�g�������l�̃Z�b�g
	// ����g�ɂȂ�΃Q�[�g�̂������l���オ��悤�ɂȂ邽��
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}

void FilterGate::BandPassGate(float freq, float q, float samplerate)
{
	// �t�B���^�W���v�Z�Ŏg�p���钆�Ԓl�����߂�B
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// �t�B���^�W�������߂�B
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = alpha;
	b1 = 0.0f;
	b2 = -alpha;

	// ���g������Q�[�g�������l�̃Z�b�g
	// ����g�ɂȂ�΃Q�[�g�̂������l���オ��悤�ɂȂ邽��
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}