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

				// ����p�����[�^�[���쐬
				MyParameter* param2 = new MyParameter(STR16("Freq"), PARAM_FILTERFREQ_TAG, STR16("Hz"), 20.0f, 22000.0f, 22000.0f);
				param2->setPrecision(2); // �����扽�ʂ܂ŕ\�����邩
				parameters.addParameter(param2);

				// �͈̓p�����[�^�[���쐬
				RangeParameter* param3 = new RangeParameter(STR16("Q"), PARAM_FILTERQ_TAG, STR16(""), 0.5f, 12.0f, 0.5f);
				parameters.addParameter(param3);
				
				// �t�B���^�[�^�C�v
				StringListParameter* param4 = new StringListParameter(STR16("Type"), PARAM_FILTERTYPE_TAG);
				param4->appendString(STR16("LPF"));  // ���X�g���ڂ�ǉ�
				param4->appendString(STR16("HPF"));
				param4->appendString(STR16("BPF"));
				// �����񃊃X�g�p�����[�^�[���R���g���[���[�ɒǉ�
				parameters.addParameter(param4);
				
				// PostGain
				RangeParameter* param991 = new RangeParameter(STR16("Gain"), PARAM_POSTGAIN_TAG, STR16(""), 0.0f, 10.0f, 1.0f);
				param991->setPrecision(2);
				parameters.addParameter(param991);

				// �ʑ����]
				StringListParameter* param997 = new StringListParameter(STR16("Invert"), PARAM_INV_TAG);
				param997->appendString(STR16("False"));
				param997->appendString(STR16("True"));
				parameters.addParameter(param997);

				// Mix
				RangeParameter* param999 = new RangeParameter(STR16("Mix"), PARAM_MIX_TAG, STR16(""), 0.0f, 1.0f, 1.0f);
				param999->setPrecision(2); // �����扽�ʂ܂ŕ\�����邩
				parameters.addParameter(param999);

				// ����͉������Ȃ�
			}

			// �����������������kResultTrue��Ԃ��B
			result = kResultTrue;
			return result;
		}

		tresult PLUGIN_API MyVSTController::setComponentState(IBStream* state)
		{
			if (!state) {
				return kResultFalse;
			}

			float savedFreq = 0.f;
			float savedQ = 0.f;
			float savedType = 0.f;
			float savedPostGain = 0.f;
			float savedInvert = 0.f;
			float savedMix = 0.f;

			IBStreamer streamer(state, kLittleEndian);
			if (!streamer.readFloat(savedFreq))
				return kResultFalse;
			if (!streamer.readFloat(savedQ))
				return kResultFalse;
			if (!streamer.readFloat(savedType))
				return kResultFalse;
			if (!streamer.readFloat(savedPostGain))
				return kResultFalse;
			if (!streamer.readFloat(savedInvert))
				return kResultFalse;
			if (!streamer.readFloat(savedMix))
				return kResultFalse;

			// �ǂݍ��܂ꂽ�f�[�^���p�����[�^�[�ɔ��f����
			// ���f����f�[�^��0.0�`1.0�͈̔͂ɂ��Ă���
			setParamNormalized(PARAM_FILTERFREQ_TAG, plainParamToNormalized(PARAM_FILTERFREQ_TAG, savedFreq));
			setParamNormalized(PARAM_FILTERQ_TAG, plainParamToNormalized(PARAM_FILTERQ_TAG, savedQ));
			setParamNormalized(PARAM_FILTERTYPE_TAG, plainParamToNormalized(PARAM_FILTERTYPE_TAG, savedType));
			setParamNormalized(PARAM_POSTGAIN_TAG, plainParamToNormalized(PARAM_POSTGAIN_TAG, savedPostGain));
			setParamNormalized(PARAM_INV_TAG, plainParamToNormalized(PARAM_INV_TAG, savedInvert));
			setParamNormalized(PARAM_MIX_TAG, plainParamToNormalized(PARAM_MIX_TAG, savedMix));

			// �֐��̏����ɖ�肪�Ȃ����kResultOk��Ԃ�
			return kResultOk;

		}

	}
} // namespace Steinberg��Vst�̏I���