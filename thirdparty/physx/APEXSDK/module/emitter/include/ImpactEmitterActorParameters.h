/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


// This file was generated by NxParameterized/scripts/GenParameterized.pl
// Created: 2015.10.22 17:26:23

#ifndef HEADER_ImpactEmitterActorParameters_h
#define HEADER_ImpactEmitterActorParameters_h

#include "NxParametersTypes.h"

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
#include "NxParameterized.h"
#include "NxParameters.h"
#include "NxParameterizedTraits.h"
#include "NxTraitsInternal.h"
#endif

namespace physx
{
namespace apex
{
namespace emitter
{

#pragma warning(push)
#pragma warning(disable: 4324) // structure was padded due to __declspec(align())

namespace ImpactEmitterActorParametersNS
{



struct ParametersStruct
{

	NxParameterized::Interface* explosionEnv;

};

static const physx::PxU32 checksum[] = { 0x6785e5a5, 0x2d0171c3, 0xecd991a6, 0xeebe4e58, };

} // namespace ImpactEmitterActorParametersNS

#ifndef NX_PARAMETERIZED_ONLY_LAYOUTS
class ImpactEmitterActorParameters : public NxParameterized::NxParameters, public ImpactEmitterActorParametersNS::ParametersStruct
{
public:
	ImpactEmitterActorParameters(NxParameterized::Traits* traits, void* buf = 0, PxI32* refCount = 0);

	virtual ~ImpactEmitterActorParameters();

	virtual void destroy();

	static const char* staticClassName(void)
	{
		return("ImpactEmitterActorParameters");
	}

	const char* className(void) const
	{
		return(staticClassName());
	}

	static const physx::PxU32 ClassVersion = ((physx::PxU32)0 << 16) + (physx::PxU32)0;

	static physx::PxU32 staticVersion(void)
	{
		return ClassVersion;
	}

	physx::PxU32 version(void) const
	{
		return(staticVersion());
	}

	static const physx::PxU32 ClassAlignment = 8;

	static const physx::PxU32* staticChecksum(physx::PxU32& bits)
	{
		bits = 8 * sizeof(ImpactEmitterActorParametersNS::checksum);
		return ImpactEmitterActorParametersNS::checksum;
	}

	static void freeParameterDefinitionTable(NxParameterized::Traits* traits);

	const physx::PxU32* checksum(physx::PxU32& bits) const
	{
		return staticChecksum(bits);
	}

	const ImpactEmitterActorParametersNS::ParametersStruct& parameters(void) const
	{
		ImpactEmitterActorParameters* tmpThis = const_cast<ImpactEmitterActorParameters*>(this);
		return *(static_cast<ImpactEmitterActorParametersNS::ParametersStruct*>(tmpThis));
	}

	ImpactEmitterActorParametersNS::ParametersStruct& parameters(void)
	{
		return *(static_cast<ImpactEmitterActorParametersNS::ParametersStruct*>(this));
	}

	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle) const;
	virtual NxParameterized::ErrorType getParameterHandle(const char* long_name, NxParameterized::Handle& handle);

	void initDefaults(void);

protected:

	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void);
	virtual const NxParameterized::DefinitionImpl* getParameterDefinitionTree(void) const;


	virtual void getVarPtr(const NxParameterized::Handle& handle, void*& ptr, size_t& offset) const;

private:

	void buildTree(void);
	void initDynamicArrays(void);
	void initStrings(void);
	void initReferences(void);
	void freeDynamicArrays(void);
	void freeStrings(void);
	void freeReferences(void);

	static bool mBuiltFlag;
	static NxParameterized::MutexType mBuiltFlagMutex;
};

class ImpactEmitterActorParametersFactory : public NxParameterized::Factory
{
	static const char* const vptr;

public:
	virtual NxParameterized::Interface* create(NxParameterized::Traits* paramTraits)
	{
		// placement new on this class using mParameterizedTraits

		void* newPtr = paramTraits->alloc(sizeof(ImpactEmitterActorParameters), ImpactEmitterActorParameters::ClassAlignment);
		if (!NxParameterized::IsAligned(newPtr, ImpactEmitterActorParameters::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ImpactEmitterActorParameters");
			paramTraits->free(newPtr);
			return 0;
		}

		memset(newPtr, 0, sizeof(ImpactEmitterActorParameters)); // always initialize memory allocated to zero for default values
		return NX_PARAM_PLACEMENT_NEW(newPtr, ImpactEmitterActorParameters)(paramTraits);
	}

	virtual NxParameterized::Interface* finish(NxParameterized::Traits* paramTraits, void* bufObj, void* bufStart, physx::PxI32* refCount)
	{
		if (!NxParameterized::IsAligned(bufObj, ImpactEmitterActorParameters::ClassAlignment)
		        || !NxParameterized::IsAligned(bufStart, ImpactEmitterActorParameters::ClassAlignment))
		{
			NX_PARAM_TRAITS_WARNING(paramTraits, "Unaligned memory allocation for class ImpactEmitterActorParameters");
			return 0;
		}

		// Init NxParameters-part
		// We used to call empty constructor of ImpactEmitterActorParameters here
		// but it may call default constructors of members and spoil the data
		NX_PARAM_PLACEMENT_NEW(bufObj, NxParameterized::NxParameters)(paramTraits, bufStart, refCount);

		// Init vtable (everything else is already initialized)
		*(const char**)bufObj = vptr;

		return (ImpactEmitterActorParameters*)bufObj;
	}

	virtual const char* getClassName()
	{
		return (ImpactEmitterActorParameters::staticClassName());
	}

	virtual physx::PxU32 getVersion()
	{
		return (ImpactEmitterActorParameters::staticVersion());
	}

	virtual physx::PxU32 getAlignment()
	{
		return (ImpactEmitterActorParameters::ClassAlignment);
	}

	virtual const physx::PxU32* getChecksum(physx::PxU32& bits)
	{
		return (ImpactEmitterActorParameters::staticChecksum(bits));
	}
};
#endif // NX_PARAMETERIZED_ONLY_LAYOUTS

} // namespace emitter
} // namespace apex
} // namespace physx

#pragma warning(pop)

#endif
