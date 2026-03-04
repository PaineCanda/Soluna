// Fill out your copyright notice in the Description page of Project Settings.


#include "SunCharacter.h"

ASunCharacter::ASunCharacter()
{
	// Add the SunAttributeSet
	SunAttributeSet = CreateDefaultSubobject<USunAttributeSet>(TEXT("SunAttributeSet"));
}
