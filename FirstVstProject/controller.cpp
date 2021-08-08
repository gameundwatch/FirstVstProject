// 自作VST用のインクルードファイル
#include "FirstVSTdefine.h"
#include "FirstVSTfuid.h"
#include "controller.h"
#include "Myparameter.h"


// VST3作成に必要な名前空間を使用
namespace Steinberg {
	namespace Vst {


		// クラスを初期化する関数(必須)
		tresult PLUGIN_API MyVSTController::initialize(FUnknown* context)
		{
			// まず継承元クラスの初期化を実施
			tresult result = EditController::initialize(context);
			if (result == kResultTrue)
			{

				// 以下固有の初期化を実施。

				// 自作パラメーターを作成
				MyParameter* param2 = new MyParameter(STR16("Freq"), PARAM_FILTERFREQ_TAG, STR16("Hz"), 20.0f, 22000.0f, 22000.0f);
				param2->setPrecision(2); // 小数第何位まで表示するか
				parameters.addParameter(param2);

				// 範囲パラメーターを作成
				RangeParameter* param3 = new RangeParameter(STR16("Q"), PARAM_FILTERQ_TAG, STR16(""), 0.5f, 12.0f, 0.5f);
				parameters.addParameter(param3);
				
				// フィルタータイプ
				StringListParameter* param4 = new StringListParameter(STR16("Type"), PARAM_FILTERTYPE_TAG);
				param4->appendString(STR16("LPF"));  // リスト項目を追加
				param4->appendString(STR16("HPF"));
				param4->appendString(STR16("BPF"));
				// 文字列リストパラメーターをコントローラーに追加
				parameters.addParameter(param4);
				
				// PostGain
				RangeParameter* param991 = new RangeParameter(STR16("Gain"), PARAM_POSTGAIN_TAG, STR16(""), 0.0f, 10.0f, 1.0f);
				param991->setPrecision(2);
				parameters.addParameter(param991);

				// 位相反転
				StringListParameter* param997 = new StringListParameter(STR16("Invert"), PARAM_INV_TAG);
				param997->appendString(STR16("False"));
				param997->appendString(STR16("True"));
				parameters.addParameter(param997);

				// Mix
				RangeParameter* param999 = new RangeParameter(STR16("Mix"), PARAM_MIX_TAG, STR16(""), 0.0f, 1.0f, 1.0f);
				param999->setPrecision(2); // 小数第何位まで表示するか
				parameters.addParameter(param999);

				// 今回は何もしない
			}

			// 初期化が成功すればkResultTrueを返す。
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

			// 読み込まれたデータをパラメーターに反映する
			// 反映するデータは0.0～1.0の範囲にしておく
			setParamNormalized(PARAM_FILTERFREQ_TAG, plainParamToNormalized(PARAM_FILTERFREQ_TAG, savedFreq));
			setParamNormalized(PARAM_FILTERQ_TAG, plainParamToNormalized(PARAM_FILTERQ_TAG, savedQ));
			setParamNormalized(PARAM_FILTERTYPE_TAG, plainParamToNormalized(PARAM_FILTERTYPE_TAG, savedType));
			setParamNormalized(PARAM_POSTGAIN_TAG, plainParamToNormalized(PARAM_POSTGAIN_TAG, savedPostGain));
			setParamNormalized(PARAM_INV_TAG, plainParamToNormalized(PARAM_INV_TAG, savedInvert));
			setParamNormalized(PARAM_MIX_TAG, plainParamToNormalized(PARAM_MIX_TAG, savedMix));

			// 関数の処理に問題がなければkResultOkを返す
			return kResultOk;

		}

	}
} // namespace SteinbergとVstの終わり