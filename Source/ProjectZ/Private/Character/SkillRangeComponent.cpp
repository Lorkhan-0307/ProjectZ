// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SkillRangeComponent.h"

#include "GameFramework/Character.h"

USkillRangeComponent::USkillRangeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void USkillRangeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult CursorHit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	FVector Pos = CursorHit.Location - GetOwner()->GetActorLocation();
	float MouseAngle = FMath::RadiansToDegrees(FMath::Atan2(Pos.Y, Pos.X));
	SetWorldRotation(FRotator(0.f, Angle + MouseAngle, 0.f));
}

void USkillRangeComponent::InitMesh(float InAngle, float InRadius)
{
	Radius = InRadius;
	Angle = InAngle + 180.f;
	SetWorldScale3D(FVector(Radius, Radius, 1.f));
	SetWorldRotation(FRotator(0.f, Angle, 0.f));
	SetRelativeLocation(FVector(0.f, 0.f, -89.f));
}
