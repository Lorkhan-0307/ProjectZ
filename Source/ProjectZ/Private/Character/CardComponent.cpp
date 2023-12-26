// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CardComponent.h"
// TO DO : Add ZCombatPlayerController
#include "Player/ZNonCombatPlayerController.h"
#include "Character/ZNonCombatCharacter.h"
#include "UI/HUD/ZHUDBase.h"
#include "GameplayEffect.h"
#include "Data/CharacterInfo.h"
#include "GameFramework/PlayerState.h"
#include "Player/ZPlayerState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


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
}

// Add Card to Deck
void UCardComponent::AddCardToDeck(FName NewCardName)
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

void UCardComponent::InitializeNonCombat(AZCharacterBase* Character)
{
	ZCharacter = Character;
	// For Test
	if (ZCharacter && ZCharacter->GetPlayerState()) Cast<AZPlayerState>(ZCharacter->GetPlayerState())->SetCharacterName(FName("JohnDoe"));
	InitializeCardInventory(Cast<AZPlayerState>(ZCharacter->GetPlayerState())->GetCharacterName());

	// For Test
	SetLeftHandCard(ConvertCardNameToFCard(FName("Axe")));
	UpdateLeftHandCardDelegate.Broadcast(GetLeftHandCard());
	SetRightHandCard(ConvertCardNameToFCard(FName("Sword")));
	UpdateRightHandCardDelegate.Broadcast(GetRightHandCard());
}

void UCardComponent::InitializeCombat(AZCharacterBase* Character)
{
	ZCharacter = Character;
	// For Test
	//if (ZCharacter && ZCharacter->GetPlayerState()) Cast<AZPlayerState>(ZCharacter->GetPlayerState())->SetCharacterName(FName("JohnDoe"));
	//InitializeCardInventory(Cast<AZPlayerState>(ZCharacter->GetPlayerState())->GetCharacterName());

	MakeCardDeck();

	FirstDrawCard();

	// For Test
	SetLeftHandCard(ConvertCardNameToFCard(FName("Axe")));
	UpdateLeftHandCardDelegate.Broadcast(GetLeftHandCard());
	SetRightHandCard(ConvertCardNameToFCard(FName("Sword")));
	UpdateRightHandCardDelegate.Broadcast(GetRightHandCard());
}

// Get name and return FCard
FCard UCardComponent::ConvertCardNameToFCard(FName CardName)
{
	const FCard Card = *CardDataTable->FindRow<FCard>(CardName, FString(""));
	return Card;
}

// When game starts, initialize card inventory by character data table
void UCardComponent::InitializeCardInventory(FName CharacterName)
{
	const FCharacterInfo& CharacterInfo = *CharacterDataTable->FindRow<FCharacterInfo>(CharacterName, FString(""));
	const TArray<FName>& DefaultCards = CharacterInfo.DefaultCards;
	for (const FName& CardName : DefaultCards)
	{
		CardInventory.Add(ConvertCardNameToFCard(CardName));
	}
}

// Shuffle Deck
void UCardComponent::ShuffleDeck()
{
	CardDeck.Sort([this](const FCard a, const FCard b) { return FMath::FRand() < 0.5f; });
}

// Begin combat, make deck at inventory without passive cards
void UCardComponent::MakeCardDeck()
{
	CardDeck.Empty();
	for (const FCard& Card : CardInventory)
	{
		if (Card.CardType != ECardType::ECT_Passive)
		{
			CardDeck.Add(Card);
		}
	}

	// For Test
	AddCardToDeck(FName("HealthPotion"));
	AddCardToDeck(FName("KitchenKnife"));
	AddCardToDeck(FName("Sword"));
	AddCardToDeck(FName("Axe"));
	// ...

	ShuffleDeck();
}

// Apply Effect to Target
void UCardComponent::ApplyEffectToTarget(TSubclassOf<UGameplayEffect> Effect, int32 CardLevel, AZCharacterBase* TargetCharacter)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetCharacter);
	if (TargetASC == nullptr) return;

	check(Effect);

	// Make EffectHandle
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(Effect, CardLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	/* Not Implement Yet
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
	*/
}

// Active Card and apply effect
void UCardComponent::ActiveCard(FCard Card)
{
	/* TO DO : Create Combat System, find target of this card effect
	const AZCharacterBase* TargetCharacter = Cast<AZCharacterBase>(CombatSystem->GetTarget(~~~));
	*/

	// For Test
	if (Card.CardType == ECardType::ECT_UsablePassive)
	{
		AZCharacterBase* TargetCharacter = Cast<AZCharacterBase>(ZCharacter);
		if (TargetCharacter)
		{
			for (const auto& InstantEffect : Card.InstantGameplayEffects)
			{
				ApplyEffectToTarget(InstantEffect, Card.CardLevel, TargetCharacter);
			}
			for (const auto& DurationEffect : Card.DurationGameplayEffects)
			{
				ApplyEffectToTarget(DurationEffect, Card.CardLevel, TargetCharacter);
			}
			for (const auto& InfiniteEffect : Card.InfiniteGameplayEffects)
			{
				ApplyEffectToTarget(InfiniteEffect, Card.CardLevel, TargetCharacter);
			}
		}
	}
}


// When enter combat, Draw Cards
void UCardComponent::FirstDrawCard()
{
	for (int i = 0; i < FirstCardCount; i++)
	{
		DrawCard();
	}
}
