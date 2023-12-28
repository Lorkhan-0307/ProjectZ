// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ZProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/ZProjectile.h"
#include "Interaction/CombatInterface.h"


void UZProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp,Warning,TEXT("AbilityActive"));
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
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
