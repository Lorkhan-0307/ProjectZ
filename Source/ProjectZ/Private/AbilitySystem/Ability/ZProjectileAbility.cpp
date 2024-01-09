// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ZProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/ZProjectile.h"
#include "Interaction/CombatInterface.h"
#include "ZGameplayTag.h"


void UZProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("AbilityActive"));
}

void UZProjectileAbility::SpawnProjectile()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		// TO DO : Make Weapon and get socket, Set Projectile Rotation
		//SpawnTransform.SetLocation(SocketLocation);
		FVector Loc = GetAvatarActorFromActorInfo()->GetActorLocation();
		Loc.X += 200;
		SpawnTransform.SetLocation(Loc);

		// Use SpawnActorDeferred instead SpawnActor to give the Projectile a Gameplay Effect Spec for causing damage
		AZProjectile* Projectile = GetWorld()->SpawnActorDeferred<AZProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult HitResult;
		EffectContextHandle.AddHitResult(HitResult);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		const FZGameplayTag GameplayTag = FZGameplayTag::Get();
		// Get damage for CurveTable with card level
		//const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		const float ScaledDamage = 15.f;
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Damage, ScaledDamage);

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
