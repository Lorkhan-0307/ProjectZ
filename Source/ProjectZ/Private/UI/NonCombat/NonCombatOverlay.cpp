// Fill out your copyright notice in the Description page of Project Settings.


#include "Ui/NonCombat/NonCombatOverlay.h"

#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "UI/Combat/CostPathLengthWidget.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "UI/Card/CardHandWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/CardComponent.h"
#include "Character/ZNonCombatCharacter.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Ui/Card/CardWidget.h"
#include "Player/ZNonCombatPlayerController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Game/ZGameModeBase.h"
#include "UI/Combat/CharacterPortraitWidget.h"
#include "Widgets/SViewport.h"


void UNonCombatOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	HealthCheckingBar->SetVisibility(ESlateVisibility::Visible);
	MentalityCheckingBar->SetVisibility(ESlateVisibility::Visible);
	TurnText->SetVisibility(ESlateVisibility::Hidden);
	ShowCostWidget(false);
	ShowTurnEndButton(false);
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

		UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardHandSizeBox);
		CardComponent->SetPlayCardHeight(UWidgetLayoutLibrary::GetViewportSize(this).Y - CanvasPanelSlot->GetSize().Y * UWidgetLayoutLibrary::GetViewportScale(this));

		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
		const float ViewPortScale = UWidgetLayoutLibrary::GetViewportScale(this);
		CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(LeftHandSizeBox);

		CardComponent->LeftEquipPosMin.X = CanvasPanelSlot->GetPosition().X * ViewPortScale;
		CardComponent->LeftEquipPosMin.Y = ViewportSize.Y + (CanvasPanelSlot->GetPosition().Y - CanvasPanelSlot->GetSize().Y) * ViewPortScale;

		CardComponent->LeftEquipPosMax.X = (CanvasPanelSlot->GetPosition().X + CanvasPanelSlot->GetSize().X) * ViewPortScale;
		CardComponent->LeftEquipPosMax.Y = ViewportSize.Y + CanvasPanelSlot->GetPosition().Y * ViewPortScale;

		CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(RightHandSizeBox);
		CardComponent->RightEquipPosMin.X = ViewportSize.X + (CanvasPanelSlot->GetPosition().X - CanvasPanelSlot->GetSize().X) * ViewPortScale;
		CardComponent->RightEquipPosMin.Y = ViewportSize.Y + (CanvasPanelSlot->GetPosition().Y - CanvasPanelSlot->GetSize().Y) * ViewPortScale;

		CardComponent->RightEquipPosMax.X = ViewportSize.X + CanvasPanelSlot->GetPosition().X * ViewPortScale;
		CardComponent->RightEquipPosMax.Y = ViewportSize.Y + CanvasPanelSlot->GetPosition().Y * ViewPortScale;

		LeftHandCardWidget->CardComponent = CardComponent;
		RightHandCardWidget->CardComponent = CardComponent;

		CardComponent->ActivateCardDelegate.AddDynamic(this, &UNonCombatOverlay::HideSkillCard);

		FVector2D ScreenSize = GameMode->ScreenSize;
		CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(DiscardCardSizeBox);
		CardComponent->DiscardedCardLocation = CanvasPanelSlot->GetPosition() - CanvasPanelSlot->GetSize() / 2.f;
		//CardComponent->DiscardedCardLocation.X = ScreenSize.X + (CanvasPanelSlot->GetPosition().X + CanvasPanelSlot->GetSize().X / 2.f);
		//CardComponent->DiscardedCardLocation.Y = ScreenSize.Y + (CanvasPanelSlot->GetPosition().Y + CanvasPanelSlot->GetSize().Y / 2.f);
		UE_LOG(LogTemp, Warning, TEXT("%f %f"), CardComponent->DiscardedCardLocation.X, CardComponent->DiscardedCardLocation.X);

		CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardGraveyardSizeBox);
		CardComponent->CardGraveyardLocation = CanvasPanelSlot->GetPosition() - CanvasPanelSlot->GetSize() / 2.f;
		//CardComponent->CardGraveyardLocation.X = ScreenSize.X + (CanvasPanelSlot->GetPosition().X + CanvasPanelSlot->GetSize().X / 2.f);
		//CardComponent->CardGraveyardLocation.Y = ScreenSize.Y + (CanvasPanelSlot->GetPosition().Y + CanvasPanelSlot->GetSize().Y / 2.f);
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
		OverlayWidgetController->OnCostChanged.AddDynamic(this, &UNonCombatOverlay::OnCostChanged);
		OverlayWidgetController->OnMaxCostChanged.AddDynamic(this, &UNonCombatOverlay::OnMaxCostChanged);
	}

	GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());

	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CardHandSizeBox);
	CardSpawnPosition = FVector2D(CanvasPanelSlot->GetSize().X / 2.f, 0.f);

	GameMode->TurnChangedDelegate.AddDynamic(this, &UNonCombatOverlay::TurnChanged);

	TurnEndButton->OnClicked.AddDynamic(this, &UNonCombatOverlay::TurnEnd);

	CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CharacterPortraitSizeBox);
	CharacterPortraitCenter = CanvasPanelSlot->GetSize().X / 2.f;
	GameMode->CombatActorChangedDelegate.AddDynamic(this, &UNonCombatOverlay::DiscardCharacterPortrait);
	CharacterPortraitSizeBox->SetVisibility(ESlateVisibility::Hidden);
}

