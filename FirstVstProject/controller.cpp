// ����VST�p�̃C���N���[�h�t�@�C��
#include "FirstVSTdefine.h"
#include "FirstVSTfuid.h"
#include "controller.h"
#include "Myparameter.h"


// VST3�쐬�ɕK�v�Ȗ��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {


		// �N���X������������֐�(�K�{)
		tresult PLUGIN_API MyVSTController::initialize(FUnknown* context)
		{
			// �܂��p�����N���X�̏����������{
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
			{

				// �ȉ��ŗL�̏����������{�B

				RangeParameter* param1 = new RangeParameter(STR16("Gain"), PARAM_PREGAIN_TAG, STR16(""), 0.0f, 10.0f, 1.0f);
				param1->setPrecision(2);
				parameters.addParameter(param1);

				// ����p�����[�^�[���쐬
				MyParameter* param2 = new MyParameter(STR16("Freq"), PARAM_FILTERFREQ_TAG, STR16("Hz"), 20.0f, 22000.0f, 22000.0f);
				param2->setPrecision(2); // �����扽�ʂ܂ŕ\�����邩
				parameters.addParameter(param2);

				// �͈̓p�����[�^�[���쐬
				RangeParameter* param3 = new RangeParameter(STR16("Q"), PARAM_FILTERQ_TAG, STR16(""), 0.5f, 12.0f, 0.5f);
				parameters.addParameter(param3);

				// ����͉������Ȃ�
			}

			// �����������������kResultTrue��Ԃ��B
			result = kResultTrue;
			return result;
		}


	}
} // namespace Steinberg��Vst�̏I���