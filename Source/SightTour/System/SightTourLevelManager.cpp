// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SightTourLevelManager.h"
#include "SightTourGameInstance.h"

#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "MoviePlayer.h"
#include "GameFramework/GameModeBase.h"


USightTourLevelManager* USightTourLevelManager::Instance = nullptr;

void USightTourLevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Instance = this;
	Super::Initialize(Collection);
}

bool USightTourLevelManager::LoadNewLevel(FName CurrentLevelName, FName NextLevelName, FString NextPlayerStartTag)
{
	//TODO:start acting

	if (NextLevelName == OldSceneName)
	{
		//TODO:end acting
		return false;
	}

	//APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	//check(CameraManager);
	//CameraManager->StartCameraFade(0.f, 1.f, 0.f, FLinearColor::Black, true, true);

	//TODO:widget visible
	FString TestLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	FLatentActionInfo LatentActionInfo;
	/*LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "SetGameModeAndPlayerLocationByPlayerStartTag";*/
	//UGameplayStatics::LoadStreamLevel(this, NewSceneName, true, false, LatentActionInfo);

	return  false;
}

void USightTourLevelManager::BeginLoadingScreen(TSubclassOf<UUserWidget> LoadingWidgetClass)
{
	UE_LOG(LogTemp, Log, TEXT("USightTourLevelManager::BeginLoadingScreen------------->"));
	if (!IsRunningDedicatedServer())
	{

		FLoadingScreenAttributes LoadingScreen;
		//LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		//LoadingScreen.bMoviesAreSkippable = true;
		//LoadingScreen.bWaitForManualStop = true;
		//LoadingScreen.PlaybackType = EMoviePlaybackType::MT_Looped;
		//引擎默认Widget
		//LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		//自定义Widget
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetOuter()->GetWorld(), 0);
		LoadingWidget = CreateWidget<UUserWidget>(PlayerController, LoadingWidgetClass);
		if (LoadingWidget) {
			TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
			LoadingScreen.WidgetLoadingScreen = WidgetPtr;
		}

		//播放视频，视频文件需要放在Content\Movies下
		//LoadingScreen.MoviePaths.Add("squad_intro_movie");
		//GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void USightTourLevelManager::SetGameModeAndPlayerLocationByPlayerStartTag(FString PlayerStartTag)
{
	/*APlayerController* PC = USightTourGameInstance::Get()->GetCurPlayerController();
	PC->SetInputMode(FInputModeGameOnly());

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	AActor* PlayerStartActor = GameMode->FindPlayerStart(PC, PlayerStartTag);

	APawn* Pawn = USightTourGameInstance::Get()->GetCurPawn();
	Pawn->TeleportTo(PlayerStartActor->GetActorLocation(), PlayerStartActor->GetActorRotation());

	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.ExecutionFunction = 
	UKismetSystemLibrary::Delay(this, 0.1f, FLatentActionInfo)*/
}
