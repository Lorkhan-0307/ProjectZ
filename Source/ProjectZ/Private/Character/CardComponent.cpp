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

void UCardComponent::GetCardInventory(TArray<FCard>& Inventory)
{
	Inventory = CardInventory;
}

FCard UCardComponent::GetCardCraftResult(FCard FirstCard, FCard SecondCard, bool& bIsValid)
{
	// Repair
	if (FirstCard == FText::FromString("DuctTape") && (SecondCard.CardType == ECardType::ECT_OneHandWeapon || SecondCard.CardType == ECardType::ECT_TwoHandWeapon))
	{
		SecondCard.CardDef = SecondCard.CardMaxDef;
		bIsValid = true;
		return SecondCard;
	}
	if (SecondCard == FText::FromString("DuctTape") && (FirstCard.CardType == ECardType::ECT_OneHandWeapon || FirstCard.CardType == ECardType::ECT_TwoHandWeapon))
	{
		FirstCard.CardDef = FirstCard.CardMaxDef;
		bIsValid = true;
		return FirstCard;
	}

	for (FCardCombination Comb : CardCombination)
	{
		if ((FirstCard == Comb.FirstCard && SecondCard == Comb.SecondCard) || (FirstCard == Comb.SecondCard && SecondCard == Comb.FirstCard))
		{
			FName ResultCardName = FName(*Comb.ResultCard.ToString());
			bIsValid = true;
			return ConvertCardNameToFCard(ResultCardName);
		}
	}
	bIsValid = false;
	return FCard();
}

// Called when the game starts
void UCardComponent::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AZGameModeBase>(GetWorld()->GetAuthGameMode());
	GameMode->TurnChangedDelegate.AddDynamic(this, &UCardComponent::TurnChanged);
}

void UCardComponent::AddCardToInventory(FCard NewCardName)
{
	const FCard NewCard = NewCardName;
	CardInventory.Push(NewCard);
	//UpdateCardInventoryDelegate.Broadcast();
}

void UCardComponent::RemoveCardFromInventory(FCard& DeleteCard)
{
	CardInventory.RemoveSingle(DeleteCard);
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

	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	for (TTuple<FName, unsigned char*> Row : CardDataTable->GetRowMap())
	{
		const FCard Card = *CardDataTable->FindRow<FCard>(Row.Key, FString(""));

		if (Abilities.Contains(Card.CardAbility)) continue;

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Card.CardAbility, 1);
		if (const UZGameplayAbility* ZAbility = Cast<UZGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(ZAbility->CardSkillTag);
			ZCharacter->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		}
		Abilities.Add(Card.CardAbility);
	}

	UCharacterClassInfo* CharacterClassInfo = UZAbilitySystemLibrary::GetCharacterClassInfo(Character);
	InitializeCardInventory(CharacterClassInfo);

	TArray<FCardCombination*> Arr;
	CardCombinationDataTable->GetAllRows<FCardCombination>(TEXT("GetAllRows"), Arr);

	for (FCardCombination* Comb : Arr)
	{
		CardCombination.Add(*Comb);
	}

	// For Test
	AddCardToInventory(ConvertCardNameToFCard(FName("KitchenKnife")));
	AddCardToInventory(ConvertCardNameToFCard(FName("Stab")));
	AddCardToInventory(ConvertCardNameToFCard(FName("Wound")));
	AddCardToInventory(ConvertCardNameToFCard(FName("Salt")));
	AddCardToInventory(ConvertCardNameToFCard(FName("Hack")));
	AddCardToInventory(ConvertCardNameToFCard(FName("DuctTape")));
	AddCardToInventory(ConvertCardNameToFCard(FName("DuctTape")));
	AddCardToInventory(ConvertCardNameToFCard(FName("DuctTape")));

	UpdateCardInventoryDelegate.Broadcast();
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

void UCardComponent::SortInventory()
{
	CardInventory.Sort([](const FCard& a, const FCard& b)
	{
		if (a.CardType == b.CardType) return a.CardName.ToString() < b.CardName.ToString();
		return a.CardType < b.CardType;
	});
}

// Shuffle Deck
void UCardComponent::ShuffleDeck()
{
	CardDeck.Sort([this](const FCard a, const FCard b) { return FMath::FRand() < 0.5f; });
}

// Begin combat, make deck at inventory without passive cards
void UCardComponent::MakeCardDeck()
{
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

	bLeftAttack = false;
	bRightAttack = false;
}


