// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ZHUD.h"
#include "Blueprint/UserWidget.h"
#include "HUD/Card/CardHandWidget.h"
#include "GameFramework/PlayerController.h"

void AZHUD::BeginPlay()
{
	Super::BeginPlay();
	//AddCardHandHUDOverlay();
	//GetOwningPlayerController()->GetViewportSize(ViewportSizeX,ViewportSizeY);
}

void AZHUD::AddCardHandHUDOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CardHandHUDClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CardHandHUD Created"));
		CardHandHUDWidget = CreateWidget<UCardHandWidget>(PlayerController, CardHandHUDClass);
		PlayerController->GetViewportSize(ViewportSizeX,ViewportSizeY);
		CardHandHUDWidget->SetViewportSize(FVector2D(ViewportSizeX,ViewportSizeY));
		CardHandHUDWidget->AddToViewport();
	}
}
