#pragma once
#include <vector>
#include <queue>
#include "Common.h"
#include "WObject.h"
#include "WCharactor.h"
#include "WStaticObject.h"
#include "WCamera.h"
#include <PxDefaultErrorCallback.h>
#include <Px.h>
#include <PxDefaultBufferedProfiler.h>
#include <PxFoundation.h>
#include "Light.h"

//#define PXPHYSICS_ONLINE
//#define	PHYSXDEBUG

using namespace physx;
class WWorld {
private:
	std::vector< WObject* >					m_objBuf;					// �����еĶ���ָ���
	std::priority_queue< ObjFreeHashNode >	m_freeque;	// ���ڲ�ѯ����ָ������Ƿ���ڱ����Ϊ�յ�λ��
	std::map<string, int>					m_mapName2Index;
	IDirect3DDevice9*						m_pDevice;
	ID3DXEffect*							g_pEffect;

//PhysX ���֣�
#ifdef PXPHYSICS_ONLINE
	/************************************************************************/
	/* ����                                                                 */
	/************************************************************************/
	const  physx::PxVec3					gDefaultGravity = PxVec3(0.0f, -9.8f, 0.0f);
	static physx::PxDefaultErrorCallback	gDefaultErrorCallback;
	static physx::PxDefaultAllocator		gDefaultAllocatorCallback;

	physx::PxFoundation*					mFoundation;
	physx::PxProfileZoneManager*			mProfileZoneManager;
	physx::PxPhysics*						mPhysics;
	physx::PxCooking*						mCooking;
	physx::PxSceneDesc*						mSceneDesc;
	physx::PxCpuDispatcher*					mCpuDispatcher;
	physx::PxScene*							mScene;
	physx::PxRigidActor*					mTestActor;
	physx::PxMaterial*						mPxMtrl;
	
	const physx::PxU32						mNbThreads = 1;
	const bool								recordMemoryAllocations = true;
	
public:
	void CreatePhysicsEngine() {
		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation) fatalError("PxCreateFoundation failed!");

		mProfileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(mFoundation);
		if (!mProfileZoneManager) fatalError("PxProfileZoneManager::createProfileZoneManager failed!");

		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager);
		if (!mPhysics) fatalError("PxCreatePhysics failed!");

		mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));
		if (!mCooking) fatalError("PxCreateCooking failed!");

		if (!PxInitExtensions(*mPhysics)) fatalError("PxInitExtensions failed!");

		mCpuDispatcher = PxDefaultCpuDispatcherCreate(mNbThreads);
		if (!mCpuDispatcher) fatalError("PxDefaultCpuDispatcherCreate failed!");

		mSceneDesc = new physx::PxSceneDesc(mPhysics->getTolerancesScale());
		if (!mSceneDesc) fatalError("new PxSceneDesc failed!");

		mSceneDesc->gravity = physx::PxVec3(0.0f,0.0f,9.8f);
		mSceneDesc->cpuDispatcher = mCpuDispatcher;

		mScene = mPhysics->createScene(*mSceneDesc);

		//mPhysics = customCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager);
		//if (!mPhysics) fatalError("PxCreatePhysics failed!");
	}

	static PxRigidActor* createRigidActor(PxScene& scene, PxPhysics& physics,
		const PxTransform& pose, const PxGeometry& geometry, PxMaterial& material,
		const PxFilterData* fd, const PxReal* density, const PxReal* mass, PxU32 flags)
	{
		const bool isDynamic = (density && *density) || (mass && *mass);
		PxRigidActor* actor = isDynamic ? static_cast<PxRigidActor*>(physics.createRigidDynamic(pose))
			: static_cast<PxRigidActor*>(physics.createRigidStatic(pose));
		if (!actor)
			return NULL;
		PxShape* shape = actor->createShape(geometry, material);
		if (!shape)
		{
			actor->release();
			return NULL;
		}
		if (fd)
			shape->setSimulationFilterData(*fd);
		if (isDynamic)
		{
			PxRigidDynamic* body = static_cast<PxRigidDynamic*>(actor);
			{
				if (density)
					PxRigidBodyExt::updateMassAndInertia(*body, *density);
				else
					PxRigidBodyExt::setMassAndUpdateInertia(*body, *mass);
			}
		}
		scene.addActor(*actor);
		return actor;
	}

	void CreateTestBox(IDirect3DDevice9* g_pDevice) {
		if(!mScene) fatalError("TestBox Create Failed cause no mScene existed");
		WStaticObject *obj = new WStaticObject();
		obj->CreateBox(g_pDevice);
		this->AddObj(obj);
		mTestActor = mPhysics->createRigidDynamic(PxTransform(PxVec3(obj->transform.position.x, obj->transform.position.y, obj->transform.position.z)));
		physx::PxMaterial *aMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f);
		PxShape* aBoxShape = mTestActor->createShape(PxBoxGeometry(1.0f, 1.0f, 1.0f), *aMaterial);
		
	}
	

	physx::PxPhysics* customCreatePhysics(physx::PxU32 version,     // �Զ�����Ҫ��������ʱû������
		physx::PxFoundation& foundation,
		const physx::PxTolerancesScale& scale,
		bool trackOutstandingAllocations,
		physx::PxProfileZoneManager* profileZoneManager)
	{
		physx::PxPhysics* physics = PxCreateBasePhysics(version, foundation, scale,
			trackOutstandingAllocations, profileZoneManager);

		if (!physics)
			return NULL;

		PxRegisterArticulations(*physics);
		PxRegisterHeightFields(*physics);

		return physics;
	}

	////////////////////////////////////////////////////////////////////////// //


