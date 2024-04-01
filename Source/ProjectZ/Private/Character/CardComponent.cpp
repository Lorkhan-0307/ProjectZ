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
#include "ZGameplayTag.h"
#include "AbilitySystem/ZAbilitySystemComponent.h"
#include "AbilitySystem/ZAbilitySystemLibrary.h"
#include "AbilitySystem/ZAttributeSet.h"
#include "AbilitySystem/Ability/ZGameplayAbility.h"
#include "Game/ZGameModeBase.h"


// Sets default values for this component's properties
UCardComponent::UCardComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UCardComponent::UseCard(FCard Card)
{
	if (Card.CardType == ECardType::ECT_Skill || Card.CardType == ECardType::ECT_Buff)
	{
		DiscardCard.Add(Card);
	}
	CardHand.RemoveSingle(Card);
}

// Called when the game starts
void UCardComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->TurnChangedDelegate.AddDynamic(this, &UCardComponent::TurnChanged);
}

void UCardComponent::AddCardToInventory(FName NewCardName)
{
	const FCard NewCard = ConvertCardNameToFCard(NewCardName);
	CardInventory.Push(NewCard);
}

// Add Card to Deck
void UCardComponent::AddCardToDeck(FName NewCardName)
{
	const FCard NewCard = ConvertCardNameToFCard(NewCardName);
	CardDeck.Push(NewCard);
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

void UCardComponent::InitializeCardComponent(AZCharacterBase* Character)
{
	ZCharacter = Character;

	for (TTuple<FName, unsigned char*> Row : CardDataTable->GetRowMap())
	{
		const FCard Card = *CardDataTable->FindRow<FCard>(Row.Key, FString(""));
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Card.CardAbility, 1);
		if (const UZGameplayAbility* ZAbility = Cast<UZGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ZAbility->CardSkillTag);
			ZCharacter->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		}
	}


/*
	for (const TSubclassOf<UGameplayAbility> Ability : CardAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		if (const UZGameplayAbility* ZAbility = Cast<UZGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ZAbility->CardSkillTag);
			ZCharacter->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		}
	}
	*/
	
	// For Test
	//if (ZCharacter && ZCharacter->GetPlayerState()) Cast<AZPlayerState>(ZCharacter->GetPlayerState())->SetCharacterName(FName("JohnDoe"));
	UCharacterClassInfo* CharacterClassInfo = UZAbilitySystemLibrary::GetCharacterClassInfo(Character);
	InitializeCardInventory(CharacterClassInfo);

	// For Test
	/*
	SetLeftHandCard(ConvertCardNameToFCard(FName("Axe")));
	UpdateLeftHandCardDelegate.Broadcast(GetLeftHandCard());
	SetRightHandCard(ConvertCardNameToFCard(FName("Sword")));
	UpdateRightHandCardDelegate.Broadcast(GetRightHandCard());
	*/
}

// Get name and return FCard
FCard UCardComponent::ConvertCardNameToFCard(FName CardName)
{
	const FCard Card = *CardDataTable->FindRow<FCard>(CardName, FString(""));
	return Card;
}

