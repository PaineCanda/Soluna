// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SolunaBaseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class USolunaBaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOLUNA_API ISolunaBaseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.


public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OrientRotationToMovement();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void OrientRotationToController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void MoveCamera(FVector TargetSocketOffset);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Character")
	void RotateCamera(float TargetYawAngle);
};
