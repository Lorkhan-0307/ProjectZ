// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomGenerate.generated.h"

UCLASS()
class PROJECTZ_API ARoomGenerate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomGenerate();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	int RoomX = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	int RoomY = 5;
};
