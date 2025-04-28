// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonManager.h"

TSharedPtr<FJsonObject> UJsonManager::ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString = ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> ReturnJsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), ReturnJsonObject))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read Json file failed - Was not able to Deserialize the json string. Is it the right format? : %s"), *JsonString);
		return nullptr;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json file succeeded : %s"), *JsonFilePath);
	return ReturnJsonObject;
}

void UJsonManager::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString;

	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write Json file failed - Was not able to serialize the json string. Is the JsonObject valid?"));
		return;
	}

	WriteStringToFile(JsonFilePath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json file succeeded : %s"), *JsonFilePath);
}

FString UJsonManager::ReadStringFromFile(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read string from file failed - file does not exist : %s"), *FilePath);
		return "";
	}

	FString ReturnString = "";

	if (!FFileHelper::LoadFileToString(ReturnString, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read string from file failed - Was not able to read file. Is this a text file? : %s"), *FilePath);
		return "";
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read string from file succeeded : %s"), *FilePath);
	return ReturnString;
}

void UJsonManager::WriteStringToFile(FString FilePath, FString String, bool& bOutSuccess, FString& OutInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		bOutSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write string to file failed - Was not able to write to file. Is file read only? Is the path valid? : %s"), *FilePath);
		return;
	}

	bOutSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write string to file succeeded : %s"), *FilePath);
}
