// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZCharacterBase.h"
#include "Character/CardComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

// Sets default values
AZCharacterBase::AZCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CardComponent = CreateDefaultSubobject<UCardComponent>(TEXT("CardComponent"));
}

// Called when the game starts or when spawned
void AZCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 AZCharacterBase::GetLevel()
{
	return 0;
}

void AZCharacterBase::InitAbilityActorInfo()
{
}

void AZCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

// Initialize first attribute value
void AZCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1.f);
	ApplyEffectToSelf(DefaultVitalAttributes,1.f);
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

