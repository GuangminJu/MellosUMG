// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicType/MUserWidgetBasicType.h"
#include "StructUtils/InstancedStruct.h"

#include "MObjectUserWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMObjectUserWidget : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual void OnSetProperty(FProperty* InProperty) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UObject> ObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* Object;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CollectProperties();

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	
	TSubclassOf<UMUserWidgetBasicType> GetSupportedWidgetClass(const FProperty* InProperty);

	UFUNCTION(BlueprintCallable, CallInEditor)
	TArray<UMUserWidgetBasicType*> GenerateWidget();
	
	TArray<FProperty*> Properties;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UMUserWidgetBasicType>> BasicTypeWidgets;
};