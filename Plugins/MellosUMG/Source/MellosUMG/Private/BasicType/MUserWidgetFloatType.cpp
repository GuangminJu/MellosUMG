// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicType/MUserWidgetFloatType.h"

bool UMUserWidgetFloatType::IsPropertySupported(const FProperty* InProperty) const
{
	return InProperty->IsA<FFloatProperty>();	
}

void UMUserWidgetFloatType::SetValue(float InValue)
{
	Value = InValue;

	if (FloatProperty)
	{
		if (FloatProperty->HasSetter())
		{
			FloatProperty->CallSetter(GetMemory(), &Value);
		}
		else
		{
			FloatProperty->SetPropertyValue_InContainer(GetMemory(), Value);			
		}
	}
}

float UMUserWidgetFloatType::GetValue() const
{
	if (FloatProperty)
	{
		if (FloatProperty->HasGetter())
		{
			FloatProperty->CallGetter(GetMemory(), (void*)&Value);
			return Value;
		}
		
		return *FloatProperty->GetPropertyValuePtr_InContainer(GetMemory());
	}

	return Value;
}

bool UMUserWidgetFloatType::GetClampedValue(float& OutMin, float& OutMax) const
{
	if (FloatProperty)
	{
		const FString* ClampMin = FloatProperty->FindMetaData(TEXT("ClampMin"));
		const FString* ClampMax = FloatProperty->FindMetaData(TEXT("ClampMax"));
		if (ClampMin)
		{
			OutMin = FCString::Atof(**ClampMin);
		}
		if (ClampMax)
		{
			OutMax = FCString::Atof(**ClampMax);
		}
			
		return ClampMin && ClampMax;
	}
		
	return false;
}

void UMUserWidgetFloatType::OnSetProperty(FProperty* InProperty)
{
	FloatProperty = CastField<FFloatProperty>(InProperty);
}
