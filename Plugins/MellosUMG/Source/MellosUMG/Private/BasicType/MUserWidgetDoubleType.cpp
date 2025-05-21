#include "BasicType/MUserWidgetDoubleType.h"

bool UMUserWidgetDoubleType::IsPropertySupported(const FProperty* InProperty) const
{
	return InProperty->IsA<FDoubleProperty>();
}

void UMUserWidgetDoubleType::SetValue(double InValue)
{
	Value = InValue;

	if (DoubleProperty)
	{
		DoubleProperty->SetPropertyValue_InContainer(Memory, Value);
	}
}

double UMUserWidgetDoubleType::GetValue() const
{
	if (DoubleProperty)
	{
		return DoubleProperty->GetPropertyValue_InContainer(Memory);
	}

	return Value;
}

bool UMUserWidgetDoubleType::GetClampedValue(double& OutMin, double& OutMax) const
{
	if (DoubleProperty)
	{
		const FString* ClampMin = DoubleProperty->FindMetaData(TEXT("ClampMin"));
		const FString* ClampMax = DoubleProperty->FindMetaData(TEXT("ClampMax"));
		if (ClampMin)
		{
			OutMin = FCString::Atod(**ClampMin);
		}
		if (ClampMax)
		{
			OutMax = FCString::Atod(**ClampMax);
		}
			
		return ClampMin && ClampMax;
	}

	return false;
}

void UMUserWidgetDoubleType::OnSetProperty(FProperty* InProperty)
{
	DoubleProperty = CastField<FDoubleProperty>(InProperty);
}
