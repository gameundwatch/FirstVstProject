#pragma once
// VST3 SDK�̃C���N���[�h�t�@�C��
#include "pluginterfaces\base\funknown.h"

// VST3�쐬�ɕK�v�Ȃ̖��O��Ԃ��g�p
namespace Steinberg {
	namespace Vst {

		// FUID�̐���
		static const FUID ProcessorUID(0xAE98D68E, 0xCAAC444B, 0xB87F716E, 0xBFFFFECE);
		static const FUID ControllerUID(0xCE886047, 0xDEB04573, 0x89490975, 0xEE1E3046);
	}
} // namespace Steinberg��Vst�̏I���