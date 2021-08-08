#pragma once
#ifndef __RINGBUF__
#define __RINGBUF__

#include <string.h>

// �����O�o�b�t�@�̃T�C�Y
// �G�t�F�N�^�[(�f�B���C�Ȃ�)�Ŏg�p����z��Ȃ̂�
// �Ƃ肠����4�b���m�ۂ��Ă���(�T���v�����O���[�g 44,100Hz)
#define RB_SIZE (44100 * 4)

// ===================================================================================
// �����O�o�b�t�@�N���X
// ===================================================================================
class CRingBuffur
{
private:
	int rpos; // �ǂݍ��݈ʒu
	int wpos; // �������݈ʒu

	float buf[RB_SIZE]; // �����o�b�t�@

public:
	inline CRingBuffur();

	// �ǂݍ��݈ʒu�Ə������݈ʒu�̊Ԋu��ݒ肷��֐�
	// �f�B���C�G�t�F�N�^�[�̏ꍇ�͂��̂܂ܒx������(�f�B���C�^�C��)�ɂȂ�
	inline void SetInterval(int interval);

	// �����o�b�t�@�̓ǂݍ��݈ʒu(rpos)�̃f�[�^��ǂݍ��ފ֐�
	// ������pos�͓ǂݍ��݈ʒu(rpos)����̑��Έʒu
	// (���Έʒu(pos)�̓R�[���X��s�b�`�V�t�^�Ȃǂ̃G�t�F�N�^�[�ŗ��p����)
	inline float Read(int pos = 0);

	// �����o�b�t�@�̏������݈ʒu(wpos)�Ƀf�[�^���������ފ֐�
	inline void  Write(float in);

	// �����o�b�t�@�̓ǂݍ��݈ʒu(rpos)�A�������݈ʒu(wpos)����i�߂�֐�
	inline void Update();

	inline float GetPower();

};


// �R���X�g���N�^
CRingBuffur::CRingBuffur()
{
	// ���������s��
	rpos = 0;
	wpos = RB_SIZE / 2; // �Ƃ肠�����o�b�t�@�T�C�Y�̔������炢�ɂ��Ă���

	memset(buf, 0, sizeof(float) * RB_SIZE);
}


// �ǂݍ��݈ʒu�Ə������݈ʒu�̊Ԋu��ݒ肷��֐�
void CRingBuffur::SetInterval(int interval)
{
	// �ǂݍ��݈ʒu�Ə������݈ʒu�̊Ԋu��ݒ�

	// �l��0�ȉ���o�b�t�@�T�C�Y�ȏ�ɂȂ�Ȃ��悤����
	interval = interval % RB_SIZE;
	if (interval <= 0) { interval = 1; }

	// �������݈ʒu��ǂݍ��݈ʒu����interval�����������Đݒ�
	wpos = (rpos + interval) % RB_SIZE;
}


// �����o�b�t�@�̓ǂݍ��݈ʒu(rpos)�̃f�[�^��ǂݍ��ފ֐�
float CRingBuffur::Read(int pos)
{
	// �ǂݍ��݈ʒu(rpos)�Ƒ��Έʒu(pos)������ۂɓǂݍ��ވʒu���v�Z����B
	int tmp = rpos + pos;
	while (tmp < 0)
	{
		tmp += RB_SIZE;
	}
	tmp = tmp % RB_SIZE; // �o�b�t�@�T�C�Y�ȏ�ɂȂ�Ȃ��悤����

	// �ǂݍ��݈ʒu�̒l��Ԃ�
	return buf[tmp];
}


// �����o�b�t�@�̏������݈ʒu(wpos)�Ƀf�[�^���������ފ֐�
void  CRingBuffur::Write(float in)
{
	// �������݈ʒu(wpos)�ɒl����������
	buf[wpos] = in;
}


// �����o�b�t�@�̓ǂݍ��݈ʒu(rpos)�A�������݈ʒu(wpos)����i�߂�֐�
void  CRingBuffur::Update()
{
	// �����o�b�t�@�̓ǂݍ��݈ʒu(rpos)�A�������݈ʒu(wpos)����i�߂�
	rpos = (rpos + 1) % RB_SIZE;
	wpos = (wpos + 1) % RB_SIZE;
}

float CRingBuffur::GetPower()
{
	float pow = 0.0f;
	for (int ipos = rpos ; ipos < (wpos - rpos) ; ipos++) {
		pow += buf[ipos];
	}
	return pow / (wpos - rpos);
}

#endif