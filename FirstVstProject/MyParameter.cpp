#include "myparameter.h"

// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		// �R���X�g���N�^(�p���������o�[�ϐ��Ȃǂ̏��������s�����߂قڕK�{)
		MyParameter::MyParameter(const TChar* title, ParamID tag, const TChar* units, ParamValue minPlain,
			ParamValue maxPlain, ParamValue defaultValuePlain, int32 flags, UnitID unitID)
			: Parameter(title, tag, units, 0.0f, 0, flags, unitID) // �ꕔ�̈����͌p�����N���X�ɂ��̂܂ܓn���A����������
		{
			// �ŏ��l�A�ő�l��ݒ�
			min = minPlain;
			max = maxPlain;

			// �f�t�H���g�l��ݒ�
			// valueNormalized�͌p����(Parameter)�N���X�Œ�`���ꂽ���K�����ꂽ���݂̒l
			// toNormalized()�֐����g���Ĉ����̃f�t�H���g�l(�\���l)�𐳋K������
			valueNormalized = toNormalized(defaultValuePlain);
		}

		// ���K�����ꂽ�l(0.0f�`1.0f�̒l)��\���l�̕�����ɂ���֐�
		void MyParameter::toString(ParamValue valueNormalized, String128 string) const
		{
			// ���K�����ꂽ�l�𕶎���ɂ���ɂ�UString128�N���X���g��
			// UString128�N���X�͓����ɕ�����p�o�b�t�@(����128)���������N���X
			// �ڍׂ͊������邪�A����������낢��ϊ����邽�߂̊֐����p�ӂ���Ă���
			UString128 wrapper;

			// printFloat()�֐���float�̒l�𕶎���ɂ��Awrapper�̓���������p�o�b�t�@�ɕۑ�����
			// ���K�����ꂽ�l�Ȃ̂ŁAtoPlain()�֐����g���\���l�ɕϊ����Ă���ۑ�����B
			// precision�͏����扽�ʂ܂ŕ\�����邩�������ϐ�(�p�����N���X�Œ�`�ς�)
			wrapper.printFloat(toPlain(valueNormalized), precision);

			// ����������p�o�b�t�@����string�ɃR�s�[����
			wrapper.copyTo(string, 128);
		}

		// �\������Ă��镶���񂩂琳�K�����ꂽ�l(0.0f�`1.0f�̒l)���擾����֐�
		bool MyParameter::fromString(const TChar* string, ParamValue& valueNormalized) const
		{
			// �\���l�̕����񂩂琳�K���l���擾�ɂ���ɂ�UString128�N���X���g��
			// �܂��Awrapper�̓���������p�o�b�t�@��string��ݒ肷��
			UString wrapper((TChar*)string, strlen16(string));

			//  �\���l�̕����񂩂�\���l���擾����
			ParamValue plainvalue;
			if (wrapper.scanFloat(plainvalue))
			{
				// �\���l�擾�ɐ���������AtoNormalized()�֐��Ő��K�����AvalueNormalized�ɑ��
				valueNormalized = toNormalized(valueNormalized);

				// �ϊ��ɐ���������true��Ԃ�
				return true;
			}

			// �ϊ��Ɏ��s������false��Ԃ�
			return false;
		}

		// ���K�����ꂽ�l(0.0f�`1.0f�̒l)����\������Ă���l�ɂ���֐�
		ParamValue MyParameter::toPlain(ParamValue valueNormalized) const
		{
			// ���K���l��\���l�ɕϊ����ĕԂ�
			// toNormalized()�֐���toPlain()�֐��ŉt�ł��邱�Ƃ��K�v
			// �w���֐��Ƃ��Ă͂Ƃ肠����valueNormalized��4�悷��֐��ɂ��Ă���(�K��)
			// ���킹�āA�ŏ��l�`�ő�l�͈̔͂Ɏ��߂�悤�v�Z����B
			return ((max - min) * pow(valueNormalized, 4.0f)) + min;
		}

		// �\������Ă���l���琳�K�����ꂽ�l(0.0f�`1.0f�̒l)�ɂ���֐�
		ParamValue MyParameter::toNormalized(ParamValue plainValue) const
		{
			// �\���l�𐳋K���l�ɕϊ����ĕԂ�
			// toNormalized()�֐���toPlain()�֐��ŉt�ł��邱�Ƃ��K�v
			// toPlain()�֐��ł�4�悵�Ă���̂ŁAplainValue��4�捪���v�Z����
			return pow((plainValue - min) / (max - min), 1.0f / 4.0f);
		}

	}
} // namespace Steinberg��Vst�̏I���
