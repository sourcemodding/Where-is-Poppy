/******************************
Where is Poppy
February, 2017

wip_static_mesh.h

Static Mesh Loader for the mod
Where is Poppy

*******************************/

#ifndef WIP_STATIC_MESH_H
#define WIP_STATIC_MESH_H

#include "extdll.h"		// Required for KeyValueData
#include "util.h"		// Required Consts & Macros
#include "cbase.h"		// Required for CPointEntity

class CStaticMesh : public	CBaseAnimating
{
private:
	void Spawn(void);
	void EXPORT Animate(void);
	void KeyValue(KeyValueData *pkvd);

	Vector mins = { 0, 0, 0 },
		   maxs = { 0, 0, 0 };

	unsigned short m_iCollisionMode = 0,
				   m_iSequence = 0;

	bool m_bDebugBB = false,
		 m_bAnimate = false;

	float m_flAnimationSpeed = 1.0f,
		  m_fModelScale = 1.0f;

	#define WIP_IS_SOLID 1
	#define WIP_DEBUG_BB 2
	#define WIP_ANIMATE  4
};

#endif