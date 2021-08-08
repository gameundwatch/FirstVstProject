#pragma once
// VST3 SDK�̃C���N���[�h�t�@�C��
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/base/ibstream.h"
#include "base/source/fstreamer.h"

// ����VST�p�̃C���N���[�h�t�@�C��
#include "FirstVSTdefine.h"


// VST3�쐬�ɕK�v�Ȗ��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {


		// ===================================================================================
		// VST�̃p�����[�^�[�𑀍삷�邽�߂�Controller�N���X
		// ===================================================================================
		class MyVSTController : public EditController
		{
		public:
			// �N���X������������֐�(�K�{)
			tresult PLUGIN_API initialize(FUnknown* context);

			// Processor�N���X�̕ۑ���Ԃ𔽉f������֐�
			tresult PLUGIN_API setComponentState(IBStream* state);

			// ����VST Controller�N���X�̃C���X�^���X���쐬���邽�߂̊֐�(�K�{)
			static FUnknown* createInstance(void*) { return (IEditController*)new MyVSTController(); }

		};


	}
} // namespace Steinberg��Vst�̏I���