#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Card.generated.h"

class UImage;

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
};
