// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Blueprint/UserWidget.h"
#include "Game/ZGameModeBase.h"
#include "CharacterPortraitWidget.generated.h"

class UTextBlock;
class UOverlay;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCharacterPortraitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterPortraitImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;

	UPROPERTY(meta = (BindWidget))
	UOverlay* PortraitOverlay;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TestText;

	FVector2D DestinationLocation;

	UPROPERTY(EditAnywhere)
	float InterpSpeed;

	ECharacterClass CharacterClass;

	UPROPERTY()
	AActor* CharacterActor;

	void InitPortrait(AActor* Actor);
	void DestroyWidget();

private:
	void SetLocation(float DeltaTime);

	UFUNCTION()
	void TurnChanged(ETurn Turn);
};
