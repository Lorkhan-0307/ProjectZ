// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZPlayerControllerBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Game/ZGameModeBase.h"
#include "Input/ZInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Player/ZPlayerState.h"
#include "UI/Combat/DamageTextComponent.h"

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
	ZInputComponent->BindAction(ESCAction, ETriggerEvent::Triggered, this, &AZPlayerControllerBase::ESC);
}

void AZPlayerControllerBase::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!%f!!"), DamageAmount);
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

void AZPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
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

void AZPlayerControllerBase::ESC()
{
	CardComponent = CardComponent == nullptr ? Cast<AZCharacterBase>(GetPawn())->CardComponent : CardComponent;
	if (CardComponent && CardComponent->bActivatingCard)
	{
		CardComponent->CancelActivateCard();
	}
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

void AZPlayerControllerBase::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else if (LastActor != ThisActor)
		{
			ThisActor->HighlightActor();
			LastActor->UnHighlightActor();
		}
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

	GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
	const ETurn CurrentTurn = GameMode->GetCurrentTurn();
	if (!(CurrentTurn == ETurn::ET_NonCombat || CurrentTurn == ETurn::ET_MoveTurn))
	{
		FollowTime = 0.f;
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

			const int32 Cost = FMath::CeilToInt(Spline->GetSplineLength() / 100.f);
			UZAttributeSet* AS = Cast<UZAttributeSet>(GetPlayerState<AZPlayerState>()->GetAttributeSet());

			if ((CurrentTurn == ETurn::ET_MoveTurn && Cost > AS->GetCost()) || !GameMode->bCanMove)
			{
				return;
			}

			if (NavPath->PathPoints.Num() > 0)
			{
				AS->SetCost(AS->GetCost() - Cost);
				CachedDestination = NavPath->PathPoints.Last();
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
				if (CurrentTurn == ETurn::ET_MoveTurn)
				{
					GameMode->SetTurn(ETurn::ET_PlayerTurn);
				}
			}
		}
	}
	FollowTime = 0.f;
}

void AZPlayerControllerBase::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FZGameplayTag::Get().InputTag_LMB))
	{
		CardComponent = CardComponent == nullptr ? Cast<AZCharacterBase>(GetPawn())->CardComponent : CardComponent;
		if (GetASC() && CardComponent && CardComponent->bActivatingCard)
		{
			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
			if (CardComponent->ActivatingCard.bTargeting)
			{
				if (Cast<ICombatInterface>(HitResult.GetActor()) == nullptr)
				{
					//CardComponent->CancelActivateCard();
					return;
				}
			}


			float Distance = FVector::DistXY(GetCharacter()->GetActorLocation(), HitResult.Location) / 100.f;
			if (Distance <= CardComponent->ActivatingCard.SkillRange)
			{
				GetASC()->AbilityInputTagHeld(CardComponent->ActivatingCard.CardTag); // Activate Skill Card
				CardComponent->UseCard(CardComponent->ActivatingCard);
				CardComponent->bActivatingCard = false;
				CardComponent->ActivateCardDelegate.Broadcast();
				UZAbilitySystemLibrary::PayCost(Cast<AZCharacterBase>(GetPawn()), CardComponent->ActivatingCard.CardCost);
			}
		}
		else if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	if (!InputTag.MatchesTagExact(FZGameplayTag::Get().InputTag_RMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	// Hold mouse button
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		CachedDestination = Hit.ImpactPoint;
	}

	const ETurn CurrentTurn = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->GetCurrentTurn();
	if (CurrentTurn != ETurn::ET_NonCombat) return;

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
