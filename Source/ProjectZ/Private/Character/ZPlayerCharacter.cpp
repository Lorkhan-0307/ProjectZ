// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZPlayerCharacter.h"

#include "UI/Combat/CostPathLengthWidget.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Character/SkillRangeComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Game/ZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/ZNonCombatPlayerController.h"
#include "Player/ZPlayerState.h"
#include "UI/HUD/ZHUDBase.h"


AZPlayerCharacter::AZPlayerCharacter()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));

	SkillRangeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillRange"));
	SkillRangeMesh->SetupAttachment(GetRootComponent());
	SkillRangeMesh->SetVisibility(false);
	SkillRangeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// When PlayerController Possess the Character
void AZPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (bIsPossessed) return;
	bIsPossessed = true;
	InitAbilityActorInfo();
	AddCharacterAbility();

	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->TurnChangedDelegate.AddDynamic(this, &AZPlayerCharacter::TurnChanged);
}

// Setup Character
void AZPlayerCharacter::InitAbilityActorInfo()
{
	if (AbilitySystemComponent) return;
	AZPlayerState* ZPlayerState = GetPlayerState<AZPlayerState>();
	check(ZPlayerState);
	ZPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ZPlayerState, this);
	Cast<UZAbilitySystemComponent>(ZPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = ZPlayerState->GetAbilitySystemComponent();
	AttributeSet = ZPlayerState->GetAttributeSet();
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);

	if (AZPlayerControllerBase* ZPlayerController = Cast<AZPlayerControllerBase>(GetController()))
	{
		if (AZHUDBase* ZHUD = Cast<AZHUDBase>(ZPlayerController->GetHUD()))
		{
			ZHUD->InitOverlay(ZPlayerController, GetPlayerState(), GetAbilitySystemComponent(), GetAttributeSet(), this);
		}
	}
	InitializeDefaultAttributes();

	// TO DO : Make Difference by game mode
	CardComponent->InitializeCardComponent(this);
	//CardComponent->InitializeNonCombat(this);
	//CardComponent->InitializeCombat(this);
}

int32 AZPlayerCharacter::GetLevel()
{
	const AZPlayerState* ZPlayerState = GetPlayerState<AZPlayerState>();
	return ZPlayerState->GetPlayerLevel();
}

void AZPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateSplinePath();
	SetSkillRangePos();
}

