// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>
#include "CoreMinimal.h"
#include "Character/ZEnemy.h"
#include "GameFramework/Actor.h"
#include "FloorGenerate.generated.h"

USTRUCT(BlueprintType)
struct FFurnitureData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<AActor> Furniture;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Width;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 Height;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool StickToWall;
};

UCLASS()
class PROJECTZ_API AFloorGenerate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorGenerate();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    UFUNCTION(BlueprintCallable) void BasicRoom();
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<class UObject> AttachedDoor;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<FFurnitureData> FurnitureList;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<TSubclassOf<class AZEnemy>> EnemyList;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<AActor*> DoorArray;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<AActor*> FurniturePlaced;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<AActor*> EnemyPlaced;
    void CreateDoor(float x, float y, bool isVertical);
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int floorWidth=40;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int floorHeight=25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int numRooms=9;
    std::vector<std::vector<int>> furniturePlan;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* DefaultRoot;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* Tile;
    UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* Ceiling;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* XWall;
    UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* YWall;
};

struct Room {
	int number; // Added room number
	int x, y, width, height;

	Room(int number, int x, int y, int width, int height) : number(number), x(x), y(y), width(width), height(height) {}
};

class Floor {
private:
    int width, height;

public:
    std::vector<Room> rooms;
    Floor(int width, int height) : width(width), height(height) {}

    bool isOverlapping(const Room& newRoom) const {
        for (const auto& room : rooms) {
            if (newRoom.x < room.x + room.width && newRoom.x + newRoom.width > room.x &&
                newRoom.y < room.y + room.height && newRoom.y + newRoom.height > room.y) {
                return true;
            }
        }
        return false;
    }

    bool isAdjacent(const Room& newRoom) const {
        for (const auto& room : rooms) {
            int check = 0;
            if ((newRoom.x == room.x + room.width || newRoom.x + newRoom.width == room.x) &&
                (newRoom.y >= room.y && newRoom.y <= room.y + room.height || room.y >= newRoom.y && room.y <= newRoom.y + newRoom.height)) {
                check++;
            }
            if ((newRoom.y == room.y + room.height || newRoom.y + newRoom.height == room.y) &&
                (newRoom.x >= room.x && newRoom.x <= room.x + room.width || room.x >= newRoom.x && room.x <= newRoom.x + newRoom.width)) {
                check++;
            }
            if(check == 1) return true;
        }
        return false;
    }

    void generateRooms(int numRooms) {
        std::srand(std::time(nullptr));

        // Generate the first room without adjacency constraint
        int roomWidth = rand() % (width / 3) + 3;
        int roomHeight = rand() % (height / 3) + 3;
        int x = rand() % (width - roomWidth - 1) + 1;
        int y = rand() % (height - roomHeight - 1) + 1;

        Room firstRoom(1, x, y, roomWidth, roomHeight); // Assign room number 1
        rooms.push_back(firstRoom);

        // Generate remaining rooms with adjacency constraint and assign room numbers
        for (int i = 2; i <= numRooms; ++i) { // Start with room number 2
            int attempts = 0;
            while (true) {
                roomWidth = rand() % (width / 3) + 5;
                roomHeight = rand() % (height / 3) + 5;
                x = rand() % (width - roomWidth - 1) + 1;
                y = rand() % (height - roomHeight - 1) + 1;

                Room newRoom(i, x, y, roomWidth, roomHeight);

                if (!isOverlapping(newRoom) && isAdjacent(newRoom)) {
                    rooms.push_back(newRoom);
                    break;
                }

                // Limit attempts to prevent infinite loop
                attempts++;
                if (attempts > 100) {
                    break;
                }
            }
        }
    }
    void printFloor() const {
        std::vector<std::vector<int>> floorPlan(height, std::vector<int>(width, -1));

        // Print floor plan
        for (const auto& room : rooms) {
            for (int i = room.x; i < room.x + room.width; ++i) {
                for (int j = room.y; j < room.y + room.height; ++j) {
                    floorPlan[j][i] = room.number;
                }
            }
        }
    }
    std::vector<std::vector<int>> makePlan() const
    {
        std::vector<std::vector<int>> floorPlan(height, std::vector<int>(width, -1));

        // Print floor plan
        for (const auto& room : rooms) {
            for (int i = room.x; i < room.x + room.width; ++i) {
                for (int j = room.y; j < room.y + room.height; ++j) {
                    floorPlan[j][i] = room.number;
                }
            }
        }
        return floorPlan;
    }
};