// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FunctionUserWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCall, UFunctionUserWidget*, InWidget);

UCLASS(Abstract)
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

	// Some widget like check button ui will uncheck when call
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Function")
	void Revoke();

	UPROPERTY()
	UFunction* Function;

	UPROPERTY()
	UObject* Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function", meta=(ExposeOnSpawn))
	FText FunctionNameOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function", BlueprintAssignable)
	FOnCall PreCall;
};
