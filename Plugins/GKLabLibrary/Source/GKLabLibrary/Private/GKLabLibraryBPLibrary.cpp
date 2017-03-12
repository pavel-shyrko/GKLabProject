// Copyright 2016-2017 Pavel Shyrko. All Rights Reserved.

#include "GKLabLibrary.h"
#include "GKLabLibraryBPLibrary.h"
#include "FileManagerGeneric.h"
#include <stdio.h>
#include <tchar.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

class CSVRow
{
public:
	std::string const& operator[](std::size_t index) const
	{
		return m_data[index];
	}

	std::size_t size() const
	{
		return m_data.size();
	}

	void readNextRow(std::istream& str)
	{
		std::string line;
		std::getline(str, line);

		std::stringstream lineStream(line);
		std::string cell;

		m_data.clear();
		while (std::getline(lineStream, cell, ','))
		{
			m_data.push_back(cell);
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty())
		{
			// If there was a trailing comma then add an empty element.
			m_data.push_back("");
		}
	}

private:
	std::vector<std::string> m_data;
};

std::istream& operator >> (std::istream& str, CSVRow& data)
{
	data.readNextRow(str);
	return str;
}

bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

std::vector<CsvElement> UGKLabLibraryBPLibrary::Elements;

UGKLabLibraryBPLibrary::UGKLabLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Elements.clear();

	FString gameConfigDir = FPaths::GameConfigDir();

	FString filePath = gameConfigDir + "/AllElements.csv";

	std::ifstream file(TCHAR_TO_UTF8(*filePath));

	CSVRow row;
	while (file >> row)
	{
		CsvElement* element = new CsvElement();

		element->Num = is_number(row[0]) ? std::stoi(row[0]) : 0;
		element->Actor = row[1];
		element->Mesh = row[2];
		element->Element = is_number(row[3]) ? std::stoi(row[3]) : 0;
		element->Comment = row[4];
		element->Description = row[5];

		Elements.push_back(*element);
	}
}

bool UGKLabLibraryBPLibrary::FindElement(std::string& actor, std::string& mesh, int element, CsvElement& result)
{
	std::vector<CsvElement>::iterator it = std::find_if(Elements.begin(), Elements.end(), [&](const CsvElement& o)
	{
		return (actor.empty() || o.Actor == actor) && (mesh.empty() || o.Mesh == mesh) && (o.Element == element);
	});

	if (it == Elements.end())
	{
		return false;
	}

	result = *it;

	return true;
}

