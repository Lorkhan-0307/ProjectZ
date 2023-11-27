// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/HUD/ZNonCombatHUD.h"
#include "Player/ZNonCombatPlayerController.h"
#include "Player/ZPlayerState.h"

AZCharacter::AZCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCamera->bUsePawnControlRotation = true;

	CardComponent = CreateDefaultSubobject<UCardComponent>(TEXT("CardComponent"));
}

void AZCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
}

void AZCharacter::InitAbilityActorInfo()
{
	AZPlayerState* ZPlayerState = GetPlayerState<AZPlayerState>();
	check(ZPlayerState);
	ZPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ZPlayerState, this);
	AbilitySystemComponent = ZPlayerState->GetAbilitySystemComponent();
	AttributeSet = ZPlayerState->GetAttributeSet();

	if (AZNonCombatPlayerController* ZPlayerController = Cast<AZNonCombatPlayerController>(GetController()))
	{
		if (AZHUDBase* ZHUD = Cast<AZHUDBase>(ZPlayerController->GetHUD()))
		{
			ZHUD->InitOverlay(ZPlayerController, GetPlayerState(), GetAbilitySystemComponent(), GetAttributeSet(), this);
		}
	}
}
