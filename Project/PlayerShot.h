#pragma once

#include "GameDefine.h"

// ˆÚ“®‘¬“x
#define PLAYERSHOT_SPEED	(0.4f)

class CPlayerShot{
private:
	CMeshContainer*		m_pMesh;
	Vector3				m_Pos;
	Vector3				m_Speed;
	bool				m_bShow;
	PlayerShotMode		m_Mode;
public:
	CPlayerShot();
	~CPlayerShot();
	void Initialize();
	void Fire(const Vector3& p, const Vector3& s,PlayerShotMode m);
	void Update();
	void Render();
	void SetMesh(CMeshContainer *pm){ m_pMesh = pm; }
	bool GetShow(){ return m_bShow; }

	void UpdateMode();
	void UpdateSingleMode();
	void UpdateDoubleMode();
	void UpdateTrippleMode();
};