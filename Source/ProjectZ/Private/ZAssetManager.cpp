// Fill out your copyright notice in the Description page of Project Settings.


#include "ZAssetManager.h"
#include "ZGameplayTag.h"

UZAssetManager& UZAssetManager::Get()
{
	UZAssetManager* ZAssetManager = Cast<UZAssetManager>(GEngine->AssetManager);
	return *ZAssetManager;
}

void UZAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FZGameplayTag::InitializeNativeGameplayTags();
}
