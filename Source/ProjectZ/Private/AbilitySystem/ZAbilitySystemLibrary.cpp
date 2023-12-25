// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ZAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/ZPlayerState.h"
#include "Ui/HUD/ZHUDBase.h"
#include "UI/WidgetController/ZWidgetController.h"

UOverlayWidgetController* UZAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AZHUDBase* ZHUD=Cast<AZHUDBase>(PC->GetHUD()))
		{
			AZPlayerState* PS = PC->GetPlayerState<AZPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return ZHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
