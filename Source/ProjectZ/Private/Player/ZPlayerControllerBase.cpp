// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZPlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/ZInputComponent.h"

AZPlayerControllerBase::AZPlayerControllerBase()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetDrawDebug(true);
}

void AZPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	ZInputComponent = CastChecked<UZInputComponent>(InputComponent);
	// Bind Input Action with tags
	ZInputComponent->BindAbilityAction(InputConfig, this, &AZPlayerControllerBase::AbilityInputTagPressed, &AZPlayerControllerBase::AbilityInputTagReleased, &AZPlayerControllerBase::AbilityInputTagHeld);
	ZInputComponent->BindAction(CameraMoveAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::CameraMove);
	ZInputComponent->BindAction(CameraResetAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::CameraReset);
}

void AZPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AZPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	CameraLocation = GetCharacter()->GetActorLocation() + CameraBaseLocation;
	const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn());
	ZPlayerCharacter->FirstPersonCamera->SetWorldLocation(CameraLocation);
}

void AZPlayerControllerBase::CameraMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// TO DO : Change AZNonCombatCharacter -> AZPlayerCharacter
	if (const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn()))
	{
		CameraLocation += FVector(MovementVector.X, MovementVector.Y, 0.f) * CameraMS;
		ZPlayerCharacter->FirstPersonCamera->SetWorldLocation(CameraLocation);
	}
}

void AZPlayerControllerBase::CameraReset()
{
	if (const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn()))
	{
		UE_LOG(LogTemp, Warning, TEXT("!!"));
		FTransform CameraTransform;

		CameraLocation = GetCharacter()->GetActorLocation() + CameraBaseLocation;
		ZPlayerCharacter->FirstPersonCamera->SetWorldLocation(CameraLocation, false);
	}
}

void AZPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FZGameplayTag::Get().InputTag_RMB))
	{
		bAutoRunning = false;
	}
}

void AZPlayerControllerBase::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FZGameplayTag::Get().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	const APawn* ControlledPawn = GetPawn();
	if (FollowTime <= ShortPressThreshold && ControlledPawn)
	{
		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
			}
			if (NavPath->PathPoints.Num() > 0)
			{
				CachedDestination = NavPath->PathPoints.Last();
				bAutoRunning = true;
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this,CachedDestination);
			}
		}
	}
	FollowTime = 0.f;
}

void AZPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FZGameplayTag::Get().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	// 
	/*
	if (bIsCombat)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	*/

	// Hold mouse button
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		CachedDestination = Hit.ImpactPoint;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.f);
	}
}


UZAbilitySystemComponent* AZPlayerControllerBase::GetASC()
{
	if (ZAbilitySystemComponent == nullptr)
	{
		ZAbilitySystemComponent = Cast<UZAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ZAbilitySystemComponent;
}
