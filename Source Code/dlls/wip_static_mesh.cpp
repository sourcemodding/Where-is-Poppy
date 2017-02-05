/******************************
Where is Poppy
February, 2017

wip_static_mesh.cpp

Static Mesh Loader for the mod
Where is Poppy

*******************************/

#include "wip_static_mesh.h"

// Need to Link our class to the name (wip_StaticMesh) that hammer will read from the FGD.
// This will be linked directly to the level as well so that the engine can link to it.
LINK_ENTITY_TO_CLASS(wip_StaticMesh, CStaticMesh);

///////////////////////////////
// Spawn(void)
//
// The Spawn function handles the creation and intialization of our entitty
// It is the second function to run in this Class
////////////////////////////////
void CStaticMesh::Spawn(void)
{
	// Precache and Load the model
	if (pev->model != 0){
		PRECACHE_MODEL((char *)STRING(pev->model));
		SET_MODEL(ENT(pev), STRING(pev->model));
	}
	// If the Model doesnt exist, print an error and set a default null.mdl as the model
	else{
		ALERT(at_console, "[wip_staticMesh] Error, Model Failed to load!\n");
		ALERT(at_console, "[wip_staticMesh] Setting model/null.mdl in its place!\n");
		PRECACHE_MODEL("models/null.mdl");
		SET_MODEL(ENT(pev), "models/null.mdl");
	}

	// Check if the Solid Flag is set and if so be sure to set it 
	// solid with appropriate Collisions
	// If not we also do not set a Collision Box because for a static mesh 
	// there is no reason to do so..
	if (FBitSet(pev->spawnflags, WIP_IS_SOLID))
	{
		// Set Model solid
		pev->solid = SOLID_BBOX;

		// Check the collision mode 0 = None, 1 = Manual, 2 = Sequence based
 		if (m_iCollisionMode == 1)
		{
			// Scale the collision box 
			mins = mins * m_flModelScale;
			maxs = maxs * m_flModelScale;

			// Set Collision box Size
			UTIL_SetSize(pev, mins, maxs);
		}
		else if (m_iCollisionMode == 2)
		{
			// Grab Bounding box size from current sequence
			ExtractBbox(m_iSequence, mins, maxs);

			// Sacle the collision box
			mins = mins * m_flModelScale;
			maxs = maxs * m_flModelScale;

			// Set Collision box Size
			UTIL_SetSize(pev, mins, maxs);
		}

		// Check if the bounding box Visualizer flag is set
		if (FBitSet(pev->spawnflags, WIP_DEBUG_BB)){
			m_bDebugBB = true; // If so set this value to true, we using it in Animate()
		}
	}

	// Set the visible meshes scale
	pev->scale = m_flModelScale;

	// Check if the User wants to animate
	if (FBitSet(pev->spawnflags, WIP_ANIMATE))
	{
		m_bAnimate = true; // Used in Animate()
		pev->sequence = m_iSequence; // Set the animation based on what the user set in the level
	}

	// Set our update/think function to Animate()
	SetThink(&CStaticMesh::Animate);
	// Set when in the future to update next
	pev->nextthink = gpGlobals->time + 0.01;
}

///////////////////////////////
// KeyValue(KeyValueData *pkvd)
//
// The KeyValue function imports values set by our level editor in our map
// These Keys are created in our FGD
// We set local variables to the values that the map returns when requested
// It is the first function to run in this Class
////////////////////////////////
void CStaticMesh::KeyValue(KeyValueData *pkvd)
{
	// Grab the speed our animation plays at
	// 0.0 here also stops the animation
	// A netagive value plays the animation in reverse
	// A higher value speeds up the animation
	if (FStrEq(pkvd->szKeyName, "animationspeed"))
	{
		m_flAnimationSpeed = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// In-Game version of editor only variable "sequence"
	// Set an integer to what sequence you want this model to play ingame
	else if (FStrEq(pkvd->szKeyName, "animate"))
	{
		m_iSequence = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// Set a mode for Collision
	// 0 = No Collision
	// 1 = Manual Mins & Maxs
	// 2 = Sequence Based Collision
	else if (FStrEq(pkvd->szKeyName, "collisionmode"))
	{
		m_iCollisionMode = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// Minimum Bounding box position
	else if (FStrEq(pkvd->szKeyName, "bbmins"))
	{
		UTIL_StringToVector(mins, pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// Maximum Bounding box position
	else if (FStrEq(pkvd->szKeyName, "bbmaxs"))
	{
		UTIL_StringToVector(maxs, pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// Set the scale of our model and collision boxes
	// In-Game version of the editor only "scale" keyword
	else if (FStrEq(pkvd->szKeyName, "modelscale"))
	{
		m_flModelScale = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	// defaults
	else
		CBaseEntity::KeyValue(pkvd);
}


///////////////////////////////
// Animate(void)
//
// The Animate function is basically the Update function of this Entitiy
// You add thinks here that you want to change on a frame by frame basis
// Things like animations
// Position changes
// Interactive code
////////////////////////////////
void CStaticMesh::Animate(void)
{
	// Set when in the future to next run the animate function
	pev->nextthink = gpGlobals->time + 0.01;

	// If animation is allowed
	if (m_bAnimate)
	{
		if (m_flAnimationSpeed >= 0.0)
		{
			// Ternary condition to update the models normal animation + any extra speed the user adds 
			pev->frame > 255 ? pev->frame = 0 : pev->frame += m_flAnimationSpeed;
		}
		else
		{
			// Ternary condition to update the models reverse animation + any extra speed the user adds
			pev->frame < 0 ? pev->frame = 255 : pev->frame += m_flAnimationSpeed;
		}
	}

	// Visualize the Collision Volume around the model
	if (m_bDebugBB){
		UTIL_RenderBBox(pev->origin, pev->mins, pev->maxs, 1, 0, 255, 0);
	}
}