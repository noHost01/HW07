// PawnGameMode.cpp

// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnGameMode.h"
#include "SpherePawn.h"

APawnGameMode::APawnGameMode()
{
	DefaultPawnClass = ASpherePawn::StaticClass();
}
