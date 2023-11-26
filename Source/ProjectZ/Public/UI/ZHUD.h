// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZHUD.generated.h"

class UCardHandWidget;
class UCardWidget;
UCLASS()
class PROJECTZ_API AZHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Card Widget")
	TSubclassOf<UUserWidget> CardHandHUDClass;

	UPROPERTY()
	UCardHandWidget* CardHandHUDWidget;

	FORCEINLINE FVector2D GetViewPortSize() const {return FVector2D(ViewportSizeX,ViewportSizeY);}
	
protected:
	virtual void BeginPlay() override;
	
	void AddCardHandHUDOverlay();

private:
	int32 ViewportSizeX;
	int32 ViewportSizeY;
};
