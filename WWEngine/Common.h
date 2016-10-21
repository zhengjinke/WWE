#pragma once

#include "PxPhysicsAPI.h"


#include "WDefine.h"
#include "WClass.h"
#include "Input.h"
#include <string>
#include "lib.h"
#include <iostream>
#include <vector>
#include <queue>
struct ObjFreeHashNode {
	int idx;
	ObjFreeHashNode(int nidx) :idx(nidx) {}
	bool operator < (const ObjFreeHashNode &rhs)const {
		return idx > rhs.idx;
	}
};
