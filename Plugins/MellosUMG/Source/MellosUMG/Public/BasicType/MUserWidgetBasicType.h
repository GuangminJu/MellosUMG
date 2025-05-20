// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MUserWidgetBasicType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetBasicType : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool IsPropertySupported(const FProperty* InProperty) const
	{
		return false;
	}

	void SetProperty(FProperty* InProperty)
	{
		Property = InProperty;
		OnSetProperty(Property);
	}

	void SetMemory(void* InMemory)
	{
		Memory = InMemory;
	}

	void* GetMemory() const
	{
		return Memory;
	}

	virtual void OnSetProperty(FProperty* InProperty)
	{
		
	}

	UFUNCTION(BlueprintPure)
	FText GetPropertyName() const
	{
		return Property->GetDisplayNameText();
	}
protected:
	FProperty* Property = nullptr;
	void* Memory = nullptr;
};
