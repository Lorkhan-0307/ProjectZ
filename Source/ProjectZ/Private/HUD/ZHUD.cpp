// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ZHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/Card/CardHandWidget.h"

void AZHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCardHandHUDOverlay();
}

void AZHUD::AddCardHandHUDOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CardHandHUDClass)
	{
		UE_LOG(LogTemp,Warning,TEXT("CardHandHUD Created"));
		CardHandHUDWidget = CreateWidget<UCardHandWidget>(PlayerController,CardHandHUDClass);
		CardHandHUDWidget->AddToViewport();
	}
}