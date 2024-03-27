// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Character/CardComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AZCharacterBase::AZCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CardComponent = CreateDefaultSubobject<UCardComponent>(TEXT("CardComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	BleedDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BleedDebuffComponent");
	BleedDebuffComponent->SetupAttachment(GetRootComponent());
	BleedDebuffComponent->DebuffTag = FZGameplayTag::Get().Debuff_Bleed;
}

UAnimMontage* AZCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

FVector AZCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FZGameplayTag& GameplayTags = FZGameplayTag::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon))
	{
		// return Weapon->GetSocketLocation(WeaponSocketName);
		return FVector();
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		// return GetMesh->GetSocketLocation(LeftHandSocketName);
		return FVector();
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		// return GetMesh->GetSocketLocation(RightHandSocketName);
		return FVector();
	}
	return FVector();
}

void AZCharacterBase::Die()
{
	/*
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	*/

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->CharacterDie(this);

	SetLifeSpan(LifeSpan);
	bIsDead = true;
	OnDeathDelegate.Broadcast(this);
}

bool AZCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AZCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AZCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AZCharacterBase::AddDebuff_Implementation(FDebuff Debuff)
{
	for (FDebuff& NowDebuff : Debuffs)
	{
		if (NowDebuff.DebuffType.MatchesTagExact(Debuff.DebuffType))
		{
			if (Debuff.DebuffType.MatchesTagExact(FZGameplayTag::Get().Debuff_Stun) && StunImmunityCount > 0) return;
			
			NowDebuff.DebuffStack += Debuff.DebuffStack;
			NowDebuff.DebuffDuration = Debuff.DebuffDuration;
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Debuff.DebuffEffectSpec);
			return;
		}
	}
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Debuff.DebuffEffectSpec);
	Debuffs.Add(Debuff);
}

void AZCharacterBase::RemoveDebuff_Implementation(FGameplayTag RemoveDebuffType)
{
	for (FDebuff& NowDebuff : Debuffs)
	{
		if (NowDebuff.DebuffType.MatchesTagExact(RemoveDebuffType))
		{
			NowDebuff.DebuffDuration = 0;
			NowDebuff.DebuffStack = 0;
			
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(RemoveDebuffType);
			AbilitySystemComponent->RemoveActiveEffectsWithGrantedTags(TagContainer);
			return;
		}
	}
}

FOnASCRegistered AZCharacterBase::GetOnASCRegisterdDelegate()
{
	return OnASCRegisteredDelegate;
}

FOnDeath AZCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

// Called when the game starts or when spawned
void AZCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

int32 AZCharacterBase::GetLevel()
{
	return Level;
}

void AZCharacterBase::InitAbilityActorInfo()
{
}


void AZCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass) const
{
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


// Initialize first attribute value
void AZCharacterBase::InitializeDefaultAttributes() const
{
	/*
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	*/

	UZAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AZCharacterBase::AddCharacterAbility()
{
	UZAbilitySystemComponent* ZASC = CastChecked<UZAbilitySystemComponent>(AbilitySystemComponent);
	ZASC->AddCharacterAbility(StartupAbility);
}

// Called every frame
void AZCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->Velocity.Size() == 0)
	{
		SetCanAffectNavigationGeneration(true);
	}
	else
	{
		SetCanAffectNavigationGeneration(false);
	}
}

// Set AbilitySystemComponent
UAbilitySystemComponent* AZCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
