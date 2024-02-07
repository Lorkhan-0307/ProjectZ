// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "UI/Combat/CostPathLengthWidget.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "UI/Card/CardHandWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Game/ZGameModeBase.h"
#include "Widgets/SViewport.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	HealthCheckingBar->SetVisibility(ESlateVisibility::Visible);
	MentalityCheckingBar->SetVisibility(ESlateVisibility::Visible);
	TurnText->SetVisibility(ESlateVisibility::Hidden);
}

void UNonCombatOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateCardPosition();
	SetCardHandPosition(InDeltaTime);
	SetCostPathLengthWidgetPosition();
}

void UNonCombatOverlay::SetCardComponent(UCardComponent* CC)
{
	Super::SetCardComponent(CC);
	if (CardComponent)
	{
		CardComponent->UpdateLeftHandCardDelegate.AddDynamic(this, &UNonCombatOverlay::UpdateLeftHandCard);
		CardComponent->UpdateRightHandCardDelegate.AddDynamic(this, &UNonCombatOverlay::UpdateRightHandCard);
		CardComponent->DrawAndAddCardDelegate.AddDynamic(this, &UNonCombatOverlay::AddCardToHand);
		const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardHandSizeBox);
		CardComponent->SetPlayCardHeight(UWidgetLayoutLibrary::GetViewportSize(this).Y - CanvasPanelSlot->GetSize().Y * UWidgetLayoutLibrary::GetViewportScale(this));
		UE_LOG(LogTemp, Warning, TEXT("%f"), UWidgetLayoutLibrary::GetViewportSize(this).Y);
	}
}

void UNonCombatOverlay::WidgetControllerSet()
{
	Super::WidgetControllerSet();
	UOverlayWidgetController* OverlayWidgetController = Cast<UOverlayWidgetController>(WidgetController);
	if (OverlayWidgetController)
	{
		OverlayWidgetController->OnHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnHealthChanged);
		OverlayWidgetController->OnMaxHealthChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxHealthChanged);
		OverlayWidgetController->OnMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMentalityChanged);
		OverlayWidgetController->OnMaxMentalityChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxMentalityChanged);
	}

	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->TurnChangedDelegate.AddDynamic(this, &UNonCombatOverlay::TurnChanged);
}

void UNonCombatOverlay::UpdateLeftHandCard(FCard LeftCard)
{
	if (LeftCard.IsValid == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	LeftHandCardWidget->InitCardStatus(LeftCard);
}

void UNonCombatOverlay::UpdateRightHandCard(FCard RightCard)
{
	if (RightCard.IsValid == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	SetVisibility(ESlateVisibility::Visible);
	RightHandCardWidget->InitCardStatus(RightCard);
}

// Get Card, Create card widget
void UNonCombatOverlay::AddCardToHand(FCard NewCard)
{
	UCardWidget* CardWidget = CreateCardWidget(NewCard);
	HandCard.Add(CardWidget);
}

// Set Card Position
void UNonCombatOverlay::UpdateCardPosition()
{
	for (int i = 0; i < HandCard.Num(); i++)
	{
		HandCard[i]->DestinationPosition = FVector2D(GetCardXPosition(i), GetCardYPosition(i));
		HandCard[i]->DestinationAngle = GetCardAngle(i);
	}
}

// Create Card Widget by FCard
UCardWidget* UNonCombatOverlay::CreateCardWidget(FCard CardStatus)
{
	UCardWidget* CardWidget = CreateWidget<UCardWidget>(GetOwningPlayer(), CardWidgetClass);
	CardWidget->InitCardStatus(CardStatus);
	CardWidget->CardComponent = CardComponent;

	CardWidget->RemoveFromParent();
	CardHandCanvas->AddChild(CardWidget);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardWidget);
	CanvasPanelSlot->SetSize(FVector2D(200.f, 280.f));
	CanvasPanelSlot->SetPosition(CardSpawnPosition);

	CardWidget->CardDragStartDelegate.AddDynamic(this, &UNonCombatOverlay::DragStarted);
	CardWidget->CardDragEndDelegate.AddDynamic(this, &UNonCombatOverlay::DragEnded);
	CardWidget->AddToViewport();
	return CardWidget;
}

float UNonCombatOverlay::GetCardXPosition(int32 Index)
{
	float XPos = CardHandSizeBox->GetDesiredSize().X / 2 + GetCardIndexPositionFromCenter(Index) * CardDistance - CardSize.X / 2;
	if (HandCard[Index]->GetMouseHovered()) XPos += MouseHoveredHeight * FMath::Sin(FMath::DegreesToRadians(GetCardAngle(Index)));
	return XPos;
}

float UNonCombatOverlay::GetCardYPosition(int32 Index)
{
	float YPos = FMath::Abs(GetCardIndexPositionFromCenter(Index)) * ArcHeight;
	if (HandCard[Index]->GetMouseHovered()) YPos -= MouseHoveredHeight * FMath::Cos(FMath::DegreesToRadians(GetCardAngle(Index)));
	return YPos;
}

float UNonCombatOverlay::GetCardAngle(int32 Index)
{
	return GetCardIndexPositionFromCenter(Index) * CardAngle;
}

void UNonCombatOverlay::DragStarted(UCardWidget* CardDragged)
{
	NowDragCard = CardDragged;
}

void UNonCombatOverlay::DragEnded(UCardWidget* CardDragged, bool bIsUsed)
{
	NowDragCard = nullptr;
	if (bIsUsed)
	{
		HandCard.Remove(CardDragged);
	}
}

// Convert card index 0, 1, 2, 3, 4 -> -2, -1, 0, 1, 2
float UNonCombatOverlay::GetCardIndexPositionFromCenter(int32 Index) const
{
	float NewIndex = (float)Index - (((float)HandCard.Num() - 1.f) / 2.f);
	if (NewIndex < 0)
	{
		NewIndex -= 0.5f;
	}
	NewIndex = FMath::RoundToFloat(NewIndex);
	if (HandCard.Num() % 2 == 0)
	{
		if (NewIndex < 0) NewIndex += 0.5f;
		else
		{
			NewIndex -= 0.5f;
		}
	}
	return NewIndex;
}

void UNonCombatOverlay::TurnChanged(ETurn Turn)
{
	CurrentTurn = Turn;
	bCardHandPositionSet = false;
	ShowTurnText();
}

void UNonCombatOverlay::SetCardHandPosition(float DeltaTime)
{
	FVector2D DestinationPosition;

	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardHandSizeBox);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this) * UWidgetLayoutLibrary::GetViewportScale(this);
	if ((CurrentTurn == ETurn::ET_PlayerTurn || CurrentTurn == ETurn::ET_MoveTurn) && MousePosition.Y > CardComponent->GetPlayCardHeight() && CardComponent->bActivatingCard == false)
	{
		// Card Hand Active
		DestinationPosition = FVector2D(0.f, 0.f);
	}
	else
	{
		// Card Hand Disable
		DestinationPosition = FVector2D(0.f, CanvasPanelSlot->GetSize().Y + 100.f);
	}
	CanvasPanelSlot->SetPosition(FMath::Vector2DInterpTo(CanvasPanelSlot->GetPosition(), DestinationPosition, DeltaTime, CardHandInterpSpeed));
	if (CanvasPanelSlot->GetPosition() == DestinationPosition)
	{
		bCardHandPositionSet = true;
	}
}

