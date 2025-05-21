// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "MUserWidgetFloatType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetFloatType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	virtual void OnSetProperty(FProperty* InProperty) override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(float InValue);

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	float GetValue() const;

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	bool GetClampedValue(float& OutMin, float& OutMax) const;
public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	float Value;

private:
	FFloatProperty* FloatProperty;
};
