#include "Player.h"

/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
m_Mesh(),
m_Pos(0.0f,0.0f,0.0f),
m_RotZ(0.0f),
m_ShotPos(0.0f,0.0f,0.0f),
m_ShotMesh(),
m_ShotArray(),
m_ShotWait(),
m_ShotType(false){
}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
}

/**
 * �ǂݍ���
 */
bool CPlayer::Load(void){
	// ���b�V���̓ǂݍ���
	if (!m_Mesh.Load("player.mom"))
	{
		return false;
	}
	//�e�̃��b�V��
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
 * ������
 */
void CPlayer::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -FIELD_HALF_Z + 2.0f);
	m_ShotPos = Vector3(0.0f, 0.0f, 0.0f);
	m_RotZ = 0;
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Initialize();
	}
	m_ShotMode = MODE_DOUBLE;
}

/**
 * �X�V
 */
#define MAX_SKILLTIME 100
static int SkillTimer = 100;
void CPlayer::Update(void){
	//��]����
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

	//���ړ�
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

    //��]
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
		if (g_pInput->IsKeyHold(MOFKEY_P))
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
	
	// �e�̍X�V
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Update();
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
/**
 * �`��
 */
void CPlayer::Render(void){
	//���[���h�s��쐬
	CMatrix44 matWorld;
	matWorld.RotationZ(m_RotZ);
	matWorld.SetTranslation(m_Pos);
	// ���b�V���̕`��
	m_Mesh.Render(matWorld);
	for (int i = 0; i < SkillTimer;i++)
	{
		CGraphicsUtilities::RenderString(800 + i * 2, 0, MOF_COLOR_BLUE, "i");
	}
	// �e�̕`��
	for (int i = 0; i < PLAYERSHOT_COUNT; i++)
	{
		m_ShotArray[i].Render();
	}
}

/**
 * �f�o�b�O�����`��
 */
void CPlayer::RenderDebugText(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_XRGB(0,0,0),
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
	
	
}

/**
 * ���
 */
void CPlayer::Release(void){
	m_Mesh.Release();
	m_ShotMesh.Release();
}