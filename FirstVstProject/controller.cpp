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

				RangeParameter* param1 = new RangeParameter(STR16("Gain"), PARAM_PREGAIN_TAG, STR16(""), 0.0f, 10.0f, 1.0f);
				param1->setPrecision(2);
				parameters.addParameter(param1);

				// 自作パラメーターを作成
				MyParameter* param2 = new MyParameter(STR16("Freq"), PARAM_FILTERFREQ_TAG, STR16("Hz"), 20.0f, 22000.0f, 22000.0f);
				param2->setPrecision(2); // 小数第何位まで表示するか
				parameters.addParameter(param2);

				// 範囲パラメーターを作成
				RangeParameter* param3 = new RangeParameter(STR16("Q"), PARAM_FILTERQ_TAG, STR16(""), 0.5f, 12.0f, 0.5f);
				parameters.addParameter(param3);

				// 今回は何もしない
			}

			// 初期化が成功すればkResultTrueを返す。
			result = kResultTrue;
			return result;
		}


	}
} // namespace SteinbergとVstの終わり