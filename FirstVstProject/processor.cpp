// 自作VST用のインクルードファイル
#include "FirstVSTDefine.h"
#include "FirstVSTfuid.h"
#include "processor.h"


// VST3作成に必要なの名前空間を使用
namespace Steinberg {
	namespace Vst {

		// =================================================================================
		// コンストラクタ
		// =================================================================================
		MyVSTProcessor::MyVSTProcessor()
		{

			// コントローラーのFUIDを設定する
			setControllerClass(ControllerUID);
		}

		// ===================================================================================
		// クラスを初期化する関数
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::initialize(FUnknown* context)
		{
			// まず継承元クラスの初期化を実施
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue)
			{
				// 入力と出力を設定
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);

				// 入力のEventBusを追加する
				addEventInput(STR16("Event Input"), 1);

				// 以下固有の初期化を実施。
				// 今回は何もしない

				postgain = 0.0f;

			}

			// 初期化が成功すればkResultTrueを返す。
			return result;
		}

		tresult PLUGIN_API MyVSTProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
		{
			// inputとoutputのバスが1つずつで、かつinputとoutputの構成がステレオの場合
			if (numIns == 1 && numOuts == 1 && inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo)
			{
				return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
			}

			// 対応していないバス構成の場合、kResultFalseを返す。
			return kResultFalse;
		}

		// 状態のSetterとGetter

		tresult PLUGIN_API MyVSTProcessor::setState(IBStream* state)
		{
			// 現在のProcessorクラスの状態を読込
			if (!state) {
				return kResultFalse;
			}
			// 保存されているデータを読み込む
			float savedFreq = 1.0f;
			float savedQ = 0.5f;
			float savedType = 0.0f;
			float savedPostGain = 0.0f;
			float savedInvert = 0.0f;
			float savedMix = 1.0f;

			IBStreamer streamer(state, kLittleEndian);

			// 読込に失敗した場合はkResultFalseを返す。
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

			// 関数の処理に問題がなければkResultOkを返す
			return kResultOk;
		}

		tresult PLUGIN_API MyVSTProcessor::getState(IBStream* state)
		{
			// 現在のProcessorクラスの状態を保存
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

			// データを保存する

			streamer.writeFloat(toSaveFreq);
			streamer.writeFloat(toSaveQ);
			streamer.writeFloat(toSaveType);
			streamer.writeFloat(toSavePostGain);
			streamer.writeFloat(toSaveInvert);
			streamer.writeFloat(toSaveMix);

			// 関数の処理に問題がなければkResultOkを返す
			return kResultOk;
		}

		// ===================================================================================
		// 音声信号を処理する関数
		// ===================================================================================
		tresult PLUGIN_API MyVSTProcessor::process(ProcessData& data)
		{
			// パラメーター変更の処理
			// 与えられたパラメーターがあるとき、dataのinputParameterChangesに
			// IParameterChangesクラスへのポインタのアドレスが入る
			if (data.inputParameterChanges != NULL)
			{
				// 与えられたパラメーターの数を取得
				int32 paramChangeCount = data.inputParameterChanges->getParameterCount();

				// 与えられたパラメーター分、処理を繰り返す。
				for (int32 i = 0; i < paramChangeCount; i++)
				{
					// パラメーター変更のキューを取得
					// (処理するサンプル内に複数のパラメーター変更情報がある可能性があるため、
					// キューという形になっている。)
					IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
					if (queue != NULL)
					{
						// どのパラメーターが変更されたか知るため、パラメーターtagを取得
						int32 tag = queue->getParameterId();

						// 変更された回数を取得
						int32 valueChangeCount = queue->getPointCount();
						ParamValue value;
						int32 sampleOffset;

						// 最後に変更された値を取得
						if (queue->getPoint(valueChangeCount - 1, sampleOffset, value) == kResultTrue)
						{
							// tagに応じた処理を実施
							switch (tag)
							{
							case PARAM_POSTGAIN_TAG:
								// Gainを変更する
								// 
								postgain = 10.0f * pow(value, 3.0f);
								break;
							case PARAM_FILTERFREQ_TAG:
								// Freqを変更する
								// プロセッサクラスに渡されるときは正規化(0.0～1.0)された値となってしまう
								// 自分でMyParameterに応じた範囲に変更する必要がある
								freq = ((22000.0f - 20.0f) * pow(value, 4.0f)) + 20.0f;
								break;
							case PARAM_FILTERQ_TAG:
								// qを変更する。
								// 同様にRangeParameterに応じた範囲を設定する必要がある。
								q = ((12.0f - 0.5f) * value) + 0.5f; // 0.5～12.0の間に変更
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
								// 変更されたパラメータに応じてフィルタ係数を再計算する
								filterL.LowPassGate(freq, q);
								filterR.LowPassGate(freq, q);
								break;
							case 1: // HPF
								// 変更されたパラメータに応じてフィルタ係数を再計算する
								filterL.HiPassGate(freq, q);
								filterR.HiPassGate(freq, q);
								break;
							case 2: // BPF
								// 変更されたパラメータに応じてフィルタ係数を再計算する
								filterL.BandPassGate(freq, q);
								filterR.BandPassGate(freq, q);
								break;
							}

						}
					}
				}
				// 入力・出力バッファのポインタをわかりやすい変数に格納
				// inputs[]、outputs[]はAudioBusの数だけある(addAudioInput()、addAudioOutput()で追加した分だけ)
				// 今回はAudioBusは1つだけなので 0 のみとなる
				// channelBuffers32は32bit浮動小数点型のバッファで音声信号のチャンネル数分ある
				// モノラル(kMono)なら 0 のみで、ステレオ(kStereo)なら 0(Left) と 1(Right) となる
				Sample32* inL = data.inputs[0].channelBuffers32[0];
				Sample32* inR = data.inputs[0].channelBuffers32[1];
				Sample32* outL = data.outputs[0].channelBuffers32[0];
				Sample32* outR = data.outputs[0].channelBuffers32[1];

				// numSamplesで示されるサンプル分、音声を処理する
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
				// 問題なければkResultTrueを返す(おそらく必ずkResultTrueを返す)
				return kResultTrue;
			}

		}
	}
}// namespace SteinbergとVstの終わり