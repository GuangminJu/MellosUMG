#include "BasicType/UMUserWidgetDoubleType.h"

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

void UMUserWidgetDoubleType::OnSetProperty(FProperty* InProperty)
{
	DoubleProperty = CastField<FDoubleProperty>(InProperty);
}
