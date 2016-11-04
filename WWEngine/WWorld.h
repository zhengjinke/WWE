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
	std::vector< WObject* >m_objBuf;					// 世界中的对象指针池
	std::priority_queue< ObjFreeHashNode >m_freeque;	// 用于查询对象指针池中是否存在被标记为空的位置
	IDirect3DDevice9*	m_pDevice;
	ID3DXEffect*		g_pEffect;

//PhysX 部分；
#ifdef PXPHYSICS_ONLINE
	/************************************************************************/
	/* 物理                                                                 */
	/************************************************************************/
	const physx::PxVec3						gDefaultGravity = PxVec3(0.0f, -9.8f, 0.0f);
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

	void CreateTestBox(IDirect3DDevice9* g_pDevice) {
		if(!mScene) fatalError("TestBox Create Failed cause no mScene existed");
		WStaticObject *obj = new WStaticObject();
		obj->CreateBox(g_pDevice);
		this->AddObj(obj);
		mTestActor = mPhysics->createRigidDynamic(PxTransform(PxVec3(obj->transform.position.x, obj->transform.position.y, obj->transform.position.z)));
		physx::PxMaterial *aMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.1f);
		PxShape* aBoxShape = mTestActor->createShape(PxBoxGeometry(1.0f, 1.0f, 1.0f), *aMaterial);
		
	}
	

	physx::PxPhysics* customCreatePhysics(physx::PxU32 version,     // 自定义需要的物理，暂时没有用上
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
		mSceneDesc->gravity = gDefaultGravity;					// 设置默认重力加速度
		mSceneDesc->broadPhaseType = PxBroadPhaseType::eSAP;		// eSAP 用于只有较少的物体需要进行碰撞检测时使用
		//mSceneDesc->broadPhaseType = PxBroadPhaseType::eMBP;	// eMBP 用于只有较多的物体需要进行碰撞检测时使用，使用时需要添加世界边界
		mScene = mPhysics->createScene(*mSceneDesc);
		if (!mScene) fatalError("new mScene failed!");
	}

	void CreatePhysXMaterial() {
		
	}



	void CreatePhysXBox() {
		WStaticObject *obj = new WStaticObject(0.0f,0.0f,0.0f);
		obj->CreateBox(m_pDevice);
		AddObj(obj);
	}
#endif

#endif




public:
	WCamera *m_camera;									// 世界中的摄像机
	WWorld(){}
	WWorld(IDirect3DDevice9 *g_pDevice);
	void AddObj(WObject *obj);							// 将一个对象指针加入对象指针池中
	bool RemoveObj(int nIdx);
	bool CreateCharactor(char *szFile, char *szName, float x, float y, float z);
	void SetCamera(IDirect3DDevice9* g_pDevice, ID3DXEffect* g_pEffect);
	void Draw(IDirect3DDevice9*	g_pDevice);
	void Draw(IDirect3DDevice9*	g_pDevice, SkinnedMesh a, D3DMATRIX identity);
	void UpdateWorld(float m_deltaTime, CallbackHandler &callbackHandler);
	void Init(IDirect3DDevice9*);
	bool Release();
};