#include "PlayerShot.h"

/**
 * コンストラクタ
 */
CPlayerShot::CPlayerShot() :
m_pMesh(NULL),
m_Pos(0.0f,0.0f,0.0f),
m_bShow(false){
}

/**
 * デストラクタ
 */
CPlayerShot::~CPlayerShot(){
}

/**
 * 初期化
 */
void CPlayerShot::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_bShow = false;
}

/**
 * 発射
 */
void CPlayerShot::Fire(const Vector3& p,const Vector3& s,PlayerShotMode m){
	m_Pos = p;
	m_bShow = true;
}

/**
 * 更新
 */
void CPlayerShot::Update(void){
	if (!m_bShow)
	{
		return;
	}
	m_Pos.z += PLAYERSHOT_SPEED;
	if (FIELD_HALF_Z < m_Pos.z)
	{
		m_bShow = false;
	}
}

void CPlayerShot::UpdateMode() {
	switch (m_Mode)
	{
	case MODE_SINGLE:
		UpdateSingleMode();
		break;
	case MODE_DOUBLE:
		UpdateDoubleMode();
		break;
	case MODE_TRIPPLE:
		UpdateTrippleMode();
		break;
	}
}

void CPlayerShot::UpdateSingleMode() {
}

void CPlayerShot::UpdateDoubleMode() {
}

void CPlayerShot::UpdateTrippleMode() {
}


/**
 * 描画
 */
void CPlayerShot::Render(void){
	if (!m_bShow)
	{
		return;
	}
	CMatrix44 wMat;
	wMat.Translation(m_Pos);
	m_pMesh->Render(wMat);
}

/**
 * デバッグ描画
 */
void CPlayerShot::RenderDebug(void) {
	if (!m_bShow)
	{
		return;
	}
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
}
