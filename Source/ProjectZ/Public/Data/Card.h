#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

class UZGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECardType : uint8
{
	ECT_Passive,
	ECT_UsablePassive,
	ECT_CanEquip,
	ECT_CantEquip,
	ECT_Skill
};


USTRUCT(BlueprintType)
struct FCard : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CardBackGroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CardImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType CardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Ability")
	TSubclassOf<UZGameplayAbility> CardAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsValid = true;
};

