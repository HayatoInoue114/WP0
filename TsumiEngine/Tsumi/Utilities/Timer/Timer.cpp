#include "Timer.h"



// スタート時に呼び出す処理
void Timer::Start(float setStart, float setEnd)
{
	// 既にスタートしていたら早期return
	if (isActive_) {
		return;
	}

	nowFrame_ = setStart;
	endFrame_ = setEnd;

	saveTimer_ = { setStart, setEnd };

	frameRatio_ = nowFrame_ / endFrame_;

	isActive_ = true;
	isFinish_ = false;
}


// タイマーの更新処理
void Timer::Update(bool roop)
{
	if (isFinish_) {
		if (roop) {
			this->Start(saveTimer_.first, saveTimer_.second);
			return;
		}
		isActive_ = false;
		return;
	}

	nowFrame_++;
	frameRatio_ = nowFrame_ / endFrame_;

	if (nowFrame_ >= endFrame_) {
		isActive_ = false;
		isFinish_ = true;
	}
}


// リセットみたいなやつ
void Timer::Clear()
{
	nowFrame_ = 0;
	endFrame_ = 0;
	frameRatio_ = 0.0f;
	isActive_ = false;
	isFinish_ = true;
}
