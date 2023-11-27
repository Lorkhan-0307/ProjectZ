// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Card.h"
#include "CardComponent.generated.h"

class AZCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawAndAddCardDelegate, FCard, NewCard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHandCardDelegate, int32, NewHandCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateLeftHandCardDelegate, FCard, LeftCard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateRightHandCardDelegate, FCard, RightCard);

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
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDrawAndAddCardDelegate DrawAndAddCardDelegate;

	// When change left hand card
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FUpdateLeftHandCardDelegate UpdateLeftHandCardDelegate;

	// When change right hand card
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FUpdateRightHandCardDelegate UpdateRightHandCardDelegate;
	
	FCard ConvertCardNameToFCard(FName CardName);


	//Getter, Setter
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetDeckSize() const { return DeckSize; }

	FORCEINLINE int32 GetHandSize() const { return HandSize; }

	FORCEINLINE FCard GetLeftHandCard() const { return LeftHandCard; }
	FORCEINLINE FCard GetRightHandCard() const { return RightHandCard; }
	FORCEINLINE void SetLeftHandCard(FCard Card) { LeftHandCard = Card; }
	FORCEINLINE void SetRightHandCard(FCard Card) { RightHandCard = Card; }

protected:
	virtual void BeginPlay() override;
	
	void AddCard(FName NewCard);
	void DeleteCard(FName DeleteCard);

private:
	// Card data
	UPROPERTY(EditAnywhere, Category = Card)
	UDataTable* CardDataTable;

	// Card Inventory : For NonCombat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardInventory;
	
	// Card Deck : For Combat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardDeck;
	// Card Hand : For Combat
	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardHand;

	// Number of cards in hand when start combat
	UPROPERTY(EditAnywhere, Category = Card)
	int32 FirstCardCount;

	FCard LeftHandCard;

	FCard RightHandCard;

	int32 DeckSize;
	int32 HandSize = 0;

	UPROPERTY()
	AZCharacter* ZCharacter;

	void ShuffleDeck();
};
