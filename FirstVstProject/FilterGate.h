#pragma once

#define BUF_LENGTH 44100

#include <math.h>

#include "CMyfilter.h"

// --------------------------------------------------------------------------------
// LowPassGate
// --------------------------------------------------------------------------------
class FilterGate
{
private:
	// フィルタの係数
	float a0, a1, a2, b0, b1, b2;
	// ゲート
	float threshold;
	// 減衰
	float decay;

	// バッファ
	float out1, out2;
	float in1, in2;


public:
	inline FilterGate();

	// 入力信号にフィルタを適用する関数
	inline float Process(float in);

	// ゲート処理関数
	inline float Gate(float in);

	// フィルタ係数を計算するメンバー関数
	inline void LowPassGate(float freq, float q, float samplerate = 44100.0f);
	inline void HiPassGate(float freq, float q, float samplerate = 44100.0f);
	inline void BandPassGate(float freq, float q, float samplerate = 44100.0f);

};

// --------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------
FilterGate::FilterGate()
{
	// メンバー変数を初期化
	a0 = 1.0f; // 0以外にしておかないと除算でエラーになる
	a1 = 0.0f;
	a2 = 0.0f;
	b0 = 1.0f;
	b1 = 0.0f;
	b2 = 0.0f;

	threshold = 0.0f;
	decay = 0.5f;

	in1 = 0.0f;
	in2 = 0.0f;

	out1 = 0.0f;
	out2 = 0.0f;
	
}

// --------------------------------------------------------------------------------
// 入力信号にフィルタを適用する関数
// --------------------------------------------------------------------------------
float FilterGate::Process(float in)
{
	// 入力信号にフィルタを適用し、出力信号変数に保存。
	float out = b0 / a0 * in + b1 / a0 * in1 + b2 / a0 * in2
		- a1 / a0 * out1 - a2 / a0 * out2;

	in2 = in1; // 2つ前の入力信号を更新
	in1 = in;  // 1つ前の入力信号を更新

	out2 = out1; // 2つ前の出力信号を更新
	out1 = out;  // 1つ前の出力信号を更新

	// ゲート処理
	// gate値より低い場合信号をOFFに
	out = Gate(out);
	
	// 出力信号を返す
	return out;
}

// --------------------------------------------------------------------------------
// ゲート処理関数
// --------------------------------------------------------------------------------

float FilterGate::Gate(float in) {

	// 
	float ratio = 0.01f;

	// ローパスフィルターを設定
	static CMyFilter envfilter;   // 音圧を検知するために使うローパスフィルタ
	static CMyFilter gainfilter; // 急激な音量変化を避けるためのローパスフィルタ
	// カットオフ周波数が高いほど音圧変化に敏感になる。目安は10～50Hz程度
	envfilter.LowPass(30.0f, 1.0);
	// カットオフ周波数が高いほど急激な音量変化になる。目安は5～50Hz程度
	gainfilter.LowPass(5.0f, 1.0);

	float gain = 1.0f;

	// 入力信号の絶対値をとったものをローパスフィルタにかけて音圧を検知する
	float tmp = envfilter.Process(abs(in));
	if (tmp < threshold)
	{
		gain = gainfilter.Process(tmp) * ratio;
	}
	// 音量(ゲイン)が急激に変化しないようローパスフィルタを通す
	gain = gainfilter.Process(gain);

	return gain * in;

}

// --------------------------------------------------------------------------------
// フィルタ係数を計算するメンバー関数
// --------------------------------------------------------------------------------

void FilterGate::LowPassGate(float freq, float q, float samplerate)
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// フィルタ係数を求める。
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = (1.0f - cos(omega)) / 2.0f;
	b1 = 1.0f - cos(omega);
	b2 = (1.0f - cos(omega)) / 2.0f;

	// 周波数からゲートしきい値のセット
	// 低周波になればゲートのしきい値が上がるようになるため
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}

void FilterGate::HiPassGate(float freq, float q, float samplerate)
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// フィルタ係数を求める。
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = (1.0f + cos(omega)) / 2.0f;
	b1 = - (1.0f + cos(omega));
	b2 = (1.0f + cos(omega)) / 2.0f;

	// 周波数からゲートしきい値のセット
	// 低周波になればゲートのしきい値が上がるようになるため
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}

void FilterGate::BandPassGate(float freq, float q, float samplerate)
{
	// フィルタ係数計算で使用する中間値を求める。
	float omega = 2.0f * 3.14159265f * freq / samplerate;
	float alpha = sin(omega) / (2.0f * q);

	// フィルタ係数を求める。
	a0 = 1.0f + alpha;
	a1 = -2.0f * cos(omega);
	a2 = 1.0f - alpha;
	b0 = alpha;
	b1 = 0.0f;
	b2 = -alpha;

	// 周波数からゲートしきい値のセット
	// 低周波になればゲートのしきい値が上がるようになるため
	threshold = -1.0f * pow((freq - 20.0f) / (22000.0f - 20.0f), 1.0f / 4.0f) + 1.0f;

}