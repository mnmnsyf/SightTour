// Fill out your copyright notice in the Description page of Project Settings.


#include "FillBall.h"
#include "TP_WeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SightTourCharacter.h"
#include "Equipment/EquipmentManagerComponent.h"

AFillBall::AFillBall()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	ProjectileMesh->SetupAttachment(RootComponent);

	// Use a sphere as a simple collision representation
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionSphere->InitSphereRadius(5.0f);
	CollisionSphere->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionSphere->OnComponentHit.AddDynamic(this, &AFillBall::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionSphere->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionSphere->CanCharacterStepUpOn = ECB_No;

	// Die after 3 seconds by default
	//InitialLifeSpan = 3.0f;
}

void AFillBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAttacting)
	{
		PerMove();
	}
}

void AFillBall::BeginPlay()
{
	Super::BeginPlay();

	EnableBall(true);

	bOpenPhysicSimulate = ProjectileMesh->IsSimulatingPhysics();

	//数字小球配置
	if (!BallConfig.IsNull())
	{
		if (FFillBallBase* Config = BallConfig.GetRow<FFillBallBase>(TEXT("Get Digital Projectile Config")))
		{
			SetFillBallConfig(*Config);
		}
	}
}

void AFillBall::Attract(class UTP_WeaponComponent* WeaponComp)
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
		bAttacting = true;
	}
}

void AFillBall::Spawn(class UTP_WeaponComponent* WeaponComp)
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

void AFillBall::OnEquipped()
{
	if (ensure(!OwnerWeapon || !OwnerWeapon->GetOwnerCharacter()))
	{
		return;
	}

	UEquipmentManagerComponent* EquipmentManagerComponent = UEquipmentManagerComponent::Get(OwnerWeapon->GetOwnerCharacter());
	if (ensure(EquipmentManagerComponent))
	{
		EquipmentManagerComponent->PickupFillBall(this);
		EnableBall(false);
	}
}

bool AFillBall::GeSpawnDirection(FVector& OutHitLocation)
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

bool AFillBall::TraceUnderCrosshair(FVector& OutHitLocation)
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

void AFillBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		GEngine->AddOnScreenDebugMessage(-1, 1500.0f, FColor::Red, TEXT("Projectile hit"));

		Destroy();
	}
}

void AFillBall::SetFillBallConfig(FFillBallBase& NewConfig)
{
	FillBallConfig = NewConfig;
}

void AFillBall::EnableBall(bool bEnable)
{
	this->SetActorHiddenInGame(bEnable);
}

void AFillBall::PerMove()
{
	FVector DirectionVector = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OwnerWeapon->GetMuzzelLocation()).Vector();

	SetActorLocation(GetActorLocation() + DirectionVector * AttractSpeed);

	if (GetActorLocation().Equals(OwnerWeapon->GetMuzzelLocation(), 50.f))
	{
		bAttacting = false;

		OnEquipped();
	}
}