// When game starts, initialize card inventory by character data table
void UCardComponent::InitializeCardInventory(UCharacterClassInfo* CharacterClassInfo)
{
	TArray<FName> Cards = CharacterClassInfo->GetClassDefaultInfo(ZCharacter->GetCharacterClass()).DefaultCards;
	for (const FName& CardName : Cards)
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
	// For Test
	AddCardToInventory(FName("KitchenKnife"));
	AddCardToInventory(FName("Sword"));
	AddCardToInventory(FName("Axe"));
	AddCardToInventory(FName("HealthPotion"));
	AddCardToInventory(FName("HealthPotion"));
	AddCardToInventory(FName("Smash"));
	AddCardToInventory(FName("Smash"));
	AddCardToInventory(FName("Blocking"));
	AddCardToInventory(FName("Blocking"));
	AddCardToInventory(FName("Frenzy"));
	AddCardToInventory(FName("Frenzy"));
	AddCardToInventory(FName("Gather"));
	AddCardToInventory(FName("Gather"));
	// ...

	CardDeck.Empty();
	for (const FCard& Card : CardInventory)
	{
		if (Card.CardType != ECardType::ECT_Passive)
		{
			CardDeck.Add(Card);
		}
	}


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

void UCardComponent::TurnChanged(ETurn Turn)
{
	CurrentTurn = Turn;
	if (GameMode->GetLastTurn() == ETurn::ET_NonCombat && Turn != ETurn::ET_NonCombat)
	{
		MakeCardDeck();
	}

	if (CurrentTurn == ETurn::ET_MoveTurn)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawCard();
		}
	}
	else if (CurrentTurn == ETurn::ET_EnemyTurn && (GameMode->GetLastTurn() == ETurn::ET_PlayerTurn || GameMode->GetLastTurn() == ETurn::ET_MoveTurn))
	{
		for (FCard Card : CardHand)
		{
			DiscardCard.Add(Card);
		}
		CardHand.Empty();
		if (CardDeck.Num() < 5)
		{
			for (FCard Card : DiscardCard)
			{
				CardDeck.Add(Card);
			}
			DiscardCard.Empty();
			ShuffleDeck();
		}
	}
}


// Active Card and apply effect
void UCardComponent::ActiveCard(FCard Card, bool bIsLeftHand)
{
	FGameplayTagContainer TagContainer;
	switch (Card.CardType)
	{
	case ECardType::ECT_UsablePassive:
		for (const auto& InstantEffect : Card.InstantGameplayEffects)
		{
			ApplyEffectToTarget(InstantEffect, Card.CardLevel, ZCharacter);
		}
		for (const auto& DurationEffect : Card.DurationGameplayEffects)
		{
			ApplyEffectToTarget(DurationEffect, Card.CardLevel, ZCharacter);
		}
		for (const auto& InfiniteEffect : Card.InfiniteGameplayEffects)
		{
			ApplyEffectToTarget(InfiniteEffect, Card.CardLevel, ZCharacter);
		}
		UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		UseCard(Card);
		ActivateCardDelegate.Broadcast();
		break;

	case ECardType::ECT_Skill:
		ActivatingCard = Card;
		bActivatingCard = true;
		ShowSkillCardDelegate.Broadcast(Card);
		break;

	case ECardType::ECT_Buff:
		ActivatingCard = Card;
		TagContainer.AddTag(Card.CardTag);
		ZCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);
		UseCard(Card);
		ActivateCardDelegate.Broadcast();
		UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		break;

	case ECardType::ECT_OneHandWeapon:
		if (bIsLeftHand)
		{
			SetLeftHandCard(Card);
		}
		else
		{
			SetRightHandCard(Card);
		}
		ZCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_TwoHand);
		ZCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_OneHand);
		UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		ActivateCardDelegate.Broadcast();
		break;

	case ECardType::ECT_TwoHandWeapon:
		SetLeftHandCard(Card);
		SetRightHandCard(Card);
		ZCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_OneHand);
		ZCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_TwoHand);
		UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		ActivateCardDelegate.Broadcast();
		break;

	case ECardType::ECT_Passive:
		break;

	case ECardType::ECT_CantEquip:
		break;
	}
}

void UCardComponent::CancelActivateCard()
{
	ActivatingCard.IsValid = false;
	bActivatingCard = false;
	CancelActivateCardDelegate.Broadcast();
}

void UCardComponent::SetLeftHandCard(FCard Card, bool bIsValid)
{
	if (bIsValid == false)
	{
		Card.IsValid = false;
	}

	LeftHandCard = Card;
	UpdateLeftHandCardDelegate.Broadcast(Card);
}

void UCardComponent::SetRightHandCard(FCard Card, bool bIsValid)
{
	if (bIsValid == false)
	{
		Card.IsValid = false;
	}

	RightHandCard = Card;
	UpdateRightHandCardDelegate.Broadcast(Card);
}


// When enter combat, Draw Cards
void UCardComponent::FirstDrawCard()
{
	for (int i = 0; i < FirstCardCount; i++)
	{
		DrawCard();
	}
}
