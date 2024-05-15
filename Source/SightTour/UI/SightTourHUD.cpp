// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SightTourHUD.h"

ASightTourHUD::ASightTourHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ASightTourHUD::BeginPlay()
{
	Super::BeginPlay();
}

TArray<int32> ASightTourHUD::CalculateSurroundSide(int32 Num)
{
	int32 MaxIndex;
	int32 R = FMath::Sqrt((float)Num);
	int32 SqrR = FMath::Square(R);
	MaxIndex = FMath::Square(R + 2) + Num - SqrR;
	if (Num - SqrR <= 0)
	{
		MaxIndex += -1;
	}
	else if (Num - SqrR <= R)
	{
		MaxIndex += 1;
	}
	else
	{
		MaxIndex += 3;
	}

	TArray<int32> Ans;
	for (int32 i = Num + 1; i <= MaxIndex; ++i)
	{
		Ans.Add(i);
	}
	UE_LOG(LogTemp, Log, TEXT("MaxIndex = %d"), MaxIndex);
	return Ans;
}

void ASightTourHUD::DrawSurroundSide(int32 Num)
{
	int32 Length = FMath::Sqrt((float)Num) + 2;
	int32 x = Length, y = Length;
	TMap<int32, TMap<int32, int32>> Arr;
	for (int32 i = 0; i <= Length * 3; ++i)
	{
		Arr.FindOrAdd(i);
		for (int32 j = 0; j <= Length * 3; ++j)
		{
			Arr[i].FindOrAdd(j, 0);
		}
	}
	Arr[x][y] = 1;
	int32 Direction = 0;

	int32 IndexNum = 2;
	while (IndexNum <= FMath::Square(Length))
	{	
		bool bMove = false;
		//下
		if (Direction == 0)
		{
			if (!Arr[x + 1][y])
			{
				Arr[x + 1][y] = IndexNum;
				x += 1;
				++IndexNum;
				bMove = true;
			}
		}
		//右
		else if (Direction == 1)
		{
			if (!Arr[x][y + 1])
			{
				Arr[x][y + 1] = IndexNum;
				y += 1;
				++IndexNum;
				bMove = true;
			}
		}
		// 上
		else if (Direction == 2)
		{
			if (!Arr[x - 1][y])
			{
				Arr[x - 1][y] = IndexNum;
				x -= 1;
				++IndexNum;
				bMove = true;
			}
		}
		// 左
		else if (Direction == 3)
		{
			if (!Arr[x][y - 1])
			{
				Arr[x][y - 1] = IndexNum;
				y -= 1;
				++IndexNum;
				bMove = true;
			}
		}

		if (bMove)
			Direction = (Direction + 1) % 4;
		else
			Direction = (Direction + 4 - 1) % 4;
	}

	for (int32 i = Length; i <= Length * 2; ++i)
	{
		for (int32 j = Length; j <= Length * 2; ++j)
		{
			if (Arr[i][j] == Num)
			{
				UE_LOG(LogTemp, Log, TEXT("------------Num = %d----------------"), Num);
				UE_LOG(LogTemp, Log, TEXT("Arr[%d][%d] = %d"), i, j, Arr[i][j]);
				UE_LOG(LogTemp, Log, TEXT("Arr[%d][%d] = %d"), i + 1, j, Arr[i + 1][j]);
				UE_LOG(LogTemp, Log, TEXT("Arr[%d][%d] = %d"), i, j + 1, Arr[i][j + 1]);
				UE_LOG(LogTemp, Log, TEXT("Arr[%d][%d] = %d"), i, j - 1, Arr[i][j - 1]);
				UE_LOG(LogTemp, Log, TEXT("Arr[%d][%d] = %d"), i - 1, j, Arr[i - 1][j]);
				CalculateSurroundSide(Num);
			}
		}
	}
}

void ASightTourHUD::Test()
{
	/*FScreenshotRequest ScreenshotRequest;*/
	/*ScreenshotRequest.OnScreenshotCaptured().AddUObject(this, &ASightTourHUD::DrawSurroundSide, Test2);*/
}

void ASightTourHUD::Test2(int32 Width, int32 Height, const TArray<FColor>& Colors)
{
	;
}
