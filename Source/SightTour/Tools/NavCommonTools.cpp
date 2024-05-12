#include "NavCommonTools.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

FVector FNavCommonTools::ConvertStandUpGroundPos(UObject* WorldObj, const FVector& CalcPos, const float UpPos /*= 2000.0f*/, const float BottomPos /*= 2000.0f*/)
{
	check(WorldObj);

	FVector LineStart = CalcPos;
	LineStart.Z += UpPos;
	FVector LineEnd = CalcPos;
	LineEnd.Z -= BottomPos;
	FHitResult HitResult(ForceInit);
	FCollisionQueryParams TraceParams;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(EObjectTypeQuery::ObjectTypeQuery1);
	FCollisionObjectQueryParams const ObjParam(ObjectType);
	FHitResult hit(ForceInit); //初始化射线类并且发射一个射线

	//UKismetSystemLibrary::DrawDebugArrow(UMOGameInstance::Get()->GetWorld(),LineStart,LineEnd,100,FLinearColor::Green,20.0f,200);

	WorldObj->GetWorld()->LineTraceSingleByObjectType(HitResult, LineStart, LineEnd, ObjParam, TraceParams);

	if (HitResult.bBlockingHit)
	{
		return HitResult.ImpactPoint;
	}

	return CalcPos;
}

TArray<FVector> FNavCommonTools::FindPathToLocation(UObject* WorldObj, const FVector& StartPoint, const FVector& EndPoint)
{
	TArray<FVector> PathEmpty;
	UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(WorldObj);
	if (NavigationSystem && !NavigationSystem->IsNavigationBuildInProgress())
	{
		ANavigationData* NavMesh = NavigationSystem->GetDefaultNavDataInstance();
		FSharedConstNavQueryFilter Filter = UNavigationQueryFilter::GetQueryFilter(*NavMesh, WorldObj, nullptr);

		UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(WorldObj, StartPoint, EndPoint, (AActor*)NavMesh, nullptr);
		if (Path != nullptr)
		{
			return Path->PathPoints;
		}
	}

	return PathEmpty;
}

