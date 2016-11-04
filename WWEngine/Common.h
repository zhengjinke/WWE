#pragma once


#define PXPHYSICS_ONLINE
#define	PHYSXDEBUG

#include "WDefine.h"
#include "WClass.h"
#include "Input.h"
#include <string>
#include "lib.h"
#include <iostream>
#include <vector>
#include <queue>

#include "PxPhysicsAPI.h"
#include "vehicle/PxVehicleSDK.h"

#include <PxDefaultErrorCallback.h>


struct ObjFreeHashNode {
	int idx;
	ObjFreeHashNode(int nidx) :idx(nidx) {}
	bool operator < (const ObjFreeHashNode &rhs)const {
		return idx > rhs.idx;
	}
};
