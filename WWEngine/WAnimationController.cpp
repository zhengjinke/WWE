#include "WAnimationController.h"

void WAnimationController::GetAnimationsFromSkinnedMesh(SkinnedMesh mesh)
{
	mesh.GetAnimations(this->animations);
}