#ifdef PHYSXDEBUG
public:

	void CreatePhysXFrame() {
		// Create PhysX Frame World
		mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if (!mFoundation) fatalError("PxCreateFoundation failed!");

		mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, mProfileZoneManager);
		if (!mPhysics) fatalError("PxCreatePhysics failed!");

	}

	void CreatePhysXScene() {
		mSceneDesc = new physx::PxSceneDesc(mPhysics->getTolerancesScale()); 
		if (!mSceneDesc) fatalError("new PxSceneDesc failed!");
		mSceneDesc->gravity = gDefaultGravity;					// ����Ĭ���������ٶ�
		mSceneDesc->broadPhaseType = PxBroadPhaseType::eSAP;		// eSAP ����ֻ�н��ٵ�������Ҫ������ײ���ʱʹ��
		//mSceneDesc->broadPhaseType = PxBroadPhaseType::eMBP;	// eMBP ����ֻ�н϶��������Ҫ������ײ���ʱʹ�ã�ʹ��ʱ��Ҫ�������߽�
		mScene = mPhysics->createScene(*mSceneDesc);
		if (!mScene) fatalError("new mScene failed!");
	}

	void CreatePhysXBox() {
		WStaticObject *obj = new WStaticObject(0.0f,0.0f,0.0f);
		obj->CreateBox(m_pDevice);
		AddObj(obj);
	}
#endif

#endif

public:
	WCamera *m_camera;											// �����е������
public:
	
	WWorld(){}
	WWorld(IDirect3DDevice9 *g_pDevice);
	void		AddObj(WObject *obj);							// ��һ������ָ��������ָ�����
	bool		RemoveObj(int nIdx);
	bool		CreateCharactor(char *szFile, char *szName, float x, float y, float z);
	int			GetPlayerIndexByName(string szName);
	WObject *	GetPlayerByName(string szName);
	void		SetCamera(IDirect3DDevice9* g_pDevice, ID3DXEffect* g_pEffect);
	void		Draw(IDirect3DDevice9*	g_pDevice);
	void		Draw(IDirect3DDevice9*	g_pDevice, SkinnedMesh a, D3DMATRIX identity);
	void		UpdateWorld(float m_deltaTime, CallbackHandler &callbackHandler);
	void		Init(IDirect3DDevice9*);
	bool		Release();
};