// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PROJECTZ_API ADoor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADoor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetParams(int Count, int number);
	UFUNCTION(BlueprintCallable) void SpawnRoom();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int DoorCount = 1;
	int RoomNo = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* DefaultRoot;
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* Right;
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* Left;

};