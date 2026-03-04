// Fill out your copyright notice in the Description page of Project Settings.


#include "SunAttributeSet.h"
#include "Net/UnrealNetwork.h"

USunAttributeSet::USunAttributeSet()
{
	Ammo = 6.f;
	MaxAmmo = 6.f;
}

void USunAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(USunAttributeSet, Ammo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USunAttributeSet, MaxAmmo, COND_None, REPNOTIFY_Always);
}
