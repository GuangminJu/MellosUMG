// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FunctionUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELLOSUMG_API UFunctionUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetFunction(UFunction* InFunction)
	{
		Function = InFunction;
	}

	void SetObject(UObject* InObject)
	{
		Object = InObject;
	}

	UFUNCTION(BlueprintCallable, Category = "Function")
	FText GetFunctionName() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Function")
	void Call();

	UPROPERTY()
	UFunction* Function;

	UPROPERTY()
	UObject* Object;
};
