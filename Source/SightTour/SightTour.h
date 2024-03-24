// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

//自定义Object通道类型
#define TRACE_PICKUP UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)	//可拾取的Object通道类型

//可Break程序块
#define SIGHTTOUR_BREAK_BLOCK do{
#define SIGHTTOUR__BREAK_BLOCK_END }\
while(false);