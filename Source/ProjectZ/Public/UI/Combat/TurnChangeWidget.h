// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/ZGameModeBase.h"
#include "TurnChangeWidget.generated.h"

class UTextBlock;
class AZGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterPortraitDelegate);

/**
 * 
 */
UCLASS()
class PROJECTZ_API UTurnChangeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FCharacterPortraitDelegate CharacterPortraitDelegate;
	
	void SetText();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnChangeText;

	FTimerHandle TurnTextTimer;
	
	UPROPERTY()
	AZGameModeBase* GameMode;

	ETurn CurrentTurn;
	ETurn LastTurn;

	UPROPERTY(EditAnywhere)
	float TurnTextDisplayTime = 1.f;

	UFUNCTION()
	void HideText();
};
