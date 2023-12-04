#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterInfo.generated.h"

struct FCard;

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> DefaultCards;
};
