// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/ZProjectileAbility.h"

#include "Actor/ZProjectile.h"
#include "Interaction/CombatInterface.h"


void UZProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		// TO DO : Make Weapon and get socket, Set Projectile Rotation
		//SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetLocation(GetAvatarActorFromActorInfo()->GetActorLocation());

		// Use SpawnActorDeferred instead SpawnActor to give the Projectile a Gameplay Effect Spec for causing damage
		AZProjectile* Projectile = GetWorld()->SpawnActorDeferred<AZProjectile>(ProjectileClass,SpawnTransform,GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