void AZPlayerCharacter::UpdateSplinePath()
{
	Spline->ClearSplinePoints(true);
	if (SplineMesh.Num() > 0)
	{
		for (int i = 0; i < SplineMesh.Num(); i++)
		{
			if (SplineMesh[i])
			{
				SplineMesh[i]->DestroyComponent();
			}
		}
		SplineMesh.Empty();
	}

	const AZGameModeBase* ZGameMode = Cast<AZGameModeBase>(UGameplayStatics::GetGameMode(this));
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	if (ZGameMode->GetCurrentTurn() != ETurn::ET_MoveTurn || MousePosition.Y > CardComponent->GetPlayCardHeight())
	{
		return;
	}

	FVector StartPos = GetActorLocation();
	FVector EndPos;

	FHitResult Hit;
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		if (Hit.GetComponent() && Hit.GetComponent()->ComponentHasTag(FName("NonClickable"))) return;
		EndPos = Hit.ImpactPoint;
	}

	if (GetCharacterMovement()->Velocity.Size() > 0) EndPos = Cast<AZPlayerControllerBase>(GetController())->GetCachedDestination() + FVector(0.f, 0.f, SplineHeight);
	if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, StartPos, EndPos))
	{
		Spline->ClearSplinePoints();
		for (const FVector& PointLoc : NavPath->PathPoints)
		{
			Spline->AddSplinePoint(PointLoc + FVector(0.f, 0.f, SplineHeight), ESplineCoordinateSpace::World, ESplinePointType::Curve);
		}
	}

	int32 PointNum = Spline->GetNumberOfSplinePoints();
	for (int SplineCount = 0; SplineCount < PointNum; SplineCount++)
	{
		const FVector& CurrentPointLoc = Spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);

		const int32 NextPointIndex = SplineCount + 1;
		if (NextPointIndex < PointNum)
		{
			const FVector& NextPointLoc = Spline->GetLocationAtSplinePoint(NextPointIndex, ESplineCoordinateSpace::Local);
			const float DistToNext = FVector::Distance(NextPointLoc, CurrentPointLoc);
			Spline->SplineCurves.Position.Points[SplineCount].LeaveTangent = Spline->SplineCurves.Position.Points[SplineCount].LeaveTangent.GetClampedToMaxSize(DistToNext);
		}

		const int32 PrevPointIndex = SplineCount - 1;
		if (PrevPointIndex >= 0)
		{
			const FVector& PrevPointLoc = Spline->SplineCurves.Position.Points[PrevPointIndex].OutVal;
			const float DistToPrev = FVector::Distance(PrevPointLoc, CurrentPointLoc);
			Spline->SplineCurves.Position.Points[SplineCount].ArriveTangent = Spline->SplineCurves.Position.Points[SplineCount].ArriveTangent.GetClampedToMaxSize(DistToPrev);
		}
		Spline->SplineCurves.Position.Points[SplineCount].InterpMode = CIM_CurveUser;
	}

	for (int SplineCount = 0; SplineCount < Spline->GetNumberOfSplinePoints() - 1; SplineCount++)
	{
		Spline->SetSplinePointType(SplineCount, ESplinePointType::Curve);
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);
		SplineMeshComponent->SetStaticMesh(DefaultMesh);
		SplineMeshComponent->CastShadow = false;

		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());

		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStartScale(SplineScale);
		SplineMeshComponent->SetEndScale(SplineScale);

		const FVector StartPoint = Spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
		const FVector EndPoint = Spline->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SplineMesh.Add(SplineMeshComponent);
	}
	SplineLength = Spline->GetSplineLength() / 100.f;
}

void AZPlayerCharacter::TurnChanged(ETurn Turn)
{
	if (Turn != ETurn::ET_MoveTurn) return;

	UZAttributeSet* AS = Cast<UZAttributeSet>(AttributeSet);
	if (AS)
	{
		AS->SetCost(AS->GetMaxCost());
	}

	GetCharacterMovement()->StopActiveMovement();
}

void AZPlayerCharacter::SetSkillRangePos()
{
	FHitResult CursorHit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	FVector Pos = CursorHit.Location - GetActorLocation();
	for (USkillRangeComponent* Component : SkillRangeComponents)
	{
		if (IsValid(Component))
		{
			float MouseAngle = FMath::RadiansToDegrees(FMath::Atan2(Pos.Y, Pos.X));
			Component->SetWorldRotation(FRotator(0.f, Component->Angle + MouseAngle, 0.f));
		}
	}
}

int32 AZPlayerCharacter::GetPathCost()
{
	return FMath::CeilToInt32(SplineLength);
}

float AZPlayerCharacter::GetPathLength()
{
	return SplineLength;
}

void AZPlayerCharacter::ShowSKillRange(float Angle, float Range)
{
	/*
	SkillRangeMesh->SetWorldScale3D(FVector(Range, Range, 1.f));
	SkillRangeMesh->SetVisibility(true);
	*/

	HideSkillRange();

	for (int i = - Angle / 10; i < Angle / 10; i++)
	{
		USkillRangeComponent* SkillRangeComponent = NewObject<USkillRangeComponent>(this, SkillRangedMeshComponentClass);
		SkillRangeComponent->RegisterComponent();
		SkillRangeComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SkillRangeComponent->InitMesh(i * 5, Range);
		SkillRangeComponent->SetMaterial(0, SkillRangeMaterial);
		SkillRangeComponents.Add(SkillRangeComponent);
	}
}

void AZPlayerCharacter::HideSkillRange()
{
	//SkillRangeMesh->SetVisibility(false);

	for (USkillRangeComponent* SkillRangeComponent : SkillRangeComponents)
	{
		if (SkillRangeComponent) SkillRangeComponent->DestroyComponent();
	}
	SkillRangeComponents.Empty();
}

void AZPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}
