// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZObjectBase.generated.h"

UCLASS()
class PROJECTZ_API AZObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() void OnBeginCursorOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION() void OnEndCursorOver(UPrimitiveComponent* TouchedComponent);

	void ApplyOutlineMaterial();
	void RemoveOutlineMaterial();

	UPROPERTY(VisibleAnywhere) UStaticMeshComponent* MeshComponent;

	UMaterialInterface* DefaultMaterial;
	UMaterialInterface* OutlineMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
