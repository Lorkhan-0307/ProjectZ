// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_FightData.generated.h"

UCLASS()
class PROJECTZ_API UGI_FightData : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Data")
	int32 BoardWidth=8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Data")
	int32 BoardHeight=8;
};
