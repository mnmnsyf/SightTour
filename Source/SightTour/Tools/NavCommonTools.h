/**********************************************      
        @File                NavCommonTools.h
        @Brief                导航工具类
        @Author                SheYongfei
        @Version        V1.0
        @Date                2024/03/24
        @History changes
                V1.0        NoChange
**********************************************/
#pragma once

#include "CoreMinimal.h"

class FNavCommonTools
{
public:
	/************************************************************************/
	/* @brief    将目标坐标转换为地面站立坐标                                             */
	/************************************************************************/
    static FVector ConvertStandUpGroundPos(UObject* WorldObj, const FVector& CalcPos, const float UpPos = 2000.0f, const float BottomPos = 2000.0f);

    /***************************************************************
         @brief    获取当前寻路路径点
         @param    ExtendSize:体积
    ***************************************************************/
    static TArray<FVector> FindPathToLocation(UObject* WorldObj, const FVector& StartPoint, const FVector& EndPoint);
};