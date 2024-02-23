// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	JohnDoe,
	Zombie
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Default")
	TSubclassOf<UGameplayEffect> PrimaryAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> DefaultCards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* CharacterPortrait;
};

UCLASS()
class PROJECTZ_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> SecondaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> VitalAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbility;

	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
