#pragma once

#include "Mof.h"

// 移動可能範囲の半分
#define FIELD_HALF_X (9.0f)
#define FIELD_HALF_Z (14.0f)

/********************** Player **********************/
// 移動速度
#define PLAYER_SPEED		(0.1f)

// 移動速度
#define PLAYERSHOT_SPEED	(0.4f)

// 弾の発射限界数
#define PLAYERSHOT_COUNT	(40)

// 弾の発射間隔
#define PLAYERSHOT_WAIT		(5)

// 弾の発射角度
#define WIDE_RAD			(0.05f)


enum PlayerShotMode {
	MODE_SINGLE,
	MODE_DOUBLE,
	MODE_TRIPPLE,
};

enum PlayerShotSubMode {
	MODE_DIRECT,
	MODE_WIDE,
};

//イージングの種類
enum EASING_TYPE {
	EASE_LINEAR,
	EASE_IN_SINE,
	EASE_OUT_SINE,
	EASE_INOUT_SINE,
};

//アニメーション用構造体
struct ANIM_DATA {
	float			Time;
	float			Value;
	EASING_TYPE		EasingType;
};

float InterpolationAnim(float AnimTime, ANIM_DATA* AnimData, int cnt);