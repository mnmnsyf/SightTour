// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourGameInstance.h"
#include "UI/SightTourHUD.h"
#include "UI/Subsystem/SightTourUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "SightTourLevelDefines.h"
#include "Enemy/EnemyCharacter.h"
#include "../Player/SightTourCharacter.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(GameFinish, "UI.Layer.Game");

USightTourGameInstance* USightTourGameInstance::Instance = nullptr;

USightTourGameInstance::USightTourGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USightTourGameInstance::Init()
{
	Instance = this;
	Super::Init();

	SightTourUIManager = USightTourUIManager::Get();

	
	if (ensure(GameLevelData))
	{
		TArray<FSightTourLevelConfig*> OutRowArray;
		GameLevelData->GetAllRows<FSightTourLevelConfig>("USightTourGameInstance::Init", OutRowArray);

		for (auto EachRow : OutRowArray)
		{
			if(!EachRow)
				continue;
			GameLevelConfigs.FindOrAdd(EachRow->LevelName);
			GameLevelConfigs[EachRow->LevelName] = *EachRow;
		}
	}
}

APlayerController* USightTourGameInstance::GetCurPlayerController()
{
	if (Instance == nullptr)
		return nullptr;
	UWorld* UEWorld = Instance->GetWorld();
	if (UEWorld == nullptr)
		return nullptr;
	return UEWorld->GetFirstPlayerController();
}

APawn* USightTourGameInstance::GetCurPawn()
{
	APlayerController* CurPlayerController = GetCurPlayerController();
	if (CurPlayerController == nullptr)
		return nullptr;
	return CurPlayerController->GetPawn();
}

int32 USightTourGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);

	/*if (ReturnVal != INDEX_NONE)
	{
		GetSubsystem<USightTourUIManager>()->NotifyPlayerAdded(NewPlayer);
	}*/
	

	return ReturnVal;
}

void USightTourGameInstance::SetFusionVisionStrength(float InStrength)
{
	APawn* CurPawn = GetCurPawn();
	if(!IsValid(CurPawn))
		return;
	ASightTourCharacter* Player = Cast<ASightTourCharacter>(CurPawn);
	if(!IsValid(Player))
		return;
	UCameraComponent* Camera = Player->GetFirstPersonCameraComponent();
	if(!IsValid(Camera))
		return;
	float Strength = FMath::Clamp(InStrength / 10.f, 0.f, 1.f);
	Camera->SetPostProcessBlendWeight(Strength);
}

void USightTourGameInstance::EnableFusionVision(bool bEnable)
{
	APawn* CurPawn = GetCurPawn();
	if (!IsValid(CurPawn))
		return;
	ASightTourCharacter* Player = Cast<ASightTourCharacter>(CurPawn);
	if (!IsValid(Player))
		return;
	UCameraComponent* Camera = Player->GetFirstPersonCameraComponent();
	if (!IsValid(Camera))
		return;
	float Strength = bEnable ? 1.f : 0.f;
	Camera->SetPostProcessBlendWeight(Strength);
}

void USightTourGameInstance::FinishCurrentLevel()
{
	//TransitToNextLevel();
	if (UpdateNextLevelName())
	{
		InnerLoadNextLevel();
	}
	//TODO:游戏结束
	else
	{
		OnGameFinish();
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString("USightTourGameInstance::FinishCurrentLevel"));
}

void USightTourGameInstance::LoseCurrentLevel()
{
	//RestartCurrentLevel();
	InnerReloadCurrentLevel();
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString("USightTourGameInstance::LoseCurrentLevel"));
}

bool USightTourGameInstance::UpdateNextLevelName()
{
	if (CurrentLevelName == NAME_None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString("USightTourGameInstance::CurrentLevelName Is Null!!!"));
		TArray<FName> levelName = GameLevelData->GetRowNames();
		CurrentLevelName = levelName[0];
	}
	else
	{
		LastLevelName = CurrentLevelName;
		if (GameLevelConfigs.Find(CurrentLevelName))
			CurrentLevelName = GameLevelConfigs[CurrentLevelName].NextLevelName;
	}

	if(CurrentLevelName == NAME_None)
		return false;
	else
		return true;
}


void USightTourGameInstance::TransitToNextLevel()
{
	InnerLoadNextLevel();
}

void USightTourGameInstance::RestartCurrentLevel()
{
	InnerReloadCurrentLevel();
}

void USightTourGameInstance::OnGameFinish()
{
	if (ensure(!GameFinishClass.IsNull()))
	{
		SightTourUIManager->PushStreamedContentToLayer_ForPlayer(GetCurPlayerController()->GetLocalPlayer(), GameFinish, GameFinishClass);
	}
}

void USightTourGameInstance::InnerReloadCurrentLevel()
{
	InnerInitLevelData();
}

void USightTourGameInstance::InnerLoadNextLevel()
{
	//下一关
	if (CurrentLevelName != NAME_None)
	{
		FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = "InnerInitLevelData";
		UGameplayStatics::LoadStreamLevel(this, CurrentLevelName, true, false, LatentActionInfo);
	}
}

void USightTourGameInstance::InnerInitLevelData()
{
	APlayerController* PC = GetCurPlayerController();

	//InputModeGameOnly
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	//玩家出生点
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	AActor* PlayerStartActor = GameMode->FindPlayerStart(PC, GameLevelConfigs[CurrentLevelName].PlayerStartTag);
	if (PlayerStartActor)
	{
		GameMode->RestartPlayerAtPlayerStart(PC, PlayerStartActor);
	}

	//敌人配置
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacter::StaticClass(), Enemies);
	for (auto& EnemyActor : Enemies)
	{
		if(AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyActor))
			Enemy->InitParam(GameLevelConfigs[CurrentLevelName].EnemyID);
	}
	
	//卸载上一个关卡
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USightTourGameInstance::InnerUnloadLastLevel);
}

void USightTourGameInstance::InnerUnloadLastLevel()
{
	if (LastLevelName != NAME_None)
	{
		FLatentActionInfo LatentActionInfo;
		UGameplayStatics::UnloadStreamLevel(this, LastLevelName, LatentActionInfo, false);
	}
}