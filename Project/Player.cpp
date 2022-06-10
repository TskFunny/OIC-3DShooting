#include "Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_HP(),
m_RotZ(0.0f),
m_bDead(false),
m_ShotPos(0.0f,0.0f,0.0f),
m_ShotMesh(),
m_ShotArray(),
m_ShotWait(),
m_ShotType(false){
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
}

/**
 * 読み込み
 */
bool CPlayer::Load(void){
	// メッシュの読み込み
	if (!m_Mesh.Load("player.mom"))
	{
		return false;
	}
	//弾のメッシュ
	if (!m_ShotMesh.Load("pshot.mom"))
	{
		return false;
	}
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].SetMesh(&m_ShotMesh);
	}
	return true;
}

/**
 * 初期化
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_HP = PLAYER_MAX_HP;
	m_ShotPos = Vector3(0.0f, 0.0f, 0.0f);
	m_RotZ = 0;
	m_bDead = false;
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
	m_ShotMode = PlayerShotMode::MODE_DOUBLE;
	m_SubMode = PlayerShotSubMode::MODE_DIRECT;
}

/**
 * 更新
 */
#define MAX_SKILLTIME 100
static int SkillTimer = 100;
void CPlayer::Update(void){
	if (m_bDead)
	{
		return;
	}
	//回転方向
	float Roll = 0;
	float m_Speed = PLAYER_SPEED;
	float RotSpeed = MOF_ToRadian(10);

	
	if (g_pInput->IsKeyHold(MOFKEY_LSHIFT))
	{
		if (SkillTimer > 0)
		{
			m_Speed *= 2;
			RotSpeed *= 2;
			SkillTimer--;
		}		
	}
	else
	{
		if (MAX_SKILLTIME > SkillTimer)
		{
			SkillTimer++;
		}
	}

	//横移動
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_Pos.x = max(m_Pos.x - m_Speed, -FIELD_HALF_X);
		Roll -= MOF_MATH_PI;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_Pos.x = min(m_Pos.x + m_Speed, FIELD_HALF_X);
		Roll += MOF_MATH_PI;
	}

	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_Pos.z = min(m_Pos.z + m_Speed, FIELD_HALF_Z);
	}
	if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_Pos.z = max(m_Pos.z - m_Speed, -FIELD_HALF_Z);
	}

    //回転
	if (Roll == 0)
	{
		RotSpeed = min(abs(m_RotZ) * 0.1f, RotSpeed);
	}
	if (abs(m_RotZ) <= RotSpeed || signbit(m_RotZ) != signbit(Roll))
	{
		m_RotZ += Roll;
	}
	m_RotZ -= copysignf(min(RotSpeed, abs(m_RotZ)), m_RotZ);

	UpdateShotMode();

	if (m_ShotWait <= 0)
	{
		if (g_pInput->IsKeyHold(MOFKEY_SPACE))
		{
			switch (m_ShotMode) {
			case MODE_SINGLE:
				UpdateSingleShot();
				break;
			case MODE_DOUBLE:
				UpdateDoubleShot();
				break;
			case MODE_TRIPPLE:
				UpdateTrippleShot();
				break;
			}
		}
	}
	else
	{
		m_ShotWait--;
	}
	
	// 弾の更新
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Update();
	}
}

void CPlayer::UpdateShotMode() {
	if (g_pInput->IsKeyPush(MOFKEY_1))
	{
		m_ShotMode = MODE_SINGLE;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_2))
	{
		m_ShotMode = MODE_DOUBLE;
	}
	else if (g_pInput->IsKeyPush(MOFKEY_3))
	{
		m_ShotMode = MODE_TRIPPLE;
	}

	if (g_pInput->IsKeyPush(MOFKEY_RSHIFT)) {
		m_SubMode = (m_SubMode == MODE_DIRECT) ? MODE_WIDE : MODE_DIRECT;
	}
}

void CPlayer::UpdateSingleShot() {
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if (m_ShotArray[i].GetShow()) { continue; }
		m_ShotPos = Vector3(0.0f, 0.0f, 0.0f);
		m_ShotPos.RotationZ(m_RotZ);
		m_ShotPos += m_Pos;
		CVector3 spd(0, 0, PLAYERSHOT_SPEED);
		m_ShotWait = PLAYERSHOT_WAIT;
		m_ShotArray[i].Fire(m_ShotPos, spd, m_ShotMode);
		break;
	}
}

