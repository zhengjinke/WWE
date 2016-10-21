#include "WCharactor.h"

WCharactor::WCharactor(char * szFile, char * szName, float x, float y, float z)
{
	m_mesh.Load(szFile);
	SetName(szName, sizeof(char)*(strlen(szName)+1));
	transform.SetPos(x, y, z);
	m_mesh.GetAnimations(animations);
	ShowAnimations();
	SetupCallBack();
}

void WCharactor::ShowAnimations()
{
	for (unsigned i = 0; i < animations.size(); i++) {
		std::cout <<"animation ["<<i<<"] is "<<animations[i] << endl;
	}
}

void WCharactor::Draw(IDirect3DDevice9*	g_pDevice)
{
	if (g_pDevice == NULL) {
		messagebox("Éè±¸¶ªÊ§");
		return;
	}
	m_mesh.SetPose(transform.identity);
	m_mesh.Render(NULL);
}

void WCharactor::SetupCallBack()
{
		//Get Animation controller
		m_aniController = m_mesh.GetController();

		//Get the "Aim" Animation set
		ID3DXKeyframedAnimationSet* animSet = NULL;
		m_aniController->GetAnimationSet(1, (ID3DXAnimationSet**)&animSet);

		//Compress the animation set
		ID3DXBuffer* compressedData = NULL;
		animSet->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, NULL, &compressedData);

		//Create one callback key
		UINT numCallbacks = 1;
		D3DXKEY_CALLBACK keys[1];

		// GetSourceTicksPerSecond() returns the number
		// animation key frame ticks that occur per second.
		// Callback keyframe times are tick based.
		double ticks = animSet->GetSourceTicksPerSecond();

		// Set the first callback key to trigger a callback
		// half way through the animation sequence.
		keys[0].Time = float(animSet->GetPeriod() / 2.0f*ticks);
		keys[0].pCallbackData = (void*)&m_mesh;

		// Create the ID3DXCompressedAnimationSet interface
		// with the callback keys.
		ID3DXCompressedAnimationSet* compressedAnimSet = NULL;
		D3DXCreateCompressedAnimationSet(animSet->GetName(),
			animSet->GetSourceTicksPerSecond(),
			animSet->GetPlaybackType(), compressedData,
			numCallbacks, keys, &compressedAnimSet);
		compressedData->Release();

		//Delete the old keyframed animation set.
		m_aniController->UnregisterAnimationSet(animSet);

		// and then add the new compressed animation set.
		m_aniController->RegisterAnimationSet(compressedAnimSet);

		// Hook up the animation set to the first track.
		m_aniController->SetTrackAnimationSet(0, compressedAnimSet);
		compressedAnimSet->Release();
}

void WCharactor::Update(float m_deltaTime, CallbackHandler &callbackHandler)
{
	if (m_aniController) {
		m_aniController->AdvanceTime(m_deltaTime, &callbackHandler);
	}
}
