// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SunAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class SOLUNA_API USunAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USunAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Ammo)
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ACCESSORS_BASIC(USunAttributeSet, Ammo);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxAmmo)
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ACCESSORS_BASIC(USunAttributeSet, MaxAmmo);

	UFUNCTION()
	void OnRep_Ammo(const FGameplayAttributeData& OldAmmo) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(USunAttributeSet, Ammo, OldAmmo);
	}
	UFUNCTION()
	void OnRep_MaxAmmo(const FGameplayAttributeData& OldMaxAmmo) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(USunAttributeSet, MaxAmmo, OldMaxAmmo);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
