// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Card/CardHandHUD.h"
#include "HUD/Card/CardHandWidget.h"

void UCardHandHUD::NativeConstruct()
{
	Super::NativeConstruct();
	CardHandWidget->SetViewportSize(ViewportSize);
}
