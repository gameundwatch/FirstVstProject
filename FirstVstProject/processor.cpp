// ����VST�p�̃C���N���[�h�t�@�C��
#include "FirstVSTDefine.h"
#include "FirstVSTfuid.h"
#include "processor.h"


// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		// =================================================================================
		// �R���X�g���N�^
		// =================================================================================
		MyVSTProcessor::MyVSTProcessor()
		{

			// �R���g���[���[��FUID��ݒ肷��
			setControllerClass(ControllerUID);
		}

		// ===================================================================================
		// �N���X������������֐�
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context)
		{
			// �܂��p�����N���X�̏����������{
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue)
			{
				// ���͂Əo�͂�ݒ�
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

				// ���͂�EventBus��ǉ�����
				addEventInput(STR16("Event Input"), 1);

				// �ȉ��ŗL�̏����������{�B
				// ����͉������Ȃ�

				postgain = 0.0f;

			}

			// �����������������kResultTrue��Ԃ��B
			return result;
		}

		tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
		{
			// input��output�̃o�X��1���ŁA����input��output�̍\�����X�e���I�̏ꍇ
			if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo)
			{
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}

			// �Ή����Ă��Ȃ��o�X�\���̏ꍇ�AkResultFalse��Ԃ��B
			return kResultFalse;
		}

		// ��Ԃ�Setter��Getter

		tresult PLUGIN_API MyVSTProcessor::setState(IBStream* state)
		{
			// ���݂�Processor�N���X�̏�Ԃ�Ǎ�
			if (!state) {
				return kResultFalse;
			}
			// �ۑ�����Ă���f�[�^��ǂݍ���
			float savedFreq = 1.0f;
			float savedQ = 0.5f;
			float savedType = 0.0f;
			float savedPostGain = 0.0f;
			float savedInvert = 0.0f;
			float savedMix = 1.0f;

			IBStreamer streamer(state, kLittleEndian);

			// �Ǎ��Ɏ��s�����ꍇ��kResultFalse��Ԃ��B
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

			freq = savedFreq;
			q = savedQ;
			type = savedType;
			postgain = savedPostGain;
			invert = savedInvert;
			mix = savedMix;

			// �֐��̏����ɖ�肪�Ȃ����kResultOk��Ԃ�
			return kResultOk;
		}

		tresult PLUGIN_API MyVSTProcessor::getState(IBStream* state)
		{
			// ���݂�Processor�N���X�̏�Ԃ�ۑ�
			if (!state) {
				return kResultFalse;
			}

			float	toSaveFreq = freq;
			float	toSaveQ = q;
			float	toSaveType = type;
			float	toSavePostGain = postgain;
			float	toSaveInvert = invert;
			float	toSaveMix = mix;

			IBStreamer streamer(state, kLittleEndian);

			// �f�[�^��ۑ�����

			streamer.writeFloat(toSaveFreq);
			streamer.writeFloat(toSaveQ);
			streamer.writeFloat(toSaveType);
			streamer.writeFloat(toSavePostGain);
			streamer.writeFloat(toSaveInvert);
			streamer.writeFloat(toSaveMix);

			// �֐��̏����ɖ�肪�Ȃ����kResultOk��Ԃ�
			return kResultOk;
		}

		// ===================================================================================
		// �����M������������֐�
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data)
		{
			// �p�����[�^�[�ύX�̏���
			// �^����ꂽ�p�����[�^�[������Ƃ��Adata��inputParameterChanges��
			// IParameterChanges�N���X�ւ̃|�C���^�̃A�h���X������
			if (data.inputParameterChanges != NULL)
			{
				// �^����ꂽ�p�����[�^�[�̐����擾
				int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

				// �^����ꂽ�p�����[�^�[���A�������J��Ԃ��B
				for (int32 i = 0; i < paramChangeCount; i++)
				{
					// �p�����[�^�[�ύX�̃L���[���擾
					// (��������T���v�����ɕ����̃p�����[�^�[�ύX��񂪂���\�������邽�߁A
					// �L���[�Ƃ����`�ɂȂ��Ă���B)
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
					if (queue != NULL)
					{
						// �ǂ̃p�����[�^�[���ύX���ꂽ���m�邽�߁A�p�����[�^�[tag���擾
						int32 tag = queue->getParameterId();

						// �ύX���ꂽ�񐔂��擾
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						// �Ō�ɕύX���ꂽ�l���擾
						if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue)
						{
							// tag�ɉ��������������{
							switch (tag)
							{
							case PARAM_POSTGAIN_TAG:
								// Gain��ύX����
								// 
								postgain = 10.0f * pow(value, 3.0f);
								break;
							case PARAM_FILTERFREQ_TAG:
								// Freq��ύX����
								// �v���Z�b�T�N���X�ɓn�����Ƃ��͐��K��(0.0�`1.0)���ꂽ�l�ƂȂ��Ă��܂�
								// ������MyParameter�ɉ������͈͂ɕύX����K�v������
								freq = ((22000.0f - 20.0f) * pow(value, 4.0f)) + 20.0f;
								break;
							case PARAM_FILTERQ_TAG:
								// q��ύX����B
								// ���l��RangeParameter�ɉ������͈͂�ݒ肷��K�v������B
								q = ((12.0f - 0.5f) * value) + 0.5f; // 0.5�`12.0�̊ԂɕύX
								break;

							case PARAM_FILTERTYPE_TAG:
								type = (int32)(value * 2.0f);
								break;

							case PARAM_INV_TAG:
								invert = (int32)value;
								break;

							case PARAM_MIX_TAG:
								mix = value;
								break;
							}

							switch ((int32)type)
							{
							case 0: // LPF
								// �ύX���ꂽ�p�����[�^�ɉ����ăt�B���^�W�����Čv�Z����
								filterL.LowPassGate(freq, q);
								filterR.LowPassGate(freq, q);
								break;
							case 1: // HPF
								// �ύX���ꂽ�p�����[�^�ɉ����ăt�B���^�W�����Čv�Z����
								filterL.HiPassGate(freq, q);
								filterR.HiPassGate(freq, q);
								break;
							case 2: // BPF
								// �ύX���ꂽ�p�����[�^�ɉ����ăt�B���^�W�����Čv�Z����
								filterL.BandPassGate(freq, q);
								filterR.BandPassGate(freq, q);
								break;
							}

						}
					}
				}
				// ���́E�o�̓o�b�t�@�̃|�C���^���킩��₷���ϐ��Ɋi�[
				// inputs[]�Aoutputs[]��AudioBus�̐���������(addAudioInput()�AaddAudioOutput()�Œǉ�����������)
				// �����AudioBus��1�����Ȃ̂� 0 �݂̂ƂȂ�
				// channelBuffers32��32bit���������_�^�̃o�b�t�@�ŉ����M���̃`�����l����������
				// ���m����(kMono)�Ȃ� 0 �݂̂ŁA�X�e���I(kStereo)�Ȃ� 0(Left) �� 1(Right) �ƂȂ�
				Sample32* inL = data.inputs[0].channelBuffers32[0];
				Sample32* inR = data.inputs[0].channelBuffers32[1];
				Sample32* outL = data.outputs[0].channelBuffers32[0];
				Sample32* outR = data.outputs[0].channelBuffers32[1];

				// numSamples�Ŏ������T���v�����A��������������
				for (int32 i = 0; i < data.numSamples; i++) {

					outL[i] = filterL.Process(inL[i]);
					outR[i] = filterR.Process(inR[i]);

					outL[i] = postgain * outL[i];
					outR[i] = postgain * outR[i];

					if (invert == 1) {
						outL[i] *= -1.0f;
						outR[i] *= -1.0f;
					}

					outL[i] = outL[i] * mix + inL[i] * (1.0 - mix);
					outR[i] = outR[i] * mix + inR[i] * (1.0 - mix);

				}
				// ���Ȃ����kResultTrue��Ԃ�(�����炭�K��kResultTrue��Ԃ�)
				return kResultTrue;
			}

		}
	}
}// namespace Steinberg��Vst�̏I���