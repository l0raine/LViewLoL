#pragma once
#include <string>
#include <map>
#include "Vector.h"
#include "windows.h"
#include "MemoryLoadable.h"

/* 
	This beautiful thing is the object encoding enum. Everything about the game object type 
	is compressed in a single integer. First we have the categories that set the bits of the
	integer from 2^6 to 2^32. The bits below 2^6 are equivalent of the id of a specific object

	Example BARON = JUNGLE | OBJECTIVE | SMITABLE + 6 is part of categories JUNGLE, OBJECTIVE
	and SMITABLE and has the object index 6. The object index must be unique or we could risk
	collisions.
	
	Use IsOfTypes to check if an object has any of the categories provided
	e.g baronObj.IsOfType(SMITABLE, JUNGLE) will return true.
*/
enum GameObjectType {
	NO_OBJ              = 0,
	INVISIBLE           = (1 << 7),
	MINION              = (1 << 8),
	JUNGLE              = (1 << 9),
	OBJECTIVE           = (1 << 10),
	DRAGON              = (1 << 11),
	SMITABLE            = (1 << 12),
	PLANT               = (1 << 13),
	PLAYER              = (1 << 14),
	TURRET              = (1 << 15),
	EXPIRABLE           = (1 << 16),
	CLONE               = (1 << 17),

	WARD                = INVISIBLE | EXPIRABLE + 1,
	WARD_PINK           = INVISIBLE + 2,

	MINION_CANNON       = MINION + 3,
	MINION_MELEE        = MINION + 4,
	MINION_RANGED       = MINION + 5,

	BARON               = JUNGLE | OBJECTIVE | SMITABLE + 6,
	HERALD              = JUNGLE | OBJECTIVE | SMITABLE + 7,
	
	DRAGON_FIRE         = JUNGLE | OBJECTIVE | DRAGON | SMITABLE + 8,
	DRAGON_MOUNTAIN     = JUNGLE | OBJECTIVE | DRAGON | SMITABLE + 9,
	DRAGON_OCEAN        = JUNGLE | OBJECTIVE | DRAGON | SMITABLE + 10,
	DRAGON_AIR          = JUNGLE | OBJECTIVE | DRAGON | SMITABLE + 11,
	DRAGON_ELDER        = JUNGLE | OBJECTIVE | DRAGON | SMITABLE + 12,

	KRUG                = JUNGLE | SMITABLE + 13,
	KRUG_MEDIUM         = JUNGLE + 14,
	KRUG_SMALL          = JUNGLE + 15,

	WOLF                = JUNGLE | SMITABLE + 16,
	WOLF_SMALL          = JUNGLE + 17,

	RAZORBEAK           = JUNGLE | SMITABLE + 18,
	RAZORBEAK_SMALL     = JUNGLE + 19,

	GROMP               = JUNGLE | SMITABLE + 20,
	BLUE                = JUNGLE | SMITABLE + 21,
	RED                 = JUNGLE | SMITABLE + 22,
	CRAB                = JUNGLE | SMITABLE + 23,
	PLANT_EXPLOSION     = JUNGLE | PLANT + 24, 
	PLANT_HEALING       = JUNGLE | PLANT + 25,
	PLANT_VISION        = JUNGLE | PLANT + 26,

	SHACO_BOX           = INVISIBLE | EXPIRABLE + 27,
	TEEMO_MUSHROOM      = INVISIBLE | EXPIRABLE + 28,

	SHACO_CLONE         = CLONE + 29,
	LEBLANC_CLONE       = CLONE + 30,
};

class GameObject: MemoryLoadable {

public:
	//GameObject() { buff = new BYTE[sizeBuff]; buffDeep = new BYTE[sizeBuffDeep]; }
	//~GameObject() { delete[] buff; delete[] buffDeep; }

	void           LoadFromMem(DWORD base, HANDLE hProcess, bool deepLoad = true);

	bool           IsOfOneType(const GameObjectType& type1) const;
	bool           IsOfTwoTypes(const GameObjectType& type1, const GameObjectType& type2) const;
	bool           IsOfThreeTypes(const GameObjectType& type1, const GameObjectType& type2, const GameObjectType& type3) const;
	bool           IsEqualTo(const GameObject& other) const;
	bool           IsNotEqualTo(const GameObject& other) const;

	float          GetAttackRange() const;
	bool           IsEnemyTo(const GameObject& other) const;
	bool           IsAllyTo(const GameObject& other) const;

	float          health;
	float          baseAttack;
	float          bonusAttack;
	float          armour;
	float          magicResist;
	float          crit;
	float          critMulti;
	float          abilityPower;
	float          bonusAbilityPower;
	float          atkSpeedMulti;

	float          baseAttackRange;
	float          baseAttackSpeed;
	float          maxAttackSpeed;

	bool           isAlive;

	std::string    name;
	Vector3        position;
	GameObjectType type;

	/* How close does the mouse cursor have to be to select the object */
	float          targetRadius;

	/* Basically the bounding radius of the model of the object */
	float          gameplayRadius;

	/* Team of the object 100 = Blue, 200 = Red, 300 = Jungle */
	short          team;

	/* In how many seconds the object expires */
	float          duration;

	/* True whenever the object is visible by the players team */
	bool           isVisible;

	/* Address in memory. Used for debug purposes */
	DWORD          address;

	/* The gameTime when this object was last visible */
	float          lastVisibleAt;

	/* Unique identifier of the object in game */
	short            objectIndex;

	DWORD            networkId;
protected:
	static const SIZE_T sizeBuff = 0x3000;
	static const SIZE_T sizeBuffDeep = 0x1000;

	static BYTE buff[sizeBuff];
	static BYTE buffDeep[sizeBuffDeep];

private:
	static std::map<std::string, GameObjectType>  gameObjectNameTypeDict;
};