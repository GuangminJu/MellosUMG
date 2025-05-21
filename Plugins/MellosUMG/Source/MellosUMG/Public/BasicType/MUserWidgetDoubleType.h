// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "MUserWidgetDoubleType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetDoubleType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	virtual void OnSetProperty(FProperty* InProperty) override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(double InValue);

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	double GetValue() const;

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	bool GetClampedValue(double& OutMin, double& OutMax) const;
public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	double Value;

private:
	FDoubleProperty* DoubleProperty;
};
