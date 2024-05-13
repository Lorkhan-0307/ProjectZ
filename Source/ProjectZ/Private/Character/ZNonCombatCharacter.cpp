// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZNonCombatCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/ZNonCombatPlayerController.h"
#include "Player/ZPlayerState.h"

AZNonCombatCharacter::AZNonCombatCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	const FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	TopDownCamera->DetachFromComponent(DetachmentTransformRules);
	//FirstPersonCamera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	TopDownCamera->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position the camera
	TopDownCamera->bUsePawnControlRotation = false;
}

void AZNonCombatCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	
}