void UNonCombatOverlay::ShowTurnText()
{
	if (CurrentTurn == ETurn::ET_MoveTurn)
	{
		TurnText->SetText(FText::FromString("Player Turn"));
	}
	else if (CurrentTurn == ETurn::ET_EnemyTurn)
	{
		TurnText->SetText(FText::FromString("Enemy Turn"));
	}
	else
	{
		return;
	}
	TurnText->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TurnTextTimer, this, &UNonCombatOverlay::HideTurnText, TurnTextDisplayTime, false);
}

void UNonCombatOverlay::HideTurnText()
{
	TurnText->SetVisibility(ESlateVisibility::Hidden);
}

void UNonCombatOverlay::SetCostPathLengthWidgetPosition()
{
	if (PathLengthCostWidget == nullptr || CardComponent == nullptr) return;

	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	if (CurrentTurn != ETurn::ET_MoveTurn || MousePosition.Y * UWidgetLayoutLibrary::GetViewportScale(this) > CardComponent->GetPlayCardHeight())
	{
		PathLengthCostWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PathLengthCostWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (PathLengthCostWidget->GetVisibility() == ESlateVisibility::Hidden) return;

	AZPlayerCharacter* Character = Cast<AZPlayerCharacter>(Cast<APlayerController>(GetOwningPlayer())->GetCharacter());

	if (Character)
	{
		PathLengthCostWidget->SetVisibility(ESlateVisibility::Visible);
		PathLengthCostWidget->SetCost(Character->GetPathCost());
		PathLengthCostWidget->SetPathLength(Character->GetPathLength());
	}

	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PathLengthCostWidget);
	CanvasPanelSlot->SetPosition(MousePosition - FVector2D(0.f, CanvasPanelSlot->GetSize().Y));
}

float UNonCombatOverlay::GetHealthCheckingBarPos()
{
	const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HealthBar);
	return CanvasPanelSlot->GetSize().X * HealthBar->GetPercent() + CanvasPanelSlot->GetPosition().X;
}

float UNonCombatOverlay::GetMentalityCheckingBarPos()
{
	const UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MentalityBar);
	return CanvasPanelSlot->GetSize().X * MentalityBar->GetPercent() + CanvasPanelSlot->GetPosition().X;
}

void UNonCombatOverlay::SetHealthCheckingBarPos()
{
	HealthCheckingBar->SetVisibility(ESlateVisibility::Visible);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(HealthCheckingBar);
	CanvasPanelSlot->SetPosition(FVector2D(GetHealthCheckingBarPos(), 0.f));
}

void UNonCombatOverlay::SetMentalityCheckingBarPos()
{
	MentalityCheckingBar->SetVisibility(ESlateVisibility::Visible);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(MentalityCheckingBar);
	CanvasPanelSlot->SetPosition(FVector2D(GetMentalityCheckingBarPos(), 0.f));
}

void UNonCombatOverlay::OnHealthChanged(float NewValue)
{
	Health = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
	SetHealthCheckingBarPos();
}

void UNonCombatOverlay::OnMaxHealthChanged(float NewValue)
{
	MaxHealth = NewValue;
	if (MaxHealth != 0) HealthBar->SetPercent(Health / MaxHealth);
	SetHealthCheckingBarPos();
}

void UNonCombatOverlay::OnMentalityChanged(float NewValue)
{
	Mentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
	SetMentalityCheckingBarPos();
}

void UNonCombatOverlay::OnMaxMentalityChanged(float NewValue)
{
	MaxMentality = NewValue;
	if (MaxMentality != 0) MentalityBar->SetPercent(Mentality / MaxMentality);
	SetMentalityCheckingBarPos();
}
