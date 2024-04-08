// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZEnemy.h"

#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "ZGameplayTag.h"
#include "AI/ZAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CardComponent.h"
#include "Game/ZGameModeBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "ProjectZ/ProjectZ.h"

AZEnemy::AZEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UZAbilitySystemComponent>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UZAttributeSet>("AttributeSet");

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));
	AIPerceptionStimuliSourceComponent->bAutoRegister = true;
	//AIPerceptionStimuliSourceComponent->regi
}

void AZEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ZAIController = Cast<AZAIController>(NewController);
	ZAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	ZAIController->RunBehaviorTree(BehaviorTree);
	ZAIController->GetBlackboardComponent()->SetValueAsBool(FName("EnemyTurn"), false);
	ZAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), bRangeAttacker);
	/*
	if (CharacterClass == ECharacterClass::RangedAttackZombie)
		ZAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), true);
	*/
}

void AZEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AZCharacterBase* ZCharacter = Cast<AZCharacterBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UCardComponent* CharacterCardComponent = ZCharacter->GetCardComponent();

	if (!CharacterCardComponent->bActivatingCard) return;
	FHitResult CursorHit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	FVector MousePos = CursorHit.Location;

	FVector CenterToMouse = MousePos - ZCharacter->GetActorLocation();
	CenterToMouse.Normalize();
	FVector CenterToTarget = GetActorLocation() - ZCharacter->GetActorLocation();
	const float Distance = CenterToTarget.Length() / 100.f;
	CenterToTarget.Normalize();
	const float TargetAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CenterToMouse, CenterToTarget)));

	if (CharacterCardComponent && TargetAngle <= CharacterCardComponent->ActivatingCard.SkillAngle / 2.f && Distance <= CharacterCardComponent->ActivatingCard.SkillRange)
	{
		HighlightActor();
	}
	else
	{
		UnHighlightActor();
	}
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

void AZEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AZEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AZEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	UZAbilitySystemLibrary::GiveStartupAbility(this, AbilitySystemComponent, CharacterClass);

	if (const UZAttributeSet* ZAS = Cast<UZAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ZAS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
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

	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->TurnChangedDelegate.AddDynamic(this, &AZEnemy::TurnChanged);
}

void AZEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UZAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	UCharacterClassInfo* CharacterClassInfo = UZAbilitySystemLibrary::GetCharacterClassInfo(this);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	bRangeAttacker = ClassDefaultInfo.bRangeAttacker;

	InitializeDefaultAttributes();
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
}

void AZEnemy::TurnChanged(ETurn Turn)
{
	if (Turn != ETurn::ET_EnemyTurn)
	{
		ZAIController->GetBlackboardComponent()->SetValueAsBool(FName("MyTurn"), false);
		return;
	}

	UZAttributeSet* AS = Cast<UZAttributeSet>(AttributeSet);
	if (AS)
	{
		AS->SetCost(AS->GetMaxCost());
	}

	if (!bIsMyTurn) return;

	ZAIController->GetBlackboardComponent()->SetValueAsFloat(FName("Cost"), AS->GetCost());
	ZAIController->GetBlackboardComponent()->SetValueAsBool(FName("MyTurn"), true);
}
