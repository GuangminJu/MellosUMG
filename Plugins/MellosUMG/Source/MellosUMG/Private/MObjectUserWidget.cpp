#include "MObjectUserWidget.h"

#include "FunctionWidget/FunctionUserWidget.h"

UMObjectUserWidget::UMObjectUserWidget(): Object(nullptr)
{
}

void UMObjectUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UMObjectUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!Object)
	{
		Object = NewObject<UObject>(this, ObjectClass);
		SetMemory(Object);
		CollectProperties();
	}
}

void UMObjectUserWidget::OnSetProperty(FProperty* InProperty)
{
	FObjectProperty* ObjectProperty = CastField<FObjectProperty>(InProperty);
	Object = ObjectProperty->GetObjectPropertyValue(GetMemory());
	ensureMsgf(Object->GetClass() == ObjectClass, TEXT("Object class mismatch. Expected: %s, Actual: %s"),
	           *ObjectClass->GetName(), *Object->GetClass()->GetName());
}

TArray<FProperty*> UMObjectUserWidget::GetProperties()
{
	CollectProperties();

	return Properties;
}

void UMObjectUserWidget::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	TryUpdateDefaultConfigFile();
}

void UMObjectUserWidget::SetObject(UObject* InObject)
{
	if (InObject->GetClass() != ObjectClass)
	{
		ensureMsgf(false, TEXT("Object class mismatch. Expected: %s, Actual: %s"),
		           *ObjectClass->GetName(), *InObject->GetClass()->GetName());
		return;
	}

	Object = InObject;
	SetMemory(Object);

	for (UUserWidget* Widget : GeneratedWidgets)
	{
		if (UMUserWidgetBasicType* UmUserWidgetBasic = Cast<UMUserWidgetBasicType>(Widget))
		{
			UmUserWidgetBasic->SetMemory(Object);
		}
		else if (UFunctionUserWidget* FunctionWidget = Cast<UFunctionUserWidget>(Widget))
		{
			FunctionWidget->SetObject(Object);
		}
		else
		{
			ensureMsgf(false, TEXT("Widget is not of type UMUserWidgetBasicType or UFunctionUserWidget: %s"), *Widget->GetName());
		}
	}
}

void UMObjectUserWidget::CollectProperties()
{
	Properties.Reset();
	for (FProperty* SubProperty : TFieldRange<FProperty>(ObjectClass))
	{
		Properties.Add(SubProperty);

		if (PropertySettings.Contains(SubProperty->GetName()))
			continue;

		FPropertySettings PropertySetting;
		PropertySetting.Name = SubProperty->GetName();
		PropertySettings.Add(PropertySetting);
	}

	Functions.Empty();
	for (UFunction* Function : TFieldRange<UFunction>(ObjectClass))
	{
		if (!Function->GetName().IsEmpty()
			&& Function->HasAnyFunctionFlags(FUNC_BlueprintCallable)
			&& Function->GetOwnerClass() == ObjectClass
			&& Function->NumParms == 0
			&& Function->GetReturnProperty() == nullptr)
		{
			Functions.Add(Function);

			if (FunctionSettings.Contains(Function->GetName()))
				continue;

			FFunctionSettings FunctionSetting;
			FunctionSetting.Name = Function->GetName();
			FunctionSettings.Add(FunctionSetting);
		}
	}

	Algo::Reverse(Functions);
	Algo::Reverse(FunctionSettings);

	TryUpdateDefaultConfigFile();
}

bool UMObjectUserWidget::IsPropertySupported(const FProperty* InProperty) const
{
	const FObjectProperty* InObjectProperty = CastField<FObjectProperty>(InProperty);
	return InObjectProperty->PropertyClass == ObjectClass;
}

TSubclassOf<UMUserWidgetBasicType> UMObjectUserWidget::GetSupportedWidgetClass(const FProperty* InProperty)
{
	for (const TSubclassOf<UMUserWidgetBasicType>& BasicTypeWidget : BasicTypeWidgets)
	{
		UMUserWidgetBasicType* UserWidgetBasic = BasicTypeWidget.GetDefaultObject();
		if (UserWidgetBasic->IsPropertySupported(InProperty))
		{
			return BasicTypeWidget;
		}
	}

	return nullptr;
}

TArray<UUserWidget*> UMObjectUserWidget::GenerateWidget()
{
	if (!Object)
	{
		Object = NewObject<UObject>(this, ObjectClass);
	}

	GeneratedWidgets.Empty();

	for (FProperty* SubProperty : Properties)
	{
		const FPropertySettings& SubPropertySettings = GetPropertySettings(SubProperty);
		TSubclassOf<UMUserWidgetBasicType> Class = SubPropertySettings.WidgetClassOverride;
		if (Class)
		{
			if (!Class.GetDefaultObject()->IsPropertySupported(SubProperty))
			{
				UE_LOG(LogTemp, Warning, TEXT("Widget class %s does not support property %s"), *Class->GetName(),
				       *SubProperty->GetName());

				Class = nullptr;
			}
		}

		if (!Class)
		{
			Class = GetSupportedWidgetClass(SubProperty);
		}

		if (Class)
		{
			UMUserWidgetBasicType* Widget = NewObject<UMUserWidgetBasicType>(this, *Class);
			Widget->SetMemory(Object);
			Widget->SetProperty(SubProperty);
			GeneratedWidgets.Add(Widget);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No widget class found for property : %s"), *SubProperty->GetName());
		}
	}

	if (!FunctionWidgetClass)
	{
		return GeneratedWidgets;
	}

	for (int32 i = 0; i < Functions.Num(); ++i)
	{
		const FFunctionSettings& Settings = GetFunctionSettings(Functions[i]);
		if (Settings.bGenerateUI)
		{
			TSubclassOf<UFunctionUserWidget> WidgetClassOverride = FunctionWidgetClass;

			if (Settings.WidgetClassOverride)
			{
				WidgetClassOverride = Settings.WidgetClassOverride;
			}

			UFunctionUserWidget* Widget = NewObject<UFunctionUserWidget>(this, WidgetClassOverride);
			Widget->SetObject(Object);
			Widget->SetFunction(Functions[i]);
			GeneratedWidgets.Add(Widget);
		}
	}

	return GeneratedWidgets;
}

void UMObjectUserWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
