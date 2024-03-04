// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/Door.h"
#include "Room/RoomGenerate.h"

UStaticMesh* CubeMesh2;
ARoomGenerate* Room;

void ADoor::SetParams(int Count, int number)
{
	DoorCount = Count;
	RoomNo = number;
}

void ADoor::SpawnRoom()
{
	Room = GetWorld()->SpawnActor<ARoomGenerate>(ARoomGenerate::StaticClass(), FTransform(FRotator(0, -90, 0)+GetActorRotation(), FVector(-100, 0, 0)+GetActorLocation()));
	Room->SetParams(false, RoomNo + 1);
	Room->BasicRoom();
}

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultRoot"));
	Upper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upper"));
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	CubeMesh2 = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	SetRootComponent(DefaultRoot);
	Upper->SetupAttachment(GetRootComponent());
	Right->SetupAttachment(GetRootComponent());
	Left->SetupAttachment(GetRootComponent());

	Upper->SetStaticMesh(CubeMesh2);
	Upper->SetWorldScale3D(FVector(2, 0.1, 1));
	Upper->SetRelativeLocation(FVector(0, 0, 150));
	Right->SetStaticMesh(CubeMesh2);
	Right->SetWorldScale3D(FVector(0.5, 0.1, 1));
	Right->SetRelativeLocation(FVector(75, 0, 50));
	Left->SetStaticMesh(CubeMesh2);
	Left->SetWorldScale3D(FVector(0.5, 0.1, 1));
	Left->SetRelativeLocation(FVector(-75, 0, 50));
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}