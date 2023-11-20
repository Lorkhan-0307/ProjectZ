// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CardComponent.h"

// Sets default values for this component's properties
UCardComponent::UCardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardComponent::BeginPlay()
{
	Super::BeginPlay();

	AddCard(FName("Axe"));
	AddCard(FName("KitchenKnife"));
	AddCard(FName("Sword"));
	AddCard(FName("Axe"));
	AddCard(FName("KitchenKnife"));
	AddCard(FName("Axe"));
	AddCard(FName("Sword"));
	AddCard(FName("KitchenKnife"));
	// ...

	for (int i = 0; i < FirstCardCount; i++)
	{
		DrawCard();
	}
}

void UCardComponent::AddCard(FName NewCardName)
{
	const FCard NewCard = *CardDataTable->FindRow<FCard>(NewCardName, FString(""));
	CardDeck.Push(NewCard);
	DeckSize++;
}


// Called every frame
void UCardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FCard UCardComponent::DrawCard()
{
	FCard TopCard = CardDeck.Top();
	CardDeck.Pop();
	HandSize++;
	DeckSize--;
	UpdateHandCardDelegate.Broadcast(HandSize);
	DrawAndAddCardDelegate.Broadcast(TopCard);
	return TopCard;
}

void UCardComponent::ShuffleDeck()
{
	CardDeck.Sort([this](const FCard a, const FCard b) { return FMath::FRand() < 0.5f; });
}
