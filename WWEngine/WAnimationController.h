#pragma once
#include "Common.h"
#include "skinnedMesh.h"
class WAnimationController {
	ID3DXAnimationController *		m_aniController;
	vector<string>					animations;

public:
	void GetAnimationsFromSkinnedMesh(SkinnedMesh mesh);
};
