#include "FGLevelModule.h"

AFGLevelModule::AFGLevelModule()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;
}

UActorComponent* AFGLevelModule::AddComponent(TSubclassOf<UActorComponent> Class, FName ComponentName)
{
	for(UActorComponent* Component : GetComponents())
	{
		if(Component->GetFName() == ComponentName)
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't add component, there already exists a component with input name"));
			return nullptr;
		}
	}

	UActorComponent* Component = NewObject<UActorComponent>(this, *Class, ComponentName);

	if(Component == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't create component, NewObject returned null"));
		return nullptr;
	}

	PostCreateBlueprintComponent(Component);
	Component->OnComponentCreated();

	USceneComponent* SceneComponent = Cast<USceneComponent>(Component);
	if(SceneComponent != nullptr)
	{
		USceneComponent* Root = GetRootComponent();
		if(Root == nullptr)
		{
			SetRootComponent(SceneComponent);
		}
		else
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
			SceneComponent->AttachToComponent(Root, Rules);
		}
	}

	Component->RegisterComponent();

	return Component;
}
