// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PullAndPush, Log, All);

#define PPLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define PPLOG_S(Verbosity) UE_LOG(PullAndPush, Verbosity, TEXT("%s"), *PPLOG_CALLINFO)
#define PPLOG(Verbosity, Format, ...) UE_LOG(PullAndPush, Verbosity, TEXT("%s \'%s\'"), *PPLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
