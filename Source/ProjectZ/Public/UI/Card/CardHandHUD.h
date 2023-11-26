// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ZUserWidget.h"
#include "CardHandHUD.generated.h"

class UCardWidget;
class UCardHandWidget;
class UCardComponent;
/**
 * 
 */
UCLASS()
class PROJECTZ_API UCardHandHUD : public UZUserWidget
{
	GENERATED_BODY()

public:
	void SetViewportSize(const FVector2D& Size) { ViewportSize = Size; }

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCardHandWidget* CardHandWidget;



private:
	
	FVector2D ViewportSize;
};
