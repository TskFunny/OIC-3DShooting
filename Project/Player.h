#pragma once

#include "GameDefine.h"
#include "PlayerShot.h"
#include "Enemy.h"

// à⁄ìÆë¨ìx
#define PLAYER_SPEED		(0.1f)

// íeÇÃî≠éÀå¿äEêî
#define PLAYERSHOT_COUNT	(40)

// íeÇÃî≠éÀä‘äu
#define PLAYERSHOT_WAIT		(5)
class CPlayer{
private:
	CMeshContainer	m_Mesh;
	CMeshContainer	m_ShotMesh;
	CPlayerShot		m_ShotArray[PLAYERSHOT_COUNT];
	CVector3		m_Pos;
	int				m_HP;
	CVector3		m_ShotPos;
	float			m_RotZ;
	int				m_ShotWait;
	bool			m_ShotType;
	bool			m_bDead;

	PlayerShotMode	m_ShotMode;
	PlayerShotSubMode	m_SubMode;
public:
	CPlayer();
	~CPlayer();
	bool Load();
	void Initialize();
	void Update();
	void Render();
	void RenderDebugText();
	const CVector3 GetPosition(){ return m_Pos; }
	void Release();
	void UpdateSingleShot();
	void UpdateDoubleShot();
	void UpdateTrippleShot();
	void UpdateShotMode();
	void RenderDebug();
	CSphere GetSphere() { return CSphere(m_Pos, 0.4f); }
	void CollisionEnemy(CEnemy& ene);
	void CollisionEnemyShot(CEnemyShot& shot);
	void Damage(int dmg);
	bool IsDead() { return m_bDead; }
};