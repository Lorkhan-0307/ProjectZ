// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CardComponent.h"
// TO DO : Change ZCombatPlayerController
#include "Player/ZNonCombatPlayerController.h"
#include "Character/ZCharacter.h"
#include "UI/HUD/ZHUDBase.h"

// Sets default values for this component's properties
UCardComponent::UCardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

// Called when the game starts
void UCardComponent::BeginPlay()
{
	Super::BeginPlay();

	// For Test
	AddCard(FName("Axe"));
	AddCard(FName("KitchenKnife"));
	AddCard(FName("Sword"));
	AddCard(FName("Axe"));
	AddCard(FName("KitchenKnife"));
	AddCard(FName("Axe"));
	AddCard(FName("Sword"));
	AddCard(FName("KitchenKnife"));
	// ...
	
	FirstDrawCard();

	SetLeftHandCard(ConvertCardNameToFCard(FName("Axe")));
	UpdateLeftHandCardDelegate.Broadcast(GetLeftHandCard());
	SetRightHandCard(ConvertCardNameToFCard(FName("Sword")));
	UpdateRightHandCardDelegate.Broadcast(GetRightHandCard());
}

// Add Card to Deck
void UCardComponent::AddCard(FName NewCardName)
{
	CardDeck.Push(ConvertCardNameToFCard(NewCardName));
	DeckSize++;
}


// Called every frame
void UCardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Draw Card for deck and add card to hand
void UCardComponent::DrawCard()
{
	if (CardDeck.Num() == 0) return;
	const FCard TopCard = CardDeck.Top();
	CardDeck.Pop();
	HandSize++;
	DeckSize--;
	CardHand.Add(TopCard);
	DrawAndAddCardDelegate.Broadcast(TopCard);
}

// Get name and return FCard
FCard UCardComponent::ConvertCardNameToFCard(FName CardName)
{
	const FCard Card = *CardDataTable->FindRow<FCard>(CardName, FString(""));
	return Card;
}

// Shuffle Deck
void UCardComponent::ShuffleDeck()
{
	CardDeck.Sort([this](const FCard a, const FCard b) { return FMath::FRand() < 0.5f; });
}

// When enter combat, Draw Cards
void UCardComponent::FirstDrawCard()
{
	for (int i = 0; i < FirstCardCount; i++)
	{
		DrawCard();
	}
}
