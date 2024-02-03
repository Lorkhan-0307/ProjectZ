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
#include "Components/SplineMeshComponent.h"
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
	ZInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::CameraRotate);
	ZInputComponent->BindAction(CameraResetAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::CameraReset);
	ZInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::CameraZoom);
}

void AZPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AZPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	CameraReset();
	CameraRotation = FRotator(CameraBaseAngle, 0.f, 0.f);
	CameraLocation = GetCharacter()->GetActorLocation() + FVector(0.f, 0.f, CameraBaseHeight);

	SetRotateLocation();
	SetCameraLocation();
}

void AZPlayerControllerBase::CameraMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// TO DO : Change AZNonCombatCharacter -> AZPlayerCharacter
	if (const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn()))
	{
		const float Radian = FMath::DegreesToRadians(CameraRotation.Yaw);
		const float DeltaX = MovementVector.X * FMath::Cos(Radian) - MovementVector.Y * FMath::Sin(Radian);
		const float DeltaY = MovementVector.X * FMath::Sin(Radian) + MovementVector.Y * FMath::Cos(Radian);
		CameraLocation += FVector(DeltaX, DeltaY, 0.f) * CameraMS;
		ZPlayerCharacter->TopDownCamera->SetWorldLocation(CameraLocation + RotateLocation);
	}
}

void AZPlayerControllerBase::CameraRotate(const FInputActionValue& InputActionValue)
{
	const float RotationValue = InputActionValue.Get<float>();
	CameraRotation += FRotator(0.f, CameraRS * RotationValue, 0.f);
	SetRotateLocation();
	SetCameraLocation();
}

void AZPlayerControllerBase::CameraReset()
{
	CameraLocation = FVector(GetCharacter()->GetActorLocation().X, GetCharacter()->GetActorLocation().Y, CameraLocation.Z);
	SetCameraLocation();
}

void AZPlayerControllerBase::CameraZoom(const FInputActionValue& InputActionValue)
{
	const float ZoomValue = InputActionValue.Get<float>();

	CameraLocation.Z -= ZoomValue * CameraZoomSpeed;
	SetRotateLocation();
	SetCameraLocation();
}

void AZPlayerControllerBase::SetCameraLocation()
{
	if (const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn()))
	{
		ZPlayerCharacter->TopDownCamera->SetWorldLocation(CameraLocation + RotateLocation);
	}
}

void AZPlayerControllerBase::SetRotateLocation()
{
	const float Radian = FMath::DegreesToRadians(CameraRotation.Yaw);
	const float Radius = CameraLocation.Z * FMath::Tan(FMath::DegreesToRadians(CameraRotation.Pitch)) * CameraRadius;
	if (const AZNonCombatCharacter* ZPlayerCharacter = Cast<AZNonCombatCharacter>(GetPawn()))
	{
		ZPlayerCharacter->TopDownCamera->SetWorldRotation(CameraRotation);
		RotateLocation = FVector(Radius * FMath::Cos(Radian), Radius * FMath::Sin(Radian), 0.f);
	}
}

void AZPlayerControllerBase::AbilityInputTagPressed(FGameplayTag InputTag)
{
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
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
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

	// Not Implemented Yet
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
