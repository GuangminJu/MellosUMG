// Fill out your copyright notice in the Description page of Project Settings.

#include "MUserWidget.h"

void UMUserWidget::OnSetProperty(FProperty* InProperty)
{
	FStructProperty* StructProperty = CastField<FStructProperty>(InProperty);
	if (!StructProperty)
	{
		UE_LOG(LogTemp, Warning, TEXT("Property is not a struct property: %s"), *InProperty->GetName());
		return;
	}
	
	InstanceStruct.InitializeAs(StructProperty->Struct, static_cast<uint8*>(GetMemory()));
}

const UStruct* UMUserWidget::GetSupportedStruct() const
{
	return InstanceStruct.GetScriptStruct();
}

void UMUserWidget::CollectProperties()
{
	Properties.Empty();

	for (FProperty* SubProperty : TFieldRange<FProperty>(InstanceStruct.GetScriptStruct()))
	{
		Properties.Add(SubProperty);
	}
}

bool UMUserWidget::IsPropertySupported(const FProperty* InProperty) const
{
	const FStructProperty* StructProperty = CastField<FStructProperty>(InProperty);
	if (!StructProperty)
		return false;

	return InstanceStruct.GetScriptStruct() == StructProperty->Struct;
}

TSubclassOf<UMUserWidgetBasicType> UMUserWidget::GetSupportedWidgetClass(const FProperty* InProperty)
{
	for (const TSubclassOf<UMUserWidgetBasicType>& BasicType : BasicTypeWidgets)
	{
		if (!BasicType)
			continue;

		UMUserWidgetBasicType* UserWidgetBasic = BasicType.GetDefaultObject();
		if (UserWidgetBasic->IsPropertySupported(InProperty))
		{
			return BasicType;
		}
	}

	return nullptr;
}

TArray<UMUserWidgetBasicType*> UMUserWidget::GenerateWidget()
{
	TArray<UMUserWidgetBasicType*> GeneratedWidgets;

	for (FProperty* SubProperty : Properties)
	{
		if (TSubclassOf<UMUserWidgetBasicType> Class = GetSupportedWidgetClass(SubProperty))
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetProperty(SubProperty);
			Widget->SetMemory(InstanceStruct.GetMutableMemory());
			GeneratedWidgets.Add(Widget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No widget class found for property : %s"), *SubProperty->GetName());
		}
	}

	return GeneratedWidgets;
}

#if WITH_EDITOR

void UMUserWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UMUserWidget, InstanceStruct))
	{
		CollectProperties();
	}
}

#endif
