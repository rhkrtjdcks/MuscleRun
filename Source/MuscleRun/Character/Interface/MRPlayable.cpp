
// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Interface/MRPlayable.h"

// Add default functionality here for any IMRPlayable functions that are not pure virtual.

void IMRPlayable::GetDamaged(float Damage)
{
	PURE_VIRTUAL(IMRPlayable::GetDamaged,)
}

void IMRPlayable::ItemActivated()
{
	PURE_VIRTUAL(IMRPlayable::ItemActivated,)
}
