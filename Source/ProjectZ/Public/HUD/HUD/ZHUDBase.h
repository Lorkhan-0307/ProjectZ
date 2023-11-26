// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ZHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	virtual void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
};