// Active Card and apply effect
void UCardComponent::ActiveCard(FCard Card, bool bIsLeftHand, bool bActiveEquipCard)
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
		if (ZCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer))
		{
			UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		}
		else
		{
			UZAbilitySystemLibrary::PayCost(ZCharacter, 1);
		}
		UseCard(Card);
		ActivateCardDelegate.Broadcast();
		break;

	case ECardType::ECT_OneHandWeapon:
		if (bActiveEquipCard)
		{
			if (bIsLeftHand && bLeftAttack)
			{
				if (AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(ZCharacter)) PlayerCharacter->HideSkillRange();
				break;
			}
			if (!bIsLeftHand && bRightAttack)
			{
				if (AZPlayerCharacter* PlayerCharacter = Cast<AZPlayerCharacter>(ZCharacter)) PlayerCharacter->HideSkillRange();
				break;
			}
			BasicAttack(Card);
			if (bIsLeftHand) bLeftAttack = true;
			else bRightAttack = true;
			break;
		}
		if (bIsLeftHand)
		{
			SetLeftHandCard(Card);
			if (RightHandCard.CardType == ECardType::ECT_TwoHandWeapon)
			{
				RightHandCard.IsValid = false;
				SetRightHandCard(RightHandCard);
			}
		}
		else
		{
			SetRightHandCard(Card);
			if (LeftHandCard.CardType == ECardType::ECT_TwoHandWeapon)
			{
				LeftHandCard.IsValid = false;
				SetLeftHandCard(LeftHandCard);
			}
		}
		ZCharacter->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_TwoHand);
		ZCharacter->GetAbilitySystemComponent()->AddLooseGameplayTag(FZGameplayTag::Get().Card_Weapon_OneHand);
		UZAbilitySystemLibrary::PayCost(ZCharacter, Card.CardCost);
		ActivateCardDelegate.Broadcast();
		break;

	case ECardType::ECT_TwoHandWeapon:
		if (bActiveEquipCard)
		{
			BasicAttack(Card);
			break;
		}
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

void UCardComponent::BasicAttack(FCard Card)
{
	ActivatingCard = Card;
	bActivatingCard = true;
}

void UCardComponent::SetLeftHandCard(FCard Card, bool bIsValid)
{
	if (bIsValid == false)
	{
		Card.IsValid = false;
	}

	LeftHandCard = Card;
	UZAttributeSet* AS = Cast<UZAttributeSet>(ZCharacter->GetAttributeSet());
	AS->SetWeaponAtk(FMath::Max(LeftHandCard.IsValid ? LeftHandCard.CardAtk : 0, RightHandCard.IsValid ? RightHandCard.CardAtk : 0));
	UE_LOG(LogTemp, Warning, TEXT("Weapon ATK : %f"), AS->GetWeaponAtk());
	UpdateLeftHandCardDelegate.Broadcast(Card);
}

void UCardComponent::SetRightHandCard(FCard Card, bool bIsValid)
{
	if (bIsValid == false)
	{
		Card.IsValid = false;
	}

	RightHandCard = Card;
	UZAttributeSet* AS = Cast<UZAttributeSet>(ZCharacter->GetAttributeSet());
	AS->SetWeaponAtk(FMath::Max(LeftHandCard.IsValid ? LeftHandCard.CardAtk : 0, RightHandCard.IsValid ? RightHandCard.CardAtk : 0));
	UpdateRightHandCardDelegate.Broadcast(Card);
}

void UCardComponent::ReduceWeaponDurability(FGameplayTag WeaponTag)
{
	FZGameplayTag GameplayTag = FZGameplayTag::Get();
	if (WeaponTag.MatchesTagExact(GameplayTag.Card_Weapon_OneHand))
	{
		if (LeftHandCard.CardAtk > RightHandCard.CardAtk)
		{
			LeftHandCard.CardDef--;
		}
		else
		{
			RightHandCard.CardDef--;
		}
	}
	else
	{
		LeftHandCard.CardDef--;
		RightHandCard.CardDef--;
	}

	if (LeftHandCard.CardDef == 0 && LeftHandCard.IsValid)
	{
		LeftHandCard.IsValid = false;
		SetLeftHandCard(LeftHandCard);
	}
	if (RightHandCard.CardDef == 0 && RightHandCard.IsValid)
	{
		RightHandCard.IsValid = false;
		SetRightHandCard(RightHandCard);
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