void CPlayer::UpdateDoubleShot() {
	for (int cnt = 0; cnt < 2; cnt++)
	{
		for (int i = 0; i < PLAYERSHOT_COUNT; i++)
		{
			if (m_ShotArray[i].GetShow()) { continue; }
			m_ShotPos = Vector3(0.4f * (cnt * 2 - 1), 0, 0);
			m_ShotPos.RotationZ(m_RotZ);
			m_ShotPos += m_Pos;
			CVector3 spd(0, 0, PLAYERSHOT_SPEED);
			if (m_SubMode == MODE_WIDE) {
				spd = Vector3(cnt * WIDE_RAD * 2 - WIDE_RAD, 0, PLAYERSHOT_SPEED);
			}
			m_ShotWait = PLAYERSHOT_WAIT;
			m_ShotArray[i].Fire(m_ShotPos, spd, m_ShotMode);
			break;
		}
	}
}

void CPlayer::UpdateTrippleShot() {
	for (int cnt = 0; cnt < 3; cnt++)
	{
		for (int i = 0; i < PLAYERSHOT_COUNT; i++)
		{
			if (m_ShotArray[i].GetShow()) { continue; }
			m_ShotPos = Vector3(0.4f * (cnt * 1 - 1), 0, 0);
			m_ShotPos.RotationZ(m_RotZ);
			m_ShotPos += m_Pos;
			CVector3 spd(0, 0, PLAYERSHOT_SPEED);
			if (m_SubMode == MODE_WIDE) {
				spd = Vector3(cnt * WIDE_RAD - WIDE_RAD, 0, PLAYERSHOT_SPEED);
			}
			m_ShotWait = PLAYERSHOT_WAIT;
			m_ShotArray[i].Fire(m_ShotPos,spd,m_ShotMode);
			break;
		}
	}
}

/**
 * 敵との当たり判定
 * 引数の敵に対して当たり判定を実行する
 * 
 * 引数
 * [in]				dmg				ダメージ
 */
void CPlayer::CollisionEnemy(CEnemy& ene) {
	if (!ene.GetShow())
	{
		return;
	}
	CSphere ps = GetSphere();
	CSphere es = ene.GetSphere();
	if (ps.CollisionSphere(es))
	{
		Damage(5);
		ene.Damage(5);
	}
	// 弾との判定
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		if (!m_ShotArray[i].GetShow())
		{
			continue;
		}
		CSphere ss = m_ShotArray[i].GetSphere();
		if (ss.CollisionSphere(es))
		{
			ene.Damage(1);
			m_ShotArray[i].SetShow(false);
			break;
		}
	}
}

/**
 * 敵弾との当たり判定
 * 引数の敵弾に対して当たり判定を実行する。
 * 
 * 引数
 * [in]			shot			判定を行う敵弾
 */
void CPlayer::CollisionEnemyShot(CEnemyShot& shot) {
	CSphere ps = GetSphere();
	if (!shot.GetShow())
	{
		return;
	}
	CSphere ss = shot.GetSphere();
	if (ss.CollisionSphere(ps))
	{
		Damage(1);
		shot.SetShow(false);
	}
}

/**
 * 描画
 */
void CPlayer::Render(void){
	if (m_bDead)
	{
		return;
	}
	//ワールド行列作成
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	// メッシュの描画
	m_Mesh.Render(matWorld);
	for (int i = 0; i < SkillTimer;i++)
	{
		CGraphicsUtilities::RenderString(800 + i * 2, 0, MOF_COLOR_BLUE, "i");
	}
	for (int i = 0; i < m_HP; i++)
	{
		CGraphicsUtilities::RenderString(800 + i * 2, 700, MOF_COLOR_GREEN, "l");
	}
	// 弾の描画
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**
 * デバッグ文字描画
 */
void CPlayer::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
	
	
}

/**
 * デバッグ描画
 */
void CPlayer::RenderDebug(void) {
	//当たり判定の表示
	CGraphicsUtilities::RenderSphere(GetSphere(), Vector4(0, 1, 0, 0.3f));
	//弾の描画
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].RenderDebug();
	}
}

void CPlayer::Damage(int dmg) {
	m_HP -= dmg;
	if (m_HP <= 0)
	{
		m_bDead = true;
	}
}

/**
 * 解放
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}