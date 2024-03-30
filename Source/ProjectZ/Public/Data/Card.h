#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

class UZGameplayAbility;
class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECardType : uint8
{
	ECT_Passive UMETA(DisplayName = "Passive"),
	ECT_UsablePassive UMETA(DisplayName = "UsablePassive"),
	ECT_OneHandWeapon UMETA(DisplayName = "OneHandWeapon"),
	ECT_TwoHandWeapon UMETA(DisplayName = "TwoHandWeapon"),
	ECT_CantEquip UMETA(DisplayName = "CantEquip"),
	ECT_Skill UMETA(DisplayName = "Skill"),
	ECT_Buff UMETA(DisplayName = "Buff"),

	ECT_Max
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTargeting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Ability")
	FGameplayTag CardTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card Ability")
	TSubclassOf<UGameplayAbility> CardAbility;
	
	bool IsValid = true;

	bool operator==(const FCard& a) const
	{
		return CardName.EqualTo(a.CardName);
	}
};