void UGKLabLibraryBPLibrary::CreateSpecification()
{
	if (!GEngine)
	{
		return;
	}

	// Receive local players list
	TArray<APlayerController*> playerList;
	GEngine->GetAllLocalPlayerControllers(playerList);
	if (sizeof(playerList) == 0)
	{
		return;
	}

	// Recieve first local player
	ULocalPlayer* player = GEngine->FindFirstLocalPlayerFromControllerId(0);
	if (!player)
	{
		return;
	}

	UWorld* world = player->GetWorld();
	if (!world)
	{
		return;
	}

	FString strToSaveIntoFile = "";

	for (TActorIterator<AActor> ActorItr(world); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AActor *Actor = *ActorItr;
		FString objName = Actor->GetName();

		float delayDebugMessage = 10.f;

		FString strActor = "Actor:\t" + objName;
		GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Black, strActor);
		strToSaveIntoFile += strActor + "\r\n";

		TArray<UStaticMeshComponent*> Components;
		Actor->GetComponents<UStaticMeshComponent>(Components);

		for (int32 i = 0; i < Components.Num(); i++)
		{
			//UStaticMesh* StaticMesh = Components[i];
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

			if (StaticMesh != NULL)
			{
				TArray<UMaterialInterface*> Materials = StaticMesh->Materials_DEPRECATED;
				FString staticMesh = StaticMesh->GetName();

				FString strMesh = objName + " Mesh:\t" + staticMesh;
				//GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Black, strMesh);
				strToSaveIntoFile += "\t" + strMesh + "\r\n";

				for (int32 j = 0; j < Materials.Num(); j++)
				{
					UMaterialInstance* originalMaterial = (UMaterialInstance*)Materials[j];
					if (originalMaterial != NULL)
					{
						FString strOriginalName = originalMaterial->GetName();

						FString strMaterial = objName + " Mesh\t" + staticMesh + " Element[" + FString::FromInt(j) + "] Original Material: " + strOriginalName;
						GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Green, strMaterial);
						strToSaveIntoFile += "\t" + strMaterial + "\r\n";
					}

					UMaterialInstance* currentMaterial = (UMaterialInstance*)StaticMeshComponent->GetMaterial(j);
					if (currentMaterial != NULL)
					{
						FString description;
						CsvElement fObj;
						std::string actorStr(TCHAR_TO_UTF8(*objName));
						std::string meshStr(TCHAR_TO_UTF8(*staticMesh));
						if (FindElement(actorStr, meshStr, j, fObj))
						{
							description = FString(fObj.Description.c_str());
						}

						FString strCurrentName = currentMaterial->GetName();

						FString strMaterial = objName + " Mesh\t" + staticMesh + " Element[" + FString::FromInt(j) + "] Current Material: " + strCurrentName;
						GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Red, strMaterial);
						strToSaveIntoFile += "\t" + strMaterial + "\r\n";

						FString strCurrentTexture;
						UTexture* currentTexture;
						if (currentMaterial->GetTextureParameterOverrideValue(FName("Diffuse"), currentTexture))
						{
							strCurrentTexture = currentTexture->GetName();
							FString strTexture = objName + " Mesh\t" + staticMesh + " Element[" + FString::FromInt(j) + "] Current Material " + strCurrentName + " Texture: " + strCurrentTexture;
							GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Red, strTexture);
							strToSaveIntoFile += "\t" + strTexture + "\r\n";
						}

						FString strCurrentColor;
						FLinearColor currentColor = FLinearColor();
						if (currentMaterial->GetVectorParameterValue(FName("Color"), currentColor))
						{
							strCurrentColor = "R: " + FString::FromInt((currentColor.R * 255))
								+ ", G: " + FString::FromInt((currentColor.G * 255))
								+ ", B: " + FString::FromInt((currentColor.B * 255));
							FString strColor = objName + " Mesh\t" + staticMesh + " Element[" + FString::FromInt(j) + "] Current Material " + strCurrentName + " Color: " + strCurrentColor;
							GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Red, strColor);
							strToSaveIntoFile += "\t" + strColor + "\r\n";
						}

						if (!description.IsEmpty())
						{
							strToSaveIntoFile += "\t" + description + ": Material=" + strCurrentName;
							if (!strCurrentTexture.IsEmpty())
							{
								strToSaveIntoFile += "; Texture=" + strCurrentTexture;
							}
							if (!strCurrentColor.IsEmpty())
							{
								strToSaveIntoFile += "; Color=" + strCurrentColor;
							}
							strToSaveIntoFile += "\r\n";
						}
					}
				}
			}
		}
		strToSaveIntoFile += "\r\n";
		//}
	}

	FDateTime CurrentDateTime = (FDateTime()).Now();

	FString gameSaveDir = FPaths::GameSavedDir();
	FString gamePlayer = player->GetName();

	FString savePlayerDirectory = gameSaveDir + gamePlayer;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectory(*savePlayerDirectory))
	{
		FString FileName = gamePlayer + "-" + CurrentDateTime.ToString() + ".txt";

		FString AbsoluteFileName = savePlayerDirectory + "/" + FileName;

		FFileHelper::SaveStringToFile(*strToSaveIntoFile, *AbsoluteFileName);
	}
}


void UGKLabLibraryBPLibrary::CreateFileOfElements(FString FileName, bool AddDateTime)
{
	if (!GEngine)
	{
		return;
	}

	// Receive local players list
	TArray<APlayerController*> playerList;
	GEngine->GetAllLocalPlayerControllers(playerList);
	if (sizeof(playerList) == 0)
	{
		return;
	}

	// Recieve first local player
	ULocalPlayer* player = GEngine->FindFirstLocalPlayerFromControllerId(0);
	if (!player)
	{
		return;
	}

	UWorld* world = player->GetWorld();
	if (!world)
	{
		return;
	}

	FString strToSaveIntoFile = "Num,Level,""Actor Name"",""Actor FName"",""Actor UniqueID"",""Actor FullName"",Mesh,Element,Material,Comment,""Document Placeholder"",""Document Text""\r\n";
	int csvRecordNum = 1;

	for (FConstLevelIterator levelItr = world->GetLevelIterator(); levelItr; ++levelItr)
	{
		ULevel *level = *levelItr;

		if (!level)
		{
			continue;
		}

		FString strLevelName = level->GetName();

		TArray<AActor *> actors = level->Actors;
		for (int32 a = 0; a < actors.Num(); a++)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			AActor *actor = actors[a];
			if (!actor)
			{
				continue;
			}

			FString strActorUniqueID = FString::FromInt(actor->GetUniqueID());
			FString strActorFullName = actor->GetFullName();
			FString strActorName = actor->GetName();
			FString strActorFName = actor->GetFName().ToString();

			TArray<UStaticMeshComponent*> Components;
			actor->GetComponents<UStaticMeshComponent>(Components);

			for (int32 i = 0; i < Components.Num(); i++)
			{
				UStaticMeshComponent* staticMeshComponent = Components[i];
				UStaticMesh* staticMesh = staticMeshComponent->GetStaticMesh();

				if (staticMesh != NULL)
				{
					FString strMeshFullName = staticMesh->GetFullName();

					for (int32 j = 0; j < staticMeshComponent->GetNumMaterials(); j++)
					{
						UMaterialInstance* currentMaterial = (UMaterialInstance*)staticMeshComponent->GetMaterial(j);

						if (currentMaterial != NULL)
						{
							strToSaveIntoFile += FString::FromInt(csvRecordNum) 
								+ "," + strLevelName 
								+ "," + strActorName
								+ "," + strActorFName
								+ "," + strActorUniqueID
								+ "," + strActorFullName 
								+ "," + strMeshFullName 
								+ "," + FString::FromInt(j) 
								+ "," + currentMaterial->GetFullName()
								+ ",,\r\n"; // Comment,""Document Placeholder"",""Document Text"" are empty
							
							csvRecordNum++;
						}
					}
				}
			}
		}
	}

	FString gameSaveDir = FPaths::GameSavedDir();
	FString gamePlayer = player->GetName();

	FString savePlayerDirectory = gameSaveDir + gamePlayer;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectory(*savePlayerDirectory))
	{
		if (AddDateTime) 
		{
			FDateTime CurrentDateTime = (FDateTime()).Now();
			FileName = FileName + "-" + CurrentDateTime.ToString();
		}

		FileName += ".csv";

		FString AbsoluteFileName = savePlayerDirectory + "/" + FileName;

		FFileHelper::SaveStringToFile(*strToSaveIntoFile, *AbsoluteFileName);
	}
}

