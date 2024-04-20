// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SkillRangeComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API USkillRangeComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	USkillRangeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY()
	float Angle = 60.f;

	UPROPERTY()
	float Radius = 100.f;

	void InitMesh(float InAngle, float InRadius);

	UPROPERTY()
	UStaticMesh* RangeMesh;

private:
	int32 MeshCount;
};
