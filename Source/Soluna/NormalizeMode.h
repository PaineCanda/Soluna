// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */


UENUM()
enum class ENormalizeMode : uint8  {
	Local UMETA(DisplayName = "Individual Chunks"),
	Global UMETA(DisplayName = "Endless Terrain")
};
