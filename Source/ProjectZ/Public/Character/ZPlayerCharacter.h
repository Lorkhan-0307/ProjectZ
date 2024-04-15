// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "Game/ZGameModeBase.h"
#include "ZPlayerCharacter.generated.h"

class USkillRangeComponent;
class UCostPathLengthWidget;
class USplineComponent;
class USplineMeshComponent;
/**
 * 
 */
UCLASS()
class PROJECTZ_API AZPlayerCharacter : public AZCharacterBase
{
	GENERATED_BODY()

public:
	AZPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void InitAbilityActorInfo() override;

	virtual int32 GetLevel() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SkillRangeMesh;

	int32 GetPathCost();
	float GetPathLength();

	UFUNCTION(BlueprintCallable)
	void ShowSKillRange(float Angle, float Range);
	
	UFUNCTION(BlueprintCallable)
	void HideSkillRange();

protected:
	virtual void BeginPlay() override;
private:
	bool bIsPossessed = false;

	UPROPERTY(EditAnywhere)
	USplineComponent* Spline;

	TArray<USplineMeshComponent*> SplineMesh;

	UPROPERTY(EditAnywhere, Category = Spline)
	UStaticMesh* DefaultMesh;

	UPROPERTY(EditAnywhere, Category = Spline)
	float SplineHeight = 20.f;

	UPROPERTY(EditAnywhere, Category = Spline)
	FVector2D SplineScale = FVector2D(0.02f, 0.02f);

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	TSubclassOf<UStaticMeshComponent> SkillRangedMeshComponentClass;

	UPROPERTY(EditAnywhere, Category = "Skill Range")
	UMaterial* SkillRangeMaterial; 

	TArray<USkillRangeComponent*> SkillRangeComponents;

	void UpdateSplinePath();

	float SplineLength;

	UFUNCTION()
	void TurnChanged(ETurn Turn);

	void SetSkillRangePos();
};
