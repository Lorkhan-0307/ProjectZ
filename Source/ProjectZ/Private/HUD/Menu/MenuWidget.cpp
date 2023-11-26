// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Menu/MenuWidget.h"

#include "Components/Button.h"
#include "HUD/ZHUD.h"
#include "Kismet/GameplayStatics.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameButton->OnClicked.AddDynamic(this,&UMenuWidget::NewGame);
	QuitGameButton->OnClicked.AddDynamic(this,&UMenuWidget::QuitGame);
}

void UMenuWidget::NewGame()
{
	GetOwningPlayer()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(this,NewGameMapName);
}

void UMenuWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this,GetOwningPlayer(),EQuitPreference::Quit,true);
}


