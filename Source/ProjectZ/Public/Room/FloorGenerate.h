// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorGenerate.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int floorWidth=50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int floorHeight=20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Settings") int numRooms=9;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Component") UStaticMeshComponent* DefaultRoot;
	UPROPERTY(EditAnywhere, Category = "Component") UInstancedStaticMeshComponent* Tile;
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
    std::vector<Room> rooms;

public:
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
            if ((newRoom.x == room.x + room.width || newRoom.x + newRoom.width == room.x) &&
                (newRoom.y >= room.y && newRoom.y <= room.y + room.height || room.y >= newRoom.y && room.y <= newRoom.y + newRoom.height)) {
                return true;
            }
            if ((newRoom.y == room.y + room.height || newRoom.y + newRoom.height == room.y) &&
                (newRoom.x >= room.x && newRoom.x <= room.x + room.width || room.x >= newRoom.x && room.x <= newRoom.x + newRoom.width)) {
                return true;
            }
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
        std::vector<std::vector<char>> floorPlan(height, std::vector<char>(width, '.'));

        // Print floor plan
        for (const auto& room : rooms) {
            for (int i = room.x; i < room.x + room.width; ++i) {
                for (int j = room.y; j < room.y + room.height; ++j) {
                    floorPlan[j][i] = '0' + room.number;
                }
            }
        }

        for (const auto& row : floorPlan) {
            for (char c : row) {
                std::cout << c;
            }
            std::cout << std::endl;
        }
    }
    std::vector<std::vector<char>> makePlan() const
    {
        std::vector<std::vector<char>> floorPlan(height, std::vector<char>(width, '.'));

        // Print floor plan
        for (const auto& room : rooms) {
            for (int i = room.x; i < room.x + room.width; ++i) {
                for (int j = room.y; j < room.y + room.height; ++j) {
                    floorPlan[j][i] = '0' + room.number;
                }
            }
        }
        return floorPlan;
    }
};