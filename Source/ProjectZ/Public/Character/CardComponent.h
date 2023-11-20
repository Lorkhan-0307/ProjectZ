// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Card.h"
#include "CardComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDrawAndAddCardDelegate, FCard, NewCard);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateHandCardDelegate, int32, NewHandCount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTZ_API UCardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FCard DrawCard();

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDrawAndAddCardDelegate DrawAndAddCardDelegate;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FUpdateHandCardDelegate UpdateHandCardDelegate;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetDeckSize() const { return DeckSize; }

	FORCEINLINE int32 GetHandSize() const { return HandSize; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Card)
	UDataTable* CardDataTable;

	UPROPERTY(VisibleAnywhere, Category = Card)
	TArray<FCard> CardDeck;

	UPROPERTY(EditAnywhere, Category = Card)
	int32 FirstCardCount;

	void AddCard(FName NewCard);
	void DeleteCard(FName DeleteCard);

private:
	void ShuffleDeck();

	int32 DeckSize;
	int32 HandSize = 0;
};
