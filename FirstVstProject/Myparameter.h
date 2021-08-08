#ifndef __CMYPARAM_H__
#define __CMYPARAM_H__

// VST3 SDK�̃C���N���[�h�t�@�C��
#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/base/ustring.h"
#include "public.sdk/source/vst/vsteditcontroller.h"

// ����VST�p�̃C���N���[�h�t�@�C��
#include <cmath>

// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		// ============================================================================================
		// �w���I�ȕω������鎩��p�����[�^�[�N���X
		// ============================================================================================
		class MyParameter : public Parameter
		{
		private:
			ParamValue min; // �p�����[�^�[�̕\���l�̍ŏ�
			ParamValue max; // �p�����[�^�[�̕\���l�̍ő�

		public:
			// �R���X�g���N�^(�p���������o�[�ϐ��Ȃǂ̏��������s�����߂قڕK�{)
			MyParameter(const TChar* title, ParamID tag, const TChar* units = 0, ParamValue minPlain = 0.0,
				ParamValue maxPlain = 1.0, ParamValue defaultValuePlain = 0.0,
				int32 flags = ParameterInfo::kCanAutomate, UnitID unitID = kRootUnitId);

			// ���K�����ꂽ�l(0.0f�`1.0f�̒l)��\���l�̕�����ɂ���֐�(�K�{)
			virtual void toString(ParamValue valueNormalized, String128 string) const;

			// �\������Ă��镶���񂩂琳�K�����ꂽ�l(0.0f�`1.0f�̒l)���擾����֐�(�K�{)
			virtual bool fromString(const TChar* string, ParamValue& valueNormalized) const;

			// ���K�����ꂽ�l(0.0f�`1.0f�̒l)����\������Ă���l�ɂ���֐�(�K�{)
			virtual ParamValue toPlain(ParamValue valueNormalized) const;

			// �\������Ă���l���琳�K�����ꂽ�l(0.0f�`1.0f�̒l)�ɂ���֐�(�K�{)
			virtual ParamValue toNormalized(ParamValue plainValue) const;

			// Parameter�N���X�̊e��ݒ������p�����[�^�[�N���X�u��������}�N��(�K�{)
			OBJ_METHODS(MyParameter, Parameter)

		};

	}
} // namespace Steinberg��Vst�̏I���

#endif