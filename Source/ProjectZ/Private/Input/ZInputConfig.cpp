// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ZInputConfig.h"

const UInputAction* UZInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FZInputAction& Action : AbilityInputAction)
	{
		if (Action.InputAction && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag %s, on InputConfig %s"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
