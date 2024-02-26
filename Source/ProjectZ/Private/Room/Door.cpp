// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/Door.h"
#include "Room/RoomGenerate.h"

ARoomGenerate* Room;

void ADoor::SetDoorCount(int Count)
{
	DoorCount = Count;
}

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	Room = GetWorld()->SpawnActor<ARoomGenerate>(ARoomGenerate::StaticClass(), FTransform(FRotator(0, -90, 0)+GetActorRotation(), FVector(-100, 0, 0)+GetActorLocation()));
	Room->BasicRoom();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

