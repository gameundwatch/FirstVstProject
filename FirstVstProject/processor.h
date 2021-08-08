#pragma once
// VST3 SDK�̃C���N���[�h�t�@�C��
#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/base/ibstream.h"
#include "base/source/fstreamer.h"

#include "FilterGate.h"

// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		// ===================================================================================
		// �����M������������Processor�N���X
		// ===================================================================================
		class MyVSTProcessor : public AudioEffect
		{
		protected:
			ParamValue freq;
			ParamValue q;
			ParamValue type;

			ParamValue postgain;
			ParamValue invert;
			ParamValue mix;

			FilterGate filterL, filterR; 
		public:
			// �R���X�g���N�^
			MyVSTProcessor();

			// �N���X������������֐�(�K�{)
			tresult PLUGIN_API initialize(FUnknown* context);

			// �o�X�\����ݒ肷��֐��B
			tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts);

			// Processor�N���X�̏�Ԃ̓Ǎ��ƕۑ�����֐�
			tresult PLUGIN_API getState(IBStream* state);
			tresult PLUGIN_API setState(IBStream* state);

			// �����M������������֐�(�K�{)
			tresult PLUGIN_API process(ProcessData& data);

			// ����VST Processor�N���X�̃C���X�^���X���쐬���邽�߂̊֐�(�K�{)
			static FUnknown* createInstance(void*) { return (IAudioProcessor*)new MyVSTProcessor(); }
		};


	}
} // namespace Steinberg��Vst�̏I���