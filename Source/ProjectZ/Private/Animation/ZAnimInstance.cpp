// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZAnimInstance.h"

#include "Character/ZCharacterBase.h"

void UZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ZCharacter = Cast<AZCharacterBase>(TryGetPawnOwner());
}

void UZAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ZCharacter = ZCharacter == nullptr ? Cast<AZCharacterBase>(TryGetPawnOwner()) : ZCharacter;
	if (ZCharacter == nullptr) return;

	FVector Velocity = ZCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
}
