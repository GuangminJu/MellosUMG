// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "MUserWidgetIntType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetIntType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	virtual void OnSetProperty(FProperty* InProperty) override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(int32 InValue);

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	int32 GetValue() const;
	
	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	bool GetClampedValue(int32& OutMin, int32& OutMax) const;
public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	int32 Value;

private:
	FIntProperty* IntProperty;
};