#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"


UENUM(BlueprintType)
enum class ECardType : uint8
{
	ECT_Passive,
	ECT_Usable,
	ECT_Equipment
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
};

