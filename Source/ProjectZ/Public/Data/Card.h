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

	// English
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardName;

	// Localization
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardDisplayName;

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
	int32 CardMaxDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECardType CardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CardLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillRange = 1.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillAngle = 360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTargeting;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowDamage = true;

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

	bool operator==(const FText& a) const
	{
		return CardName.EqualTo(a);
	}
};

USTRUCT(BlueprintType)
struct FCardCombination : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FirstCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SecondCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ResultCard;
};