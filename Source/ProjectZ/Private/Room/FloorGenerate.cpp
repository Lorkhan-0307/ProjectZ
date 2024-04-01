// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/FloorGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"

UStaticMesh* CubeMesh3;
AActor* CurrentDoor;

void AFloorGenerate::BasicRoom()
{
	int adjacent[4] = {-2, -2};
	// Reset all tiles, walls, doors
	Tile->ClearInstances();
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

	// Random Generate Floor Plans
	Floor floor(floorWidth, floorHeight);
	floor.generateRooms(numRooms);
	std::vector<std::vector<int>> floorPlan(floorHeight, std::vector<int>(floorWidth, -1));
	floorPlan = floor.makePlan();
	
	// Materialize Floor From Floor Plans
	for(int i=0;i<floorHeight;i++)
	{
		for(int j=0;j<floorWidth;j++)
		{
			if(floorPlan[i][j] != -1)
			{
				Tile->AddInstance(FTransform(FVector(i*100,j*100,0)));
				if(i==0) YWall->AddInstance(FTransform(FVector(100, 500+j*1000, 0)));
				if(i==floorHeight-1) YWall->AddInstance(FTransform(FVector(floorHeight*100, 1500+j*1000, 0)));
				if(j==0) XWall->AddInstance(FTransform(FVector(i*100, 0, 0)));
				if(j==floorWidth-1) XWall->AddInstance(FTransform(FVector(1500+i*1000, floorWidth*100,0)));
			}
			if(i && floorPlan[i][j] != floorPlan[i-1][j])
			{
				if(floorPlan[i][j] != -1 && floorPlan[i-1][j] != -1 && (adjacent[0] != floorPlan[i][j] || adjacent[1] != floorPlan[i-1][j]))
				{
					adjacent[0] = floorPlan[i][j];
					adjacent[1] = floorPlan[i-1][j];
					CreateDoor(i*120+60, j*120+60, true);
				}
				else YWall->AddInstance(FTransform(FVector(600+i*1200, 100+j*100, 0)));
			}
		}
	}
	for(int j=0;j<floorWidth;j++)
	{
		for(int i=0;i<floorHeight;i++)
		{
			if(j && floorPlan[i][j] != floorPlan[i][j-1])
			{
				if(floorPlan[i][j] != -1 && floorPlan[i][j-1] != -1 && (adjacent[0] != floorPlan[i][j] || adjacent[1] != floorPlan[i][j-1]))
				{
					adjacent[0] = floorPlan[i][j];
					adjacent[1] = floorPlan[i][j-1];
					CreateDoor(i*120+60, j*120+60, false);
				}
				else XWall->AddInstance(FTransform(FVector(100+i*100, 600+j*1200, 0)));
			}
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
    XWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("XWall"));
	YWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("YWall"));
    CubeMesh3 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
    SetRootComponent(DefaultRoot);
    Tile->SetupAttachment(GetRootComponent());
    XWall->SetupAttachment(GetRootComponent());
	YWall->SetupAttachment(GetRootComponent());
    Tile->SetStaticMesh(CubeMesh3);
    Tile->SetWorldScale3D(FVector(1.2, 1.2, 0.1));
    Tile->SetRelativeLocation(FVector(120, 120, 0));
    XWall->SetStaticMesh(CubeMesh3);
    XWall->SetWorldScale3D(FVector(1.2, 0.1, 2.0));
    XWall->SetRelativeLocation(FVector(0, 0, 100));
	YWall->SetStaticMesh(CubeMesh3);
	YWall->SetWorldScale3D(FVector(0.1, 1.2, 2.0));
	YWall->SetRelativeLocation(FVector(0, 0, 100));
	AttachedDoor = nullptr;
}

void AFloorGenerate::SetDoor(TSubclassOf<UObject> Door)
{
	AttachedDoor = Door;
}

void AFloorGenerate::CreateDoor(float x, float y, bool isVertical)
{
	if(isVertical)
	{
		CurrentDoor = GetWorld()->SpawnActor<AActor>(AttachedDoor, FTransform(FRotator(0, 90, 0), FVector(x, y, 10)+GetActorLocation()));
	}
	else
	{
		CurrentDoor = GetWorld()->SpawnActor<AActor>(AttachedDoor, FTransform(FVector(x, y, 10)+GetActorLocation()));
	}
	DoorArray.Add(CurrentDoor);
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

