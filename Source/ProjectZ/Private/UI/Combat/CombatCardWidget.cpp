// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Combat/CombatCardWidget.h"

#include "Blueprint/DragDropOperation.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/Card.h"
#include "Engine/DataTable.h"
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


void UCombatCardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatCardWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot)
	{
		if (CanvasPanelSlot->GetPosition() != DestinationPosition) SetPosition(InDeltaTime);
	}

	//TrashCard(InDeltaTime);
}

void UCombatCardWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	bMouseHovered = true;
	if ((CardStat.CardType == ECardType::ECT_Skill || CanvasPanelSlot == nullptr) && CardStat.IsValid)
	{
		AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(GetOwningPlayerPawn());
		PlayerCharacter->ShowSKillRange(CardStat.SkillAngle, CardStat.SkillRange);
	}

	if (CardStat.IsValid)
	{
		CardOutlineImage->SetBrushTintColor(FSlateColor(HoveredColor));
	}
}


void UCombatCardWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bMouseHovered = false;
	AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(GetOwningPlayerPawn());
	if (CardStat.IsValid) PlayerCharacter->HideSkillRange();
	if (CardStat.IsValid)
	{
		CardOutlineImage->SetBrushTintColor(FSlateColor(FColor(255, 255, 255)));
	}
}

FReply UCombatCardWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	UHorizontalBoxSlot* HorizontalBoxSlot = UWidgetLayoutLibrary::SlotAsHorizontalBoxSlot(this);
	if (HorizontalBoxSlot)
	{
		Cast<UInventoryWidget>(Cast<AZHUDBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD())->InventoryWidget)->DisplayCardWidget(CardStat);
	}
	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UCombatCardWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (CardStat.IsValid == false) return;

	// Don't Drag Card in NonCombat Turn
	AZGameModeBase* GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
	ETurn CurrentTurn = GameMode->GetCurrentTurn();
	if (!(CurrentTurn == ETurn::ET_MoveTurn || CurrentTurn == ETurn::ET_PlayerTurn)) return;

	UCardDragDropOperation* CardDragDropOperation = NewObject<UCardDragDropOperation>();

	CardDragDropOperation->Payload = this;
	UCardWidget* DragCardWidget = Cast<UCardWidget>(CreateWidget(GetOwningPlayer(), CardWidgetClass));
	DragCardWidget->InitCardStatus(CardStat);
	CardDragDropOperation->DefaultDragVisual = DragCardWidget;
	CardDragDropOperation->Pivot = EDragPivot::CenterCenter;

	SetVisibility(ESlateVisibility::Hidden);

	OutOperation = CardDragDropOperation;
	CardDragStartDelegate.Broadcast(this);
}


void UCombatCardWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	UZAttributeSet* AS = Cast<UZAttributeSet>(Cast<AZCharacterBase>(GetOwningPlayerPawn())->GetAttributeSet());

	bool CanEquip = CardStat.CardType == ECardType::ECT_OneHandWeapon || CardStat.CardType == ECardType::ECT_TwoHandWeapon;

	if (CanvasPanelSlot == nullptr) // Equipping Card
	{
		bool bBasicAttack;
		const bool bIsLeftHandCard = GetCachedGeometry().GetAbsolutePosition().X < UWidgetLayoutLibrary::GetViewportSize(this).X / 2.f;
		if (bIsLeftHandCard)
		{
			bBasicAttack = !(CardComponent->LeftEquipPosMin.ComponentwiseAllLessOrEqual(MousePosition) && MousePosition.ComponentwiseAllLessOrEqual(CardComponent->LeftEquipPosMax) && CanEquip);
		}
		else
		{
			bBasicAttack = !(CardComponent->RightEquipPosMin.ComponentwiseAllLessOrEqual(MousePosition) && MousePosition.ComponentwiseAllLessOrEqual(CardComponent->RightEquipPosMax) && CanEquip);
		}
		bBasicAttack = bBasicAttack && CardStat.CardCost <= AS->GetCost() && CardComponent->bActivatingCard == false;

		if (bBasicAttack)
		{
			//CardStat.IsValid = false;
			CardComponent->ActiveCard(CardStat, bIsLeftHandCard, true);
		}
		SetVisibility(ESlateVisibility::Visible);
		return;
	}


	bool bUseCard = MousePosition.Y < CardComponent->GetPlayCardHeight() && (CardStat.CardType == ECardType::ECT_Skill || CardStat.CardType == ECardType::ECT_Buff || CardStat.CardType == ECardType::ECT_UsablePassive); // Use Card
	bUseCard = bUseCard || (CardComponent->LeftEquipPosMin.ComponentwiseAllLessOrEqual(MousePosition) && MousePosition.ComponentwiseAllLessOrEqual(CardComponent->LeftEquipPosMax) && CanEquip); // Equip Card Left
	bUseCard = bUseCard || (CardComponent->RightEquipPosMin.ComponentwiseAllLessOrEqual(MousePosition) && MousePosition.ComponentwiseAllLessOrEqual(CardComponent->RightEquipPosMax) && CanEquip); // Equip Card Right
	bUseCard = bUseCard && CardStat.CardCost <= AS->GetCost() && CardComponent->bActivatingCard == false;

	if (bUseCard) // Use Card
	{
		CardComponent->ActiveCard(CardStat, (CardComponent->LeftEquipPosMin.ComponentwiseAllLessOrEqual(MousePosition) && MousePosition.ComponentwiseAllLessOrEqual(CardComponent->LeftEquipPosMax) && CanEquip));
		AZGameModeBase* GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode->GetCurrentTurn() == ETurn::ET_MoveTurn)
		{
			GameMode->SetTurn(ETurn::ET_PlayerTurn);
		}
	}
	else
	{
		CancelActivateCard();
	}
}

