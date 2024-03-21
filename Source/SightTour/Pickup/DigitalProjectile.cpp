// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/DigitalProjectile.h"
#include "TP_WeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet\GameplayStatics.h"
#include "SightTourCharacter.h"
#include "DigitalBallDefines.h"
#include "Equipment/EquipmentManagerComponent.h"

ADigitalProjectile::ADigitalProjectile()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(RootComponent);
}

void ADigitalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoving)
	{
		PerMove();
	}
}

void ADigitalProjectile::BeginPlay()
{
	Super::BeginPlay();

	bOpenPhysicSimulate = ProjectileMesh->IsSimulatingPhysics();
}

void ADigitalProjectile::Attract(class UTP_WeaponComponent* WeaponComp)
{
	check(WeaponComp);

	OwnerWeapon = WeaponComp;
	if (bOpenPhysicSimulate)
	{
		ProjectileMesh->SetSimulatePhysics(false);
	}
	float Distance = FVector::Dist(OwnerWeapon->GetMuzzelLocation(), GetActorLocation());
	if (Distance > 0)
	{
		bMoving = true;
	}
}

void ADigitalProjectile::Spawn(class UTP_WeaponComponent* WeaponComp)
{
	if (!WeaponComp || !ProjectileMesh)
	{
		return;
	}

	if (bOpenPhysicSimulate)
	{
		ProjectileMesh->SetSimulatePhysics(true);
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FVector MuzzleLocation = WeaponComp->GetMuzzelLocation();

		//发射子弹的方向
		FRotator SpawnRotation = WeaponComp->GetMuzzelRotation();
		FVector FireHitLocation;

		if (GeSpawnDirection(FireHitLocation))
		{
			SpawnRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, FireHitLocation);
		}
		else
		{
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (ASightTourCharacter* SightTourCharacter = Cast<ASightTourCharacter>(Player))
			{
				FVector CameraDirection = SightTourCharacter->GetCameraTransform().Rotator().Vector() * 100'000.0f;
				SpawnRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, CameraDirection);
			}
		}

		ProjectileMesh->AddImpulse(SpawnRotation.Vector() * ForceValue);
	}
}

void ADigitalProjectile::OnEquipped()
{
	if (OwnerWeapon == nullptr || OwnerWeapon->GetOwner())
	{
		const FString ErrorStr = FString::Printf(TEXT("There is an error in OwnerWeapon!"));
		UE_LOG(LogTemp, Error, TEXT("%s:%d %s"), *FString(__FUNCTION__), __LINE__, *ErrorStr);
		return;
	}
	UEquipmentManagerComponent* EquipmentManagerComponent = OwnerWeapon->GetOwner()->FindComponentByClass<UEquipmentManagerComponent>();
	if (EquipmentManagerComponent == nullptr)
	{
		const FString ErrorStr = FString::Printf(TEXT("Unable to find EquipmentManagerComponent from Owner!"));
		UE_LOG(LogTemp, Error, TEXT("%s:%d %s"), *FString(__FUNCTION__), __LINE__, *ErrorStr);
		return;
	}
	if (BallConfig.IsNull())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("数字子弹配置表异常！")));
		return;
	}
	FDigitalProjectileConfig* DigitalProjectileConfig = BallConfig.GetRow<FDigitalProjectileConfig>(TEXT("Get Digital Projectile Config"));
	if (DigitalProjectileConfig == nullptr)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("未找到对应的子弹配置，请优先修改！")));
	}
	
	EquipmentManagerComponent->UpdateDigitalProjectileValue(DigitalProjectileConfig->BallNumber);
}

bool ADigitalProjectile::GeSpawnDirection(FVector& OutHitLocation)
{
	if (!OwnerWeapon)
	{
		return false;
	}

	//提前计算从视口准心方向发射子弹的轨迹终点
	FVector ViewportHitLocation;
	TraceUnderCrosshair(ViewportHitLocation);

	//在枪口和子弹轨迹终点之间是否有Blocking hit
	const FVector LineTraceStart = OwnerWeapon->GetMuzzelLocation();
	const FVector TraceDirection = ViewportHitLocation - LineTraceStart;
	const FVector LineTraceEnd = LineTraceStart + TraceDirection * 1.25f;

	FHitResult OutHitResult;
	GetWorld()->LineTraceSingleByChannel(OutHitResult, LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_Visibility);
	if (OutHitResult.bBlockingHit)
	{
		OutHitLocation = LineTraceEnd;
		return true;
	}

	return false;
}

bool ADigitalProjectile::TraceUnderCrosshair(FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	//获取十字准星的位置
	FVector2D CrosshairViewportLocation = ViewportSize / 2.0f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	//获得十字准星的世界位置和方向
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0), CrosshairViewportLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	//从十字准星方向打射线检测碰撞
	if (bScreenToWorld)
	{
		FHitResult OutHitResult;
		const FVector TraceStart = CrosshairWorldPosition;
		const FVector TraceEnd = TraceStart + CrosshairWorldDirection * 100'000.0f;
		OutHitLocation = TraceEnd;
		GetWorld()->LineTraceSingleByChannel(OutHitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void ADigitalProjectile::PerMove()
{
	FVector DirectionVector = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OwnerWeapon->GetMuzzelLocation()).Vector();

	SetActorLocation(GetActorLocation() + DirectionVector * AttractSpeed);

	if (GetActorLocation().Equals(OwnerWeapon->GetMuzzelLocation(), 50.f))
	{
		bMoving = false;

		OnEquipped();
		Destroy();
	}
}