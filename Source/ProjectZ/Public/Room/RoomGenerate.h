// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"
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
	// Generates rectangle room with floor, wall
	UFUNCTION(BlueprintCallable) void BasicRoom();
	UPROPERTY(EditDefaultsOnly)TSubclassOf<AActor>Door;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<AActor*> DoorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int RoomX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int RoomY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int DoorCount = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int TileX = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") int TileY = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings") FRandomStream RoomStream;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* DefaultRoot;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* Floor;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* XWall;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* YWall;
};