void UNonCombatOverlay::UpdateLeftHandCard(FCard LeftCard)
{
	if (LeftCard.IsValid)
	{
		LeftHandCardWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		LeftHandCardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	LeftHandCardWidget->InitCardStatus(LeftCard, false);
}

void UNonCombatOverlay::UpdateRightHandCard(FCard RightCard)
{
	if (RightCard.IsValid)
	{
		RightHandCardWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		RightHandCardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	RightHandCardWidget->InitCardStatus(RightCard, false);
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
	CardWidget->CardComponent = CardComponent;
	CardWidget->InitCardStatus(CardStatus);

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
	float XPos = CardHandSizeBox->GetDesiredSize().X / 2 + GetIndexPositionFromCenter(Index, HandCard.Num()) * CardDistance - CardSize.X / 2;
	if (HandCard[Index]->GetMouseHovered()) XPos += MouseHoveredHeight * FMath::Sin(FMath::DegreesToRadians(GetCardAngle(Index)));
	return XPos;
}

float UNonCombatOverlay::GetCardYPosition(int32 Index)
{
	float YPos = FMath::Abs(GetIndexPositionFromCenter(Index, HandCard.Num())) * ArcHeight;
	if (HandCard[Index]->GetMouseHovered()) YPos -= MouseHoveredHeight * FMath::Cos(FMath::DegreesToRadians(GetCardAngle(Index)));
	return YPos;
}

float UNonCombatOverlay::GetCardAngle(int32 Index)
{
	return GetIndexPositionFromCenter(Index, HandCard.Num()) * CardAngle;
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
		//CardDragged->RemoveFromParent();
		//OverlayCanvasPanel->AddChild(CardDragged);
	}
}

// Convert card index 0, 1, 2, 3, 4 -> -2, -1, 0, 1, 2
float UNonCombatOverlay::GetIndexPositionFromCenter(int32 Index, int32 Size) const
{
	float NewIndex = (float)Index - (((float)Size - 1.f) / 2.f);
	if (NewIndex < 0)
	{
		NewIndex -= 0.5f;
	}
	NewIndex = FMath::RoundToFloat(NewIndex);
	if (Size % 2 == 0)
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
	ShowCostWidget(Turn == ETurn::ET_MoveTurn || Turn == ETurn::ET_PlayerTurn);
	ShowTurnEndButton(Turn == ETurn::ET_MoveTurn || Turn == ETurn::ET_PlayerTurn);

	if (Turn == ETurn::ET_NonCombat)
	{
		HideCharacterPortrait();
	}
	else if (CharacterPortraitSizeBox->GetVisibility() == ESlateVisibility::Hidden)
	{
		ShowCharacterPortrait();
	}

	if (CurrentTurn == ETurn::ET_EnemyTurn && (GameMode->GetLastTurn() == ETurn::ET_MoveTurn || GameMode->GetLastTurn() == ETurn::ET_PlayerTurn))
	{
		for (UCardWidget* CardWidget : HandCard)
		{
			CardWidget->DestroyWidget();
		}
		HandCard.Empty();
	}
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
	else if (CurrentTurn == ETurn::ET_EnemyTurn && (GameMode->GetLastTurn() == ETurn::ET_MoveTurn || GameMode->GetLastTurn() == ETurn::ET_PlayerTurn))
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

void UNonCombatOverlay::ShowCostWidget(bool bShow)
{
	const ESlateVisibility CostWidgetVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	CostOverlay->SetVisibility(CostWidgetVisibility);
	/*
	TArray<UWidget*> ChildWidget = CostOverlay->GetAllChildren();
	for (UWidget* Child : ChildWidget)
	{
		Child->SetVisibility(CostWidgetVisibility);
	}
	*/
}

void UNonCombatOverlay::ShowTurnEndButton(bool bShow)
{
	const ESlateVisibility TurnEndWidgetVisibility = bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	TurnEndButton->SetVisibility(TurnEndWidgetVisibility);
}

void UNonCombatOverlay::ShowSkillCard()
{
	if (CardComponent == nullptr) return;
	if (CardComponent->bActivatingCard == false) return;
	ShowSkillCardWidget->InitCardStatus(CardComponent->ActivatingCard, false);
	ShowSkillCardOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UNonCombatOverlay::HideSkillCard()
{
	ShowSkillCardOverlay->SetVisibility(ESlateVisibility::Hidden);
}

void UNonCombatOverlay::ShowCharacterPortrait()
{
	CharacterPortraitSizeBox->SetVisibility(ESlateVisibility::Visible);

	PortraitWidgets.Empty();
	for (AActor* CombatActor : GameMode->CombatActor)
	{
		UCharacterPortraitWidget* CharacterPortraitWidget = CreateWidget<UCharacterPortraitWidget>(GetOwningPlayer(), CharacterPortraitWidgetClass);
		CharacterPortraitWidget->RemoveFromParent();
		CharacterPortraitCanvasPanel->AddChild(CharacterPortraitWidget);
		CharacterPortraitWidget->InitPortrait(CombatActor);

		UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CharacterPortraitWidget);
		CanvasPanelSlot->SetSize(FVector2D(100.f, 150.f));
		CanvasPanelSlot->SetPosition(FVector2D(CharacterPortraitCenter, 0.f));

		PortraitWidgets.Add(CharacterPortraitWidget);
		CharacterPortraitWidget->AddToViewport();
		UpdateCharacterPortrait();
	}
}

void UNonCombatOverlay::HideCharacterPortrait()
{
	CharacterPortraitSizeBox->SetVisibility(ESlateVisibility::Hidden);
	for (UCharacterPortraitWidget* CharacterPortraitWidget : PortraitWidgets)
	{
		CharacterPortraitWidget->DestroyWidget();
	}
	PortraitWidgets.Empty();
}

void UNonCombatOverlay::UpdateCharacterPortrait()
{
	for (int i = 0; i < PortraitWidgets.Num(); i++)
	{
		UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PortraitWidgets[i]);
		PortraitWidgets[i]->DestinationLocation = FVector2D(GetIndexPositionFromCenter(i, PortraitWidgets.Num()) * CanvasPanelSlot->GetSize().X + CharacterPortraitCenter, 0.f);
		UE_LOG(LogTemp, Warning, TEXT("%d %f %d %f"), i, GetIndexPositionFromCenter(i, PortraitWidgets.Num()), PortraitWidgets.Num(), PortraitWidgets[i]->DestinationLocation.X);
	}
}

void UNonCombatOverlay::DiscardCharacterPortrait(AActor* Actor)
{
	//UCharacterPortraitWidget* DiscardWidget;
	for (int i = 0; i < PortraitWidgets.Num(); i++)
	{
		if (PortraitWidgets[i]->CharacterActor == Actor)
		{
			//DiscardWidget = PortraitWidget;
			PortraitWidgets[i]->DestroyWidget();
			PortraitWidgets.RemoveAt(i);
			i--;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetActorNameOrLabel())
	UpdateCharacterPortrait();
}


void UNonCombatOverlay::TurnEnd()
{
	Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->NextTurn();
	//Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode())->SetTurn(ETurn::ET_EnemyTurn);
	//CurrentTurn = ETurn::ET_EnemyTurn;
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

void UNonCombatOverlay::OnCostChanged(float NewValue)
{
	Cost = NewValue;
	CostText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), (int)Cost, (int)MaxCost)));
}

void UNonCombatOverlay::OnMaxCostChanged(float NewValue)
{
	MaxCost = NewValue;
	CostText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), (int)Cost, (int)MaxCost)));
}
