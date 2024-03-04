// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZCharacterBase.h"
#include "Character/CardComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "Components/CapsuleComponent.h"

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
}

UAnimMontage* AZCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
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

FVector AZCharacterBase::GetCombatSocketLocation()
{
	// When weapon system created, get weapon socket
	//return Weapon->GetSocketLocation(WeaponSocketName);
	return FVector();
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
}

// Set AbilitySystemComponent
UAbilitySystemComponent* AZCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
