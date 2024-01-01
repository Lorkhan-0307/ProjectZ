// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ZAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UZAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UZAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
