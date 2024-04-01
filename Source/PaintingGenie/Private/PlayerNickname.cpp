// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNickname.h"
#include <Components/TextBlock.h>

void UPlayerNickname::SetNickName(FString nickName)
{
	text_NickName->SetText(FText::FromString(nickName));
}
