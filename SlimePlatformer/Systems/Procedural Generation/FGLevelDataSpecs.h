#pragma once
#include "FGExitDoor.h"
#include "Engine/DataAsset.h"
#include "FGLevelDataSpecs.generated.h"

class AFGExitDoor;
class AFGEnemy;
class AFGObstacle;
class AFG_FollowerActor;
class AFGItem;
class UFGModuleWall;

UENUM(BlueprintType)
enum EClassType
{
	ActorComponent,
	Actor
};

USTRUCT(BlueprintType)
struct FItemWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFGItem> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};

USTRUCT(BlueprintType)
struct FObstacleWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFGObstacle> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};

USTRUCT(BlueprintType)
struct FEnemyWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFGEnemy> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
};

USTRUCT(BlueprintType)
struct FColorMappingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EClassType> ClassTypeToUse = ActorComponent;

	/** This will be added to the module as a component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UActorComponent> Class;

	/** This will be added to the module as a child actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;
};

UCLASS(BlueprintType)
class UFGLevelDataSpecs : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Each tile's size (specified in units) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize = 100.f;

	/** Each module's size (specified in tiles) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int ModuleSize = 9;

	/** The width of the corridors in the levels */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int CorridorWidth = 1;

	/** The size of one side of the level (specified in modules), level size of 3 means a grid of 3x3 modules will be generated */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int LevelSize = 3;

	/** The amount of margin between tiles, in order to generate connecting corridors if required */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int TileMarginBetweenModules = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> MoneyBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> HealthBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFGExitDoor> ExitDoorBlueprint = AFGExitDoor::StaticClass();

	/** Bigger numbers mean higher chance, changing weights for one item will affect the chance for all other items since it divides the weight by the total weight number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemWeights> ItemWeights;

	/** Bigger numbers mean higher chance, changing weights for one obstacle will affect the chance for all other obstacle since it divides the weight by the total weight number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FObstacleWeights> ObstacleWeights;

	/** Bigger numbers mean higher chance, changing weights for one enemy will affect the chance for all other enemy since it divides the weight by the total weight number */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemyWeights> EnemyWeights;

	/** The module wall component class to be used when closing corridors that shouldn't be used */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UFGModuleWall> ModuleWall;

	/** The color mappings for the level editor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FColorMappingData> ColorMappings;
};