void GetMaterialsInFolder(
	FString Directory,
	FString Path,
	TArray<UMaterialInstance*>* materialInstances,
	TArray<UMaterialInterface*>* materials,
	UObjectLibrary* lib)
{
	TArray<FString> output;
	output.Empty();
	if (FPaths::DirectoryExists(Directory))
	{
		FString path = Directory + "/*.*";
		FFileManagerGeneric::Get().FindFiles(output, *path, false, true);

		for (int i = 0; i < output.Num(); i++)
		{
			FString subDirectory = Directory + "/" + output[i];

			FString subPath = Path + "/" + output[i];
			lib->LoadAssetDataFromPath(subPath);
			TArray<FAssetData> assetData;
			lib->GetAssetDataList(assetData);

			for (FAssetData asset : assetData) {
				UObject* obj = asset.GetAsset();
				UMaterialInstance* mi = Cast<UMaterialInstance>(obj);
				if (mi) {
					materialInstances->Add(mi);
				}
				else
				{
					UMaterialInterface* m = Cast<UMaterialInterface>(obj);
					if (m) {
						materials->Add(m);
					}
				}
			}

			GetMaterialsInFolder(subDirectory, subPath, materialInstances, materials, lib);
		}
	}
}

void UGKLabLibraryBPLibrary::CreateFileOfMaterials(FString FileName, bool AddDateTime)
{
	FString contentDir = FPaths::GameContentDir();

	FPaths::NormalizeDirectoryName(contentDir);

	TArray<UMaterialInstance*>* materialInstances = new TArray<UMaterialInstance*>();
	TArray<UMaterialInterface*>* materials = new TArray<UMaterialInterface*>();

	UObjectLibrary *lib = UObjectLibrary::CreateLibrary(UMaterialInstance::StaticClass(), false, true);
	//lib->AddToRoot();
	GetMaterialsInFolder(contentDir, "/Game", materialInstances, materials, lib);
}

//
//void UPSBlueprintFunctionLibrary::LoadMaterialsCSV()
//{
//	FString contentDir = FPaths::GameContentDir();
//	FString enginePluginsDir = FPaths::EnginePluginsDir();
//	FString rootDir = FPaths::RootDir();
//	FString gameDir = FPaths::GameDir();
//	FString gameUserDir = FPaths::GameUserDir();
//	FString gameConfigDir = FPaths::GameConfigDir();
//	FString gameSaveDir = FPaths::GameSavedDir();
//	FString gamePluginsDir = FPaths::GamePluginsDir();
//
//	FString csvFile = gamePluginsDir + "/PSToolSuite/Resources/Materials.csv";
//	if (FPaths::FileExists(csvFile))
//	{
//		FString FileContent;
//		//Read the csv file
//		FFileHelper::LoadFileToString(FileContent, *csvFile);
//
//		TArray<FString> problems = Materials->CreateTableFromCSVString(FileContent);
//
//		//if (problems.Num() > 0)
//		//{
//		//	for (int32 ProbIdx = 0; ProbIdx < problems.Num(); ProbIdx++)
//		//	{
//		//		//Log the errors
//		//	}
//		//}
//		//else
//		//{
//		//	//Updated Successfully
//		//}
//	}
//}