// Initialize Card UI by FCard
void UCombatCardWidget::InitCardStatus(FCard CardStatus, bool bSetDelegate)
{
	Super::InitCardStatus(CardStatus, bSetDelegate);
	
	CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);

	if (CardComponent && bSetDelegate)
	{
		CardComponent->ActivateCardDelegate.AddDynamic(this, &UCombatCardWidget::DestroyActivateCard);
		CardComponent->CancelActivateCardDelegate.AddDynamic(this, &UCombatCardWidget::CancelActivateCard);
	}
}

// Set Position by Interpolation
void UCombatCardWidget::SetPosition(float DeltaTime)
{
	if (CanvasPanelSlot == nullptr) return;
	if (bTrashCard) return;
	CanvasPanelSlot->SetPosition(FMath::Vector2DInterpTo(CanvasPanelSlot->GetPosition(), DestinationPosition, DeltaTime, InterpSpeed));
	SetRenderTransformAngle(FMath::FInterpTo(GetRenderTransform().Angle, DestinationAngle, DeltaTime, InterpSpeed));
}

void UCombatCardWidget::DestroyActivateCard()
{
	if (!(GetVisibility() == ESlateVisibility::Hidden && CardStat.IsValid)) return;
	CardDragEndDelegate.Broadcast(this, true);

	DestroyWidget();

	/*
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetPosition(MousePosition - CanvasPanelSlot->GetSize() / 2.f);
		TrashCardStartPosition = CanvasPanelSlot->GetPosition();
		CanvasPanelSlot->SetSize(CardSize);
		SetRenderTransformAngle(0.f);

		FAnchors Anchors(1.f, 1.f);
		CanvasPanelSlot->SetAnchors(Anchors);
		CanvasPanelSlot->SetAlignment(FVector2D(1.f, 1.f));
		UE_LOG(LogTemp,Warning,TEXT("%s"),*CanvasPanelSlot->GetName());
	}
	SetVisibility(ESlateVisibility::Visible);
	CardStat.IsValid = false;
	bTrashCard = true;
	DestinationPosition = CardStat.CardType == ECardType::ECT_Skill ? CardComponent->DiscardedCardLocation : CardComponent->CardGraveyardLocation;
	*/
}

void UCombatCardWidget::CancelActivateCard()
{
	if (!(GetVisibility() == ESlateVisibility::Hidden && CardStat.IsValid)) return;
	CardDragEndDelegate.Broadcast(this, false);
	SetVisibility(ESlateVisibility::Visible);
	bMouseHovered = false;

	if (CardStat.CardType != ECardType::ECT_Skill) return;
	AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(GetOwningPlayerPawn());
	PlayerCharacter->HideSkillRange();
}

void UCombatCardWidget::TrashCard(float DeltaTime)
{
	CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
	if (CanvasPanelSlot == nullptr) return;
	if (bTrashCard == false) return;

	//SetPositionInViewport(FMath::Vector2DInterpConstantTo(GetCachedGeometry().GetAbsolutePosition(), TrashLocation, DeltaTime, InterpSpeed));
	CanvasPanelSlot->SetPosition(FMath::Vector2DInterpTo(CanvasPanelSlot->GetPosition(), DestinationPosition, DeltaTime, CardComponent->CardTrashSpeed));

	const float Alpha = (TrashCardStartPosition - CanvasPanelSlot->GetPosition()).Size() / (TrashCardStartPosition - DestinationPosition).Size();
	CanvasPanelSlot->SetSize(FMath::Lerp(CardSize, 70, Alpha));

	if (CanvasPanelSlot->GetPosition() == DestinationPosition)
	{
		CollectGarbage(EObjectFlags::RF_BeginDestroyed);
	}
}
