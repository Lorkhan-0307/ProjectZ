// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/RoomGenerate.h"
#include "Room/Door.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

UStaticMesh* CubeMesh;
ADoor* CurDoor;

// Generates rectangle room with floor, wall, door
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
	RoomX = FMath::RandRange(3, 8);
	RoomY = FMath::RandRange(3, 8);
	DoorRemainder = FMath::RandRange(0, DoorEvery-1);
	for(int i=0; i<RoomX; i++)
	{
		for(int j=0; j<RoomY; j++)
		{
			Floor->AddInstance(FTransform(FVector(i*TileX, j*TileY, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			CurDoor = GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FVector(i*TileX*2+100, 0, 0)+GetActorLocation()));
			DoorArray.Add(CurDoor);
		}
		else
		{
			XWall->AddInstance(FTransform(FVector(i*TileX, 0, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FRotator(0, 180, 0), FVector(i*TileX*2+100, RoomY*TileY*2, 0)+GetActorLocation())));
		}
		else
		{
			XWall->AddInstance(FTransform(FVector(i*TileX, RoomY*TileY*20, 0)));
		}
	}
	if(Starter)
	{
		YWall->AddInstance(FTransform(FVector(0, 0, 0)));
	}
	else
	{
		DoorArray.Add(GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FRotator(0, 90, 0), FVector(0, 100, 0)+GetActorLocation())));
	}
	if(UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
	{
		DoorArray.Add(GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FRotator(0, 270, 0), FVector(RoomX*TileX*2, 100, 0)+GetActorLocation())));
	}
	else
	{
		YWall->AddInstance(FTransform(FVector(RoomX*TileX*20, 0, 0)));
	}
	for(int i=1; i<RoomY; i++)
	{
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FRotator(0, 90, 0), FVector(0, i*TileY*2+100, 0)+GetActorLocation())));
		}
		else
		{
			YWall->AddInstance(FTransform(FVector(0, i*TileY, 0)));
		}
		if(i%DoorEvery == DoorRemainder && UKismetMathLibrary::RandomBoolWithWeightFromStream(float(DoorCount - DoorArray.Num()) / DoorCount, RoomStream))
		{
			DoorArray.Add(GetWorld()->SpawnActor<ADoor>(ADoor::StaticClass(), FTransform(FRotator(0, 90, 0), FVector(RoomX*TileX*2, i*TileY*2+100, 0)+GetActorLocation())));
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

