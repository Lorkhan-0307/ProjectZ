// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/FloorGenerate.h"
#include "Components/InstancedStaticMeshComponent.h"

UStaticMesh* CubeMesh3;
AActor* CurrentDoor;

void AFloorGenerate::BasicRoom()
{
	// Variables used for creating doors
	int adjacent[2] = {-2, -2};
	std::vector<std::vector<float>> doorCandidate;
	
	// Reset all tiles, walls, doors, furniture
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
	for(AActor* Actor : FurniturePlaced)
	{
		if(Actor)
		{
			Actor->Destroy();
		}
	}
	FurniturePlaced.Empty();

	// Random Generate Floor Plans
	Floor floor(floorWidth, floorHeight);
	floor.generateRooms(numRooms);
	std::vector<std::vector<int>> floorPlan(floorHeight, std::vector<int>(floorWidth, -1));
	floorPlan = floor.makePlan();
	furniturePlan = floorPlan;
	
	// Materialize Floor, Wall & Door From Floor Plans
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
				if(floorPlan[i][j] != -1 && floorPlan[i-1][j] != -1)
				{
					if(adjacent[0] != floorPlan[i][j] || adjacent[1] != floorPlan[i-1][j])
					{
						adjacent[0] = floorPlan[i][j];
						adjacent[1] = floorPlan[i-1][j];
						if(doorCandidate.size() != 0)
						{
							int rnd = rand() % doorCandidate.size();
							doorCandidate[rnd][4] = 1.0f;
							for(int t = 5 + rand() % 3;(rnd + t)<doorCandidate.size();t += (5 + rand() % 3))
							{
								doorCandidate[rnd + t][4] = 1.0f;
							}
							for(int t = 5 + rand() % 3;(rnd - t) >= 0;t += (5 + rand() % 3))
							{
								doorCandidate[rnd - t][4] = 1.0f;
							}
						}
						for(std::vector<float> dw : doorCandidate)
						{
							if(dw[4] == 1.0f) CreateDoor(dw[0], dw[1], true);
							else YWall->AddInstance(FTransform(FVector(dw[2], dw[3], 0)));
						}
						doorCandidate.clear();
						doorCandidate.push_back({i*120+60.0f, j*120+60.0f, i*1200+600.0f, j*100+100.0f, 0.0f});
					}
					else
					{
						doorCandidate.push_back({i*120+60.0f, j*120+60.0f, i*1200+600.0f, j*100+100.0f, 0.0f});
					}
				}
				else YWall->AddInstance(FTransform(FVector(600+i*1200, 100+j*100, 0)));
			}
		}
	}
	if(doorCandidate.size() != 0)
	{
		int rnd = rand() % doorCandidate.size();
		doorCandidate[rnd][4] = 1.0f;
		for(int t = 5 + rand() % 3;(rnd + t)<doorCandidate.size();t += (5 + rand() % 3))
		{
			doorCandidate[rnd + t][4] = 1.0f;
		}
		for(int t = 5 + rand() % 3;(rnd - t) >= 0;t += (5 + rand() % 3))
		{
			doorCandidate[rnd - t][4] = 1.0f;
		}
	}
	for(std::vector<float> dw : doorCandidate)
	{
		if(dw[4] == 1.0f) CreateDoor(dw[0], dw[1], true);
		else YWall->AddInstance(FTransform(FVector(dw[2], dw[3], 0)));
	}
	doorCandidate.clear();
	adjacent[0] = -2;
	adjacent[1] = -2;
	for(int j=0;j<floorWidth;j++)
	{
		for(int i=0;i<floorHeight;i++)
		{
			if(j && floorPlan[i][j] != floorPlan[i][j-1])
			{
				if(floorPlan[i][j] != -1 && floorPlan[i][j-1] != -1)
				{
					if(adjacent[0] != floorPlan[i][j] || adjacent[1] != floorPlan[i][j-1])
					{
						adjacent[0] = floorPlan[i][j];
						adjacent[1] = floorPlan[i][j-1];
						if(doorCandidate.size() != 0)
						{
							int rnd = rand() % doorCandidate.size();
							doorCandidate[rnd][4] = 1.0f;
							for(int t = 5 + rand() % 3;(rnd + t)<doorCandidate.size();t += (5 + rand() % 3))
							{
								doorCandidate[rnd + t][4] = 1.0f;
							}
							for(int t = 5 + rand() % 3;(rnd - t) >= 0;t += (5 + rand() % 3))
							{
								doorCandidate[rnd - t][4] = 1.0f;
							}
						}
						for(std::vector<float> dw : doorCandidate)
						{
							if(dw[4] == 1.0f) CreateDoor(dw[0], dw[1], false);
							else XWall->AddInstance(FTransform(FVector(dw[2], dw[3], 0)));
						}
						doorCandidate.clear();
						doorCandidate.push_back({i*120+60.0f, j*120+60.0f, i*100+100.0f, j*1200+600.0f, 0.0f});
					}
					else
					{
						doorCandidate.push_back({i*120+60.0f, j*120+60.0f, i*100+100.0f, j*1200+600.0f, 0.0f});
					}
				}
				else XWall->AddInstance(FTransform(FVector(100+i*100, 600+j*1200, 0)));
			}
		}
	}
	if(doorCandidate.size() != 0)
	{
		int rnd = rand() % doorCandidate.size();
		doorCandidate[rnd][4] = 1.0f;
		for(int t = 5 + rand() % 3;(rnd + t)<doorCandidate.size();t += (5 + rand() % 3))
		{
			doorCandidate[rnd + t][4] = 1.0f;
		}
		for(int t = 5 + rand() % 3;(rnd - t) >= 0;t += (5 + rand() % 3))
		{
			doorCandidate[rnd - t][4] = 1.0f;
		}
	}
	for(std::vector<float> dw : doorCandidate)
	{
		if(dw[4] == 1.0f) CreateDoor(dw[0], dw[1], false);
		else XWall->AddInstance(FTransform(FVector(dw[2], dw[3], 0)));
	}

	// Guarantee at least one way through every doors
	for(Room rm : floor.rooms)
	{
		int xp = 1 + rm.x + rand() % (rm.width - 2);
		int yp = 1 + rm.y + rand() % (rm.height - 2);
		furniturePlan[yp][xp] = -3;
		std::vector<std::vector<int>> DoorsInRoom;
		for(int i=rm.y;i<rm.y + rm.height;i++)
		{
			for(int j=rm.x;j<rm.x + rm.width;j++)
			{
				if(furniturePlan[i][j] == -2) DoorsInRoom.push_back({j, i});
			}
		}
		for(std::vector<int> doorTile : DoorsInRoom)
		{
			int txp = xp;
			int typ = yp;
			int dx = abs(doorTile[0] - txp);
			int dy = abs(doorTile[1] - typ);
			int sx = txp < doorTile[0] ? 1 : -1;
			int sy = typ < doorTile[1] ? 1 : -1;
			int err = (dx > dy ? dx : -dy) / 2;
			int e2;
			while(typ != doorTile[1] || txp != doorTile[0])
			{
				if(furniturePlan[typ][txp] != -3) furniturePlan[typ][txp] = -4;
				e2 = err;
				if(e2 > -dx)
				{
					err -= dy;
					txp += sx;
				}
				if(e2 < dy)
				{
					err += dx;
					typ += sy;
				}
			}
		}
	}

	// Place Furniture
	for(FFurnitureData fd : FurnitureList)
	{
		for(int i=0;i<floorHeight-fd.Height;i++)
		{
			for(int j=0;j<floorWidth-fd.Width;j++)
			{
				if(furniturePlan[i][j] <= -1) continue;
				bool fits = true;
				for(int k=0;k<fd.Height;k++)
				{
					for(int l=0;l<fd.Width;l++)
					{
						if(furniturePlan[i+k][j+l] != furniturePlan[i][j])
						{
							fits = false;
							break;
						}
					}
				}
				if(fits && rand()%20 == 0)
				{
					bool added = false;
					if(fd.StickToWall)
					{
						if(rand()%2 == 0 && j > 0 && floorPlan[i][j] != floorPlan[i][j-1])
						{
							FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FVector(i*120+60+60*fd.Height, j*120+60+60*fd.Width, 0)+GetActorLocation())));
							added = true;
						}
						else if(j + fd.Width < floorWidth && floorPlan[i][j] != floorPlan[i][j+fd.Width])
						{
							FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 180, 0), FVector(i*120+60+60*fd.Height, j*120+60+60*fd.Width, 0)+GetActorLocation())));
							added = true;
						}
					}
					else
					{
						if(rand()%2 == 0) FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FVector(i*120+60+60*fd.Height, j*120+60+60*fd.Width, 0)+GetActorLocation())));
						else FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 180, 0), FVector(i*120+60+60*fd.Height, j*120+60+60*fd.Width, 0)+GetActorLocation())));
						added = true;
					}
					if(added)
					{
						for(int k=0;k<fd.Height;k++)
						{
							for(int l=0;l<fd.Width;l++)
							{
								furniturePlan[i+k][j+l] = -1;
							}
						}
					}
				}
			}
		}
		for(int i=0;i<floorHeight-fd.Width;i++)
		{
			for(int j=0;j<floorWidth-fd.Height;j++)
			{
				if(furniturePlan[i][j] <= -1) continue;
				bool fits = true;
				for(int k=0;k<fd.Width;k++)
				{
					for(int l=0;l<fd.Height;l++)
					{
						if(furniturePlan[i+k][j+l] != furniturePlan[i][j])
						{
							fits = false;
							break;
						}
					}
				}
				if(fits && rand()%20 == 0)
				{
					bool added = false;
					if(fd.StickToWall)
					{
						if(rand()%2 == 0 && i > 0 && floorPlan[i][j] != floorPlan[i-1][j])
						{
							FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 270, 0),FVector(i*120+60+60*fd.Width, j*120+60+60*fd.Height, 0)+GetActorLocation())));
							added = true;
						}
						else if(i + fd.Width < floorHeight && floorPlan[i][j] != floorPlan[i+fd.Width][j])
						{
							FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 90, 0), FVector(i*120+60+60*fd.Width, j*120+60+60*fd.Height, 0)+GetActorLocation())));
							added = true;
						}
					}
					else
					{
						if(rand()%2 == 0) FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 270, 0), FVector(i*120+60+60*fd.Width, j*120+60+60*fd.Height, 0)+GetActorLocation())));
						else FurniturePlaced.Add(GetWorld()->SpawnActor<AActor>(fd.Furniture, FTransform(FRotator(0, 90, 0), FVector(i*120+60+60*fd.Width, j*120+60+60*fd.Height, 0)+GetActorLocation())));
						added = true;
					}
					if(added)
					{
						for(int k=0;k<fd.Width;k++)
						{
							for(int l=0;l<fd.Height;l++)
							{
								furniturePlan[i+k][j+l] = -1;
							}
						}
					}
				}
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

void AFloorGenerate::CreateDoor(float x, float y, bool isVertical)
{
	if(isVertical)
	{
		CurrentDoor = GetWorld()->SpawnActor<AActor>(AttachedDoor, FTransform(FRotator(0, 90, 0), FVector(x, y, 10)+GetActorLocation()));
		furniturePlan[(x-60)/120-1][(y-60)/120] = -2;
	}
	else
	{
		CurrentDoor = GetWorld()->SpawnActor<AActor>(AttachedDoor, FTransform(FVector(x, y, 10)+GetActorLocation()));
		furniturePlan[(x-60)/120][(y-60)/120-1] = -2;
	}
	furniturePlan[(x-60)/120][(y-60)/120] = -2;
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

