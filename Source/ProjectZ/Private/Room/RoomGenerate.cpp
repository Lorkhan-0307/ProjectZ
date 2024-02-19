// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/RoomGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

UStaticMesh* CubeMesh;

// Generates rectangle room with floor, wall
void ARoomGenerate::BasicRoom()
{
	Floor->ClearInstances();
	XWall->ClearInstances();
	YWall->ClearInstances();
	for(AActor* Actor : DoorArray)
	{
		if(Actor)
		{
			Actor->Destroy();
		}
	}
	DoorArray.Empty();
	RoomX = FMath::RandRange(1, 10);
	RoomY = FMath::RandRange(1, 10);
	for(int i=0; i<RoomX; i++)
	{
		for(int j=0; j<RoomY; j++)
		{
			Floor->AddInstance(FTransform(FVector(i*TileX, j*TileY, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<AActor>(Door, FTransform(FVector(i*TileX*2+100, 0, 0)+GetActorLocation())));
		}
		else
		{
			XWall->AddInstance(FTransform(FVector(i*TileX, 0, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<AActor>(Door, FTransform(FVector(i*TileX*2+100, RoomY*TileY*2, 0)+GetActorLocation())));
		}
		else
		{
			XWall->AddInstance(FTransform(FVector(i*TileX, RoomY*TileY*20, 0)));
		}
	}
	for(int i=0; i<RoomY; i++)
	{
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<AActor>(Door, FTransform(FRotator(0, 90, 0), FVector(0, i*TileY*2+100, 0)+GetActorLocation())));
		}
		else
		{
			YWall->AddInstance(FTransform(FVector(0, i*TileY, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<AActor>(Door, FTransform(FRotator(0, 90, 0), FVector(RoomX*TileX*2, i*TileY*2+100, 0)+GetActorLocation())));
		}
		else
		{
			YWall->AddInstance(FTransform(FVector(RoomX*TileX*20, i*TileY, 0)));
		}
	}
}

// Sets default values
ARoomGenerate::ARoomGenerate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultRoot"));
	Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	XWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("XWall"));
	YWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("YWall"));
	CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	SetRootComponent(DefaultRoot);
	Floor->SetupAttachment(GetRootComponent());
	XWall->SetupAttachment(GetRootComponent());
	YWall->SetupAttachment(GetRootComponent());
	RoomX = 1;
	RoomY = 1;
	
	Floor->SetStaticMesh(CubeMesh);
	Floor->SetWorldScale3D(FVector(2, 2, 0.1));
	Floor->SetRelativeLocation(FVector(100, 100, 0));
	XWall->SetStaticMesh(CubeMesh);
	XWall->SetWorldScale3D(FVector(2, 0.1, 2));
	XWall->SetRelativeLocation(FVector(100, 0, 100));
	YWall->SetStaticMesh(CubeMesh);
	YWall->SetWorldScale3D(FVector(0.1, 2, 2));
	YWall->SetRelativeLocation(FVector(0, 100, 100));
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

