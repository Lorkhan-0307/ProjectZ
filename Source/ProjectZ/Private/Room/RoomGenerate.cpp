// Fill out your copyright notice in the Description page of Project Settings.


#include "Room/RoomGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"

UStaticMesh* CubeMesh;

// Generates rectangle room with floor, wall, door
void ARoomGenerate::BasicRoom()
{
	for(int i=0; i<RoomX; i++)
	{
		for(int j=0; j<RoomY; j++)
		{
			Floor->AddInstance(FTransform(FVector(i*TileX, j*TileY, 0)));
		}
		Wall->AddInstance(FTransform(FVector(i*TileX, 0, 0)));
		Wall->AddInstance(FTransform(FVector(i*TileX, RoomY*TileY*20, 0)));
	}
}

// Sets default values
ARoomGenerate::ARoomGenerate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultRoot"));
	Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	SetRootComponent(DefaultRoot);
	Floor->SetupAttachment(GetRootComponent());
	Wall->SetupAttachment(GetRootComponent());
	
	Floor->SetStaticMesh(CubeMesh);
	Floor->SetWorldScale3D(FVector(2, 2, 0.1));
	Floor->SetRelativeLocation(FVector(100, 100, 0));
	Wall->SetStaticMesh(CubeMesh);
	Wall->SetWorldScale3D(FVector(2, 0.1, 2));
	Wall->SetRelativeLocation(FVector(100, 0, 100));

	RoomX = 1;
	RoomY = 1;
}

// Called when the game starts or when spawned
void ARoomGenerate::BeginPlay()
{
	Super::BeginPlay();
	BasicRoom();
}

// Called every frame
void ARoomGenerate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

