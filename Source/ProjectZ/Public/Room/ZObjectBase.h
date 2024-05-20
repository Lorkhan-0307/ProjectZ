// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZCharacterBase.h"
#include "GameFramework/Actor.h"
#include "Data/Card.h"
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

	virtual void NotifyActorBeginCursorOver() override;
	virtual void NotifyActorEndCursorOver() override;
	TArray<UStaticMeshComponent*> MeshComponents;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	AZCharacterBase* ZCharacter;
	// Object Inventory : Contains Cards
	UPROPERTY(EditAnywhere) TArray<FName> ObjectInventory;
	UPROPERTY(EditAnywhere) bool Interactable;
};
