// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/FloorGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

UStaticMesh* CubeMesh3;

void AFloorGenerate::BasicRoom()
{
	Floor floor(floorWidth, floorHeight);
	floor.generateRooms(numRooms);
	std::vector<std::vector<char>> floorPlan(floorHeight, std::vector<char>(floorWidth, '.'));
	floorPlan = floor.makePlan();
	Tile->ClearInstances();
	Wall->ClearInstances();
	for(int i=0;i<floorHeight;i++)
	{
		for(int j=0;j<floorWidth;j++)
		{
			if(floorPlan[i][j] != '.') Tile->AddInstance(FTransform(FVector(i*100,j*100,0)));
		}
	}
}

// Sets default values
AFloorGenerate::AFloorGenerate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    DefaultRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultRoot"));
    Tile = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tile"));
    Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
    CubeMesh3 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
    SetRootComponent(DefaultRoot);
    Tile->SetupAttachment(GetRootComponent());
    Wall->SetupAttachment(GetRootComponent());
    Tile->SetStaticMesh(CubeMesh3);
    Tile->SetWorldScale3D(FVector(1, 1, 0.1));
    Tile->SetRelativeLocation(FVector(100, 100, 0));
    Wall->SetStaticMesh(CubeMesh3);
    Wall->SetWorldScale3D(FVector(2, 0.1, 2));
    Wall->SetRelativeLocation(FVector(100, 0, 100));
}

// Called when the game starts or when spawned
void AFloorGenerate::BeginPlay()
{
	Super::BeginPlay();
	BasicRoom();
}

// Called every frame
void AFloorGenerate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

