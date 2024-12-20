﻿#include "ZAbilityType.h"


bool FZGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsDodged)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0)
		{
			RepBits |= 1 << 11;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 12;
		}
		if (DebuffType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (DebuffStack > 0)
		{
			RepBits |= 1 << 14;
		}
		if (BuffType.IsValid())
		{
			RepBits |= 1 << 15;
		}
		if (BuffAttribute.IsValid())
		{
			RepBits |= 1 << 16;
		}
		if (BuffMagnitude > 0)
		{
			RepBits |= 1 << 17;
		}
		if (BuffDuration > 0)
		{
			RepBits |= 1 << 18;
		}
		if (bIsBlocked)
		{
			RepBits |= 1 << 19;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 20;
		}


		Ar.SerializeBits(&RepBits, 21);

		if (RepBits & (1 << 0))
		{
			Ar << Instigator;
		}
		if (RepBits & (1 << 1))
		{
			Ar << EffectCauser;
		}
		if (RepBits & (1 << 2))
		{
			Ar << AbilityCDO;
		}
		if (RepBits & (1 << 3))
		{
			Ar << SourceObject;
		}
		if (RepBits & (1 << 4))
		{
			SafeNetSerializeTArray_Default<31>(Ar, Actors);
		}
		if (RepBits & (1 << 5))
		{
			if (Ar.IsLoading())
			{
				if (!HitResult.IsValid())
				{
					HitResult = MakeShared<FHitResult>();
				}
			}
			HitResult->NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 6))
		{
			Ar << WorldOrigin;
			bHasWorldOrigin = true;
		}
		else
		{
			bHasWorldOrigin = false;
		}
		if (RepBits & (1 << 7))
		{
			Ar << bIsDodged;
		}
		if (RepBits & (1 << 8))
		{
			Ar << bIsCriticalHit;
		}
		if (RepBits & (1 << 9))
		{
			Ar << bIsSuccessfulDebuff;
		}
		if (RepBits & (1 << 10))
		{
			Ar << DebuffDamage;
		}
		if (RepBits & (1 << 11))
		{
			Ar << DebuffDuration;
		}
		if (RepBits & (1 << 12))
		{
			if (Ar.IsLoading())
			{
				if (!DamageType.IsValid())
				{
					DamageType = MakeShared<FGameplayTag>();
				}
			}
			DamageType->NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 13))
		{
			Ar << DebuffType;
		}
		if (RepBits & (1 << 14))
		{
			Ar << DebuffStack;
		}
		if (RepBits & (1 << 15))
		{
			Ar << BuffType;
		}
		if (RepBits & (1 << 16))
		{
			Ar << BuffAttribute;
		}
		if (RepBits & (1 << 17))
		{
			Ar << BuffMagnitude;
		}
		if (RepBits & (1 << 18))
		{
			Ar << BuffDuration;
		}
		if (RepBits & (1 << 19))
		{
			Ar << bIsBlocked;
		}
		if (RepBits & (1 << 20))
		{
			Ar << KnockbackForce;
		}
	}

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
