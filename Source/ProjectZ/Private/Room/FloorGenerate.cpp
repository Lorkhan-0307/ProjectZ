// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/FloorGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"

UStaticMesh* CubeMesh3;

void AFloorGenerate::BasicRoom()
{
	Floor floor(floorWidth, floorHeight);
	floor.generateRooms(numRooms);
	std::vector<std::vector<char>> floorPlan(floorHeight, std::vector<char>(floorWidth, '.'));
	floorPlan = floor.makePlan();
	Tile->ClearInstances();
	XWall->ClearInstances();
	YWall->ClearInstances();
	for(int i=0;i<floorHeight;i++)
	{
		for(int j=0;j<floorWidth;j++)
		{
			if(floorPlan[i][j] != '.')
			{
				Tile->AddInstance(FTransform(FVector(i*100,j*100,0)));
				if(i==0) YWall->AddInstance(FTransform(FVector(100, 500+j*1000, 0)));
				if(i==floorHeight-1) YWall->AddInstance(FTransform(FVector(floorHeight*100, 1500+j*1000, 0)));
				if(j==0) XWall->AddInstance(FTransform(FVector(i*100, 0, 0)));
				if(j==floorWidth-1) XWall->AddInstance(FTransform(FVector(1500+i*1000, floorWidth*100,0)));
			}
			if(i && floorPlan[i][j] != floorPlan[i-1][j]) YWall->AddInstance(FTransform(FVector(500+i*1000, 100+j*100, 0)));
			if(j && floorPlan[i][j] != floorPlan[i][j-1]) XWall->AddInstance(FTransform(FVector(100+i*100, 500+j*1000, 0)));
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
    Tile->SetWorldScale3D(FVector(1, 1, 0.1));
    Tile->SetRelativeLocation(FVector(100, 100, 0));
    XWall->SetStaticMesh(CubeMesh3);
    XWall->SetWorldScale3D(FVector(1, 0.1, 2.5));
    XWall->SetRelativeLocation(FVector(0, 0, 100));
	YWall->SetStaticMesh(CubeMesh3);
	YWall->SetWorldScale3D(FVector(0.1, 1, 2.5));
	YWall->SetRelativeLocation(FVector(0, 0, 100));
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

