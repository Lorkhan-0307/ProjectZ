// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Card/CardWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"
// TO DO : Replace to CombatPlayerController
#include "AbilitySystem/ZAttributeSet.h"
#include "Player/ZNonCombatPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Game/ZGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ZUserWidget.h"
#include "UI/Card/CardDragDropOperation.h"
#include "Ui/Card/CardHandWidget.h"
#include "UI/HUD/ZHUDBase.h"
#include "UI/NonCombat/InventoryWidget.h"
#include "UI/WidgetController/InventoryWidgetController.h"

// Initialize Card UI by FCard
void UCardWidget::InitCardStatus(FCard CardStatus, bool bSetDelegate)
{
	CardStat = CardStatus;

	CardName->SetText(CardStatus.CardDisplayName);
	//CardImage->SetBrushFromTexture(CardStatus.CardImage);
	AtkText->SetText(FText::FromString(FString::FromInt(CardStatus.CardAtk)));
	DefText->SetText(FText::FromString(FString::FromInt(CardStatus.CardCost)));
}

void UCardWidget::DestroyWidget()
{
	RemoveFromParent();
	CollectGarbage(EObjectFlags::RF_BeginDestroyed);
}
