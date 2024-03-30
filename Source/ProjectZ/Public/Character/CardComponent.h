// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Card.h"
#include "GameplayEffectTypes.h"
#include "Game/ZGameModeBase.h"
#include "CardComponent.generated.h"

class UCharacterClassInfo;
class AZCharacterBase;
class AZNonCombatCharacter;
class AZGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCardDelegate, FCard, NewCard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActivateCardDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCancelActivateCardDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTZ_API UCardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardComponent();
	void FirstDrawCard();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void DrawCard();

	// Draw Card from deck, Add Card to hand
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateCardDelegate DrawAndAddCardDelegate;

	// When change left hand card
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateCardDelegate UpdateLeftHandCardDelegate;

	// When change right hand card
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateCardDelegate UpdateRightHandCardDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FUpdateCardDelegate ShowSkillCardDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FActivateCardDelegate ActivateCardDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FCancelActivateCardDelegate CancelActivateCardDelegate;
	

	void InitializeCardComponent(AZCharacterBase* Character);

	FCard ConvertCardNameToFCard(FName CardName);
	void InitializeCardInventory(UCharacterClassInfo* CharacterClassInfo);

	// Active Card, Apply Effect to target
	void ActiveCard(FCard Card, bool bIsLeftHand = true);
	void CancelActivateCard();

	void SetLeftHandCard(FCard Card, bool bIsValid = true);
	void SetRightHandCard(FCard Card, bool bIsValid = true);

	UPROPERTY(BlueprintReadOnly)
	FCard ActivatingCard;
	
	bool bActivatingCard = false;

	FVector2D LeftEquipPosMin;
	FVector2D LeftEquipPosMax;
	FVector2D RightEquipPosMin;
	FVector2D RightEquipPosMax;

	UPROPERTY(EditAnywhere, Category = Card)
	float CardTrashSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D DiscardedCardLocation;

	UPROPERTY(EditAnywhere, Category = Card)
	FVector2D CardGraveyardLocation;

	void UseCard(FCard Card);

	//Getter, Setter
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetDeckSize() const { return CardDeck.Num(); }

	FORCEINLINE int32 GetHandSize() const { return CardHand.Num(); }

	FORCEINLINE FCard GetLeftHandCard() const { return LeftHandCard; }
	FORCEINLINE FCard GetRightHandCard() const { return RightHandCard; }
	FORCEINLINE float GetPlayCardHeight() const { return PlayCardHeight; }
	FORCEINLINE void SetPlayCardHeight(float Height) { PlayCardHeight = Height; }

protected:
	virtual void BeginPlay() override;

	// Change function name AddCard -> AddCardToDeck
	void AddCardToInventory(FName NewCardName);
	void AddCardToDeck(FName NewCard);
	void DeleteCard(FName DeleteCard);

private:
	// Card data
	UPROPERTY(EditAnywhere, Category = Card)
	UDataTable* CardDataTable;

	UPROPERTY(EditAnywhere, Category = Card)
	TArray<TSubclassOf<UGameplayAbility>> CardAbilities;

	// Card Inventory : For NonCombat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardInventory;

	// Card Deck : For Combat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardDeck;
	// Card Hand : For Combat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardHand;

	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> DiscardCard;

	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardGraveyard;

	// Number of cards in hand when start combat
	UPROPERTY(EditAnywhere, Category = Card)
	int32 FirstCardCount;

	float PlayCardHeight;

	FCard LeftHandCard;

	FCard RightHandCard;

	int32 DeckSize;
	int32 HandSize = 0;

	UPROPERTY()
	AZCharacterBase* ZCharacter;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	void ShuffleDeck();
	void MakeCardDeck();

	void ApplyEffectToTarget(TSubclassOf<UGameplayEffect> Effect, int32 CardLevel, AZCharacterBase* TargetCharacter);

	UFUNCTION()
	void TurnChanged(ETurn Turn);

	ETurn CurrentTurn;

	UPROPERTY()
	AZGameModeBase* GameMode;
};
