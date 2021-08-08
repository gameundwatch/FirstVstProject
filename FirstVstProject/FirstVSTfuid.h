#pragma once
// VST3 SDKのインクルードファイル
#include "pluginterfaces\base\funknown.h"

// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// FUIDの生成
		static const FUID ProcessorUID(0xAE98D68E, 0xCAAC444B, 0xB87F716E, 0xBFFFFECE);
		static const FUID ControllerUID(0xCE886047, 0xDEB04573, 0x89490975, 0xEE1E3046);
	}
} // namespace SteinbergとVstの終わり