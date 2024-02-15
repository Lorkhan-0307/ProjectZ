// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZEnemy.h"

#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "ZGameplayTag.h"
#include "AI/ZAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectZ/ProjectZ.h"

AZEnemy::AZEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UZAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UZAttributeSet>("AttributeSet");
}

void AZEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ZAIController = Cast<AZAIController>(NewController);
	ZAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	ZAIController->RunBehaviorTree(BehaviorTree);
}

int32 AZEnemy::GetPlayerLevel()
{
	return Level;
}

void AZEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	// Weapon->SetRenderCustomDepth(true);
	// Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AZEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AZEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
}

void AZEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	UZAbilitySystemLibrary::GiveStartupAbility(this, AbilitySystemComponent);

	if (const UZAttributeSet* ZAS = Cast<UZAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
			UE_LOG(LogTemp, Warning, TEXT("Damage Applied %f"), Data.NewValue);
		});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
		// Allow events to be registered for specific gameplay tags being added or removed
		AbilitySystemComponent->RegisterGameplayTagEvent(FZGameplayTag::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AZEnemy::HitReactTagChanged);

		OnHealthChanged.Broadcast(ZAS->GetHealth());
		OnMaxHealthChanged.Broadcast(ZAS->GetMaxHealth());
	}
}

void AZEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UZAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}
