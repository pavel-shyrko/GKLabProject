#include "GKLabLibrary.h"
#include "GKLabLibraryBPLibrary.h"
#include "FileManagerGeneric.h"
#include "CsvRow.h"
#include <string>

std::wistream& operator >> (std::wistream& str, CSVRow& data)
{
	data.readNextRow(str);
	return str;
}

bool is_number(const std::wstring& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

std::wstring ConvertToWString(const FString str)
{
	std::string str1(TCHAR_TO_UTF8(*str));
	std::wostringstream wstm;
	const std::ctype<wchar_t>& ctfacet = std::use_facet< std::ctype<wchar_t> >(wstm.getloc());
	for (size_t i = 0; i < str1.size(); ++i)
		wstm << ctfacet.widen(str1[i]);
	return wstm.str();
};

std::vector<CsvMaterial> UGKLabLibraryBPLibrary::ProjectMaterials;

std::vector<CsvElement> UGKLabLibraryBPLibrary::ProjectElements;

UGKLabLibraryBPLibrary::UGKLabLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ProjectElements.clear();

	FString gameConfigDir = FPaths::GameConfigDir();

	FPaths::NormalizeDirectoryName(gameConfigDir);

	FString elementsFilePath = gameConfigDir + "/Elements.csv";

	std::wifstream elementsFile(TCHAR_TO_UTF8(*elementsFilePath));

	CSVRow elementCsvRow;
	while (elementsFile >> elementCsvRow)
	{
		CsvElement* element = new CsvElement();

		element->Num = is_number(elementCsvRow[0]) ? std::stoi(elementCsvRow[0]) : 0;
		element->Level = elementCsvRow[1];
		element->Actor = elementCsvRow[2];
		element->UniqueID = elementCsvRow[3];
		element->ActorFullName = elementCsvRow[4];
		element->Mesh = elementCsvRow[5];
		element->Element = is_number(elementCsvRow[6]) ? std::stoi(elementCsvRow[6]) : 0;
		element->Material = elementCsvRow[7];
		element->Comment = elementCsvRow[8];
		element->DocumentPlaceholder = elementCsvRow[9];
		element->DocumentText = elementCsvRow[10];

		ProjectElements.push_back(*element);
	}

	ProjectMaterials.clear();

	FString materialsFilePath = gameConfigDir + "/Materials.csv";

	std::wifstream materialFile(TCHAR_TO_UTF8(*materialsFilePath));

	CSVRow materialCsvRow;
	while (materialFile >> materialCsvRow)
	{
		CsvMaterial* material = new CsvMaterial();

		material->Num = is_number(materialCsvRow[0]) ? std::stoi(materialCsvRow[0]) : 0;
		material->Material = materialCsvRow[1];
		material->MaterialFullName = materialCsvRow[2];
		material->Texture = materialCsvRow[3];
		material->ColorRed = materialCsvRow[4];
		material->ColorGreen = materialCsvRow[5];
		material->ColorBlue = materialCsvRow[6];
		material->Comment = materialCsvRow[7];
		material->DocumentText = materialCsvRow[8];

		ProjectMaterials.push_back(*material);
	}
}

void CreateFile(std::wstring Data, FString palyerName, FString FileName, FString FileExtension, bool AddDateTime)
{
	FString gameSaveDir = FPaths::GameSavedDir();
	FPaths::NormalizeDirectoryName(gameSaveDir);
	FString savePlayerDirectory = gameSaveDir + "/" + palyerName;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectory(*savePlayerDirectory))
	{
		if (AddDateTime)
		{
			FDateTime CurrentDateTime = (FDateTime()).Now();
			FileName = FileName + "-" + CurrentDateTime.ToString();
		}

		FileName += FileExtension;

		FString AbsoluteFileName = savePlayerDirectory + "/" + FileName;

		std::wofstream resultStream;
		resultStream.open(TCHAR_TO_UTF8(*AbsoluteFileName));
		resultStream << Data;
		resultStream.close();
	}
}

bool UGKLabLibraryBPLibrary::FindElement(std::wstring& level, std::wstring& actor, std::wstring& mesh, int element, CsvElement& result)
{
	std::vector<CsvElement>::iterator it = std::find_if(ProjectElements.begin(), ProjectElements.end(), [&](const CsvElement& o)
	{
		return (level.empty() || o.Level == level)
			&& (actor.empty() || o.ActorFullName == actor) 
			&& (mesh.empty() || o.Mesh == mesh) 
			&& (o.Element == element);
	});

	if (it == ProjectElements.end())
	{
		return false;
	}

	result = *it;

	return true;
}


bool UGKLabLibraryBPLibrary::FindMaterial(std::wstring& material, std::wstring& texture, CsvMaterial& result)
{
	std::vector<CsvMaterial>::iterator it = std::find_if(ProjectMaterials.begin(), ProjectMaterials.end(), [&](const CsvMaterial& o)
	{
		return (material.empty() || o.MaterialFullName == material)
			&& (texture.empty() || o.Texture == texture);
	});

	if (it == ProjectMaterials.end())
	{
		return false;
	}

	result = *it;

	return true;
}

void UGKLabLibraryBPLibrary::CreateSpecification(FString FileName, bool AddDateTime)
{
	if (GEngine == NULL)
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
	if (player == NULL)
	{
		return;
	}

	UWorld* world = player->GetWorld();
	if (world == NULL)
	{
		return;
	}

	std::wstring strToSaveIntoFile = L"";

	ULevel* level = world->GetCurrentLevel();
	if (level == NULL)
	{
		return;
	}

	FString strLevelName = level->GetName();

	TArray<AActor *> actors = level->Actors;
	for (int32 a = 0; a < actors.Num(); a++)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AActor* Actor = actors[a];
		if (Actor == NULL) {
			continue;
		}

		if (Actor->bHidden) {
			continue;
		}

		FString actorFullName = Actor->GetFullName();

		TArray<UStaticMeshComponent*> Components;
		Actor->GetComponents<UStaticMeshComponent>(Components);

		for (int32 i = 0; i < Components.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

			if (StaticMesh != NULL)
			{
				TArray<UMaterialInterface*> materials = StaticMeshComponent->GetMaterials();
				FString staticMeshFullName = StaticMesh->GetFullName();

				for (int32 j = 0; j < materials.Num(); j++)
				{
					UMaterialInstance* currentMaterial = (UMaterialInstance*)StaticMeshComponent->GetMaterial(j);
					if (currentMaterial != NULL)
					{
						std::wstring elementDescription;
						CsvElement fElement;
						std::wstring levelStr(ConvertToWString(TCHAR_TO_UTF8(*strLevelName)));
						std::wstring actorStr(ConvertToWString(TCHAR_TO_UTF8(*actorFullName)));
						std::wstring meshStr(ConvertToWString(TCHAR_TO_UTF8(*staticMeshFullName)));
						if (FindElement(levelStr, actorStr, meshStr, j, fElement))
						{
							elementDescription = fElement.DocumentText;
						}

						if (elementDescription.size() == 0)
						{
							continue;
						}

						FString strMaterialName = currentMaterial->GetFullName();

						FString strCurrentTexture;
						UTexture* currentTexture;
						if (currentMaterial->GetTextureParameterOverrideValue(FName("Diffuse"), currentTexture))
						{
							strCurrentTexture = currentTexture->GetName();
						}

						std::wstring materialDescription = ConvertToWString(TCHAR_TO_UTF8(*strMaterialName));
						CsvMaterial fMaterial;
						std::wstring materialName(ConvertToWString(TCHAR_TO_UTF8(*strMaterialName)));
						std::wstring textureName(ConvertToWString(TCHAR_TO_UTF8(*strCurrentTexture)));
						if (FindMaterial(materialName, textureName, fMaterial))
						{
							materialDescription = fMaterial.DocumentText;
						}

						FString strCurrentColor;
						FLinearColor currentColor = FLinearColor();
						if (currentMaterial->GetVectorParameterValue(FName("Color"), currentColor))
						{
							strCurrentColor = "rgb(" + FString::FromInt((currentColor.R * 255))
								+ "," + FString::FromInt((currentColor.G * 255))
								+ "," + FString::FromInt((currentColor.B * 255))
								+ ")";
						}

						std::wstringstream itemStream;
						itemStream << elementDescription << L": Material=" << materialDescription;
						if (!strCurrentTexture.IsEmpty())
						{
							itemStream << L"; Texture=" << ConvertToWString(strCurrentTexture);
						}
						if (!strCurrentColor.IsEmpty())
						{
							itemStream << L"; Color=" << ConvertToWString(strCurrentColor);
						}
						itemStream << std::endl;

						strToSaveIntoFile += itemStream.str();
					}
				}
			}
		}
	}

	if (FileName.IsEmpty())
	{
		FileName = "Specification";
	}

	CreateFile(strToSaveIntoFile, player->GetName(), FileName, ".txt", AddDateTime);
}


void UGKLabLibraryBPLibrary::CreateFileOfElements(FString FileName, bool AddDateTime)
{
	if (GEngine == NULL)
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
	if (player == NULL)
	{
		return;
	}

	UWorld* world = player->GetWorld();
	if (world == NULL)
	{
		return;
	}

	std::wstring strToSaveIntoFile = L"Num,Level,\"Actor Name\",\"Actor UniqueID\",\"Actor FullName\",Mesh,Element,Material,Comment,\"Document Placeholder\",\"Document Text\"\n";
	int csvRecordNum = 1;

	for (FConstLevelIterator levelItr = world->GetLevelIterator(); levelItr; ++levelItr)
	{
		ULevel *level = *levelItr;

		if (level == NULL)
		{
			continue;
		}

		FString strLevelName = level->GetName();

		TArray<AActor *> actors = level->Actors;
		for (int32 a = 0; a < actors.Num(); a++)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			AActor *actor = actors[a];
			if (actor == NULL)
			{
				continue;
			}

			FString strActorUniqueID = FString::FromInt(actor->GetUniqueID());
			FString strActorFullName = actor->GetFullName();
			FString strActorName = actor->GetName();

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
							std::wstringstream itemStream;

							itemStream << ConvertToWString(FString::FromInt(csvRecordNum))
								<< L"," << ConvertToWString(strLevelName)
								<< L"," << ConvertToWString(strActorName)
								<< L"," << ConvertToWString(strActorUniqueID)
								<< L"," << ConvertToWString(strActorFullName)
								<< L"," << ConvertToWString(strMeshFullName)
								<< L"," << ConvertToWString(FString::FromInt(j))
								<< L"," << ConvertToWString(currentMaterial->GetFullName())
								<< L",,"; // Comment,""Document Placeholder"",""Document Text"" are empty
							itemStream << std::endl;

							strToSaveIntoFile += itemStream.str();

							csvRecordNum++;
						}
					}
				}
			}
		}
	}

	if (FileName.IsEmpty())
	{
		FileName = "Elements";
	}

	CreateFile(strToSaveIntoFile, player->GetName(), FileName, ".csv", AddDateTime);
}

void LoadMaterialsInFolder(
	FString Directory,
	FString Path,
	TArray<UMaterialInterface*>* materials)
{
	FString path = Directory + "/*.uasset";
	TArray<FString> files;
	files.Empty();
	FFileManagerGeneric::Get().FindFiles(files, *path, true, false);
	for (int i = 0; i < files.Num(); i++)
	{
		size_t lastindex = files[i].Find(".", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		FString fileName = files[i].Left(lastindex);

		FString materialInstanceAssetPath = "MaterialInstanceConstant'" + Path + "/" + fileName + "." + fileName + "'";

		auto materialInstanceClass = StaticLoadObject(UObject::StaticClass(), nullptr, *materialInstanceAssetPath);
		UMaterialInstance * materialInstance = Cast<UMaterialInstance>(materialInstanceClass);

		if (materialInstance != NULL) {
			materials->Add(materialInstance);
		}
		else {
			FString materialAssetPath = "Material'" + Path + "/" + fileName + "." + fileName + "'";
			auto materialClass = StaticLoadObject(UObject::StaticClass(), nullptr, *materialAssetPath);
			UMaterialInterface * material = Cast<UMaterialInterface>(materialClass);

			if (material != NULL) {
				materials->Add(material);
			}
		}
	}
}

void GetMaterialsInFolder(
	FString Directory,
	FString Path,
	TArray<UMaterialInterface*>* materials)
{
	if (FPaths::DirectoryExists(Directory))
	{
		LoadMaterialsInFolder(Directory, Path, materials);

		FString path = Directory + "/*.*";
		TArray<FString> subDirs;
		subDirs.Empty();
		FFileManagerGeneric::Get().FindFiles(subDirs, *path, false, true);
		for (int i = 0; i < subDirs.Num(); i++)
		{
			FString subDirectory = Directory + "/" + subDirs[i];
			FString subPath = Path + "/" + subDirs[i];
			GetMaterialsInFolder(subDirectory, subPath, materials);
		}
	}
}

void UGKLabLibraryBPLibrary::CreateFileOfMaterials(FString FileName, bool AddDateTime)
{
	if (GEngine == NULL)
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
	if (player == NULL)
	{
		return;
	}

	UWorld* world = player->GetWorld();
	if (world == NULL)
	{
		return;
	}

	FString contentDir = FPaths::GameContentDir();

	FPaths::NormalizeDirectoryName(contentDir);

	TArray<UMaterialInterface*>* materials = new TArray<UMaterialInterface*>();

	GetMaterialsInFolder(contentDir, "/Game", materials);

	std::wstring strToSaveIntoFile = L"Num,Material,\"Material FullName\",Texture,ColorR,ColorG,ColorB,Comment,\"Document Text\"\n";
	int csvRecordNum = 1;

	for (int32 i = 0; i < materials->Num(); i++)
	{
		UMaterialInterface* currentMaterial = (*materials)[i];

		FString strCurrentName = currentMaterial->GetName();
		FString strCurrentFullName = currentMaterial->GetFullName();

		FString strCurrentTexture;
		UTexture* currentTexture;
		if (currentMaterial->GetTextureParameterOverrideValue(FName("Diffuse"), currentTexture))
		{
			strCurrentTexture = currentTexture->GetName();
		}
		
		std::wstringstream itemStream;

		itemStream << ConvertToWString(FString::FromInt(csvRecordNum))
			<< L"," << ConvertToWString(strCurrentName)
			<< L"," << ConvertToWString(strCurrentFullName)
			<< L"," << ConvertToWString(strCurrentTexture)
			<< L",";

		FLinearColor currentColor = FLinearColor();
		if (currentMaterial->GetVectorParameterValue(FName("Color"), currentColor))
		{
			itemStream << ConvertToWString(FString::SanitizeFloat(currentColor.R))
				<< L"," << ConvertToWString(FString::SanitizeFloat(currentColor.G))
				<< L"," << ConvertToWString(FString::SanitizeFloat(currentColor.B));
		}

		itemStream << L",,"; // Comment,""Document Placeholder"",""Document Text"" are empty

		itemStream << std::endl;

		strToSaveIntoFile += itemStream.str();

		csvRecordNum++;
	}

	if (FileName.IsEmpty())
	{
		FileName = "Materials";
	}

	CreateFile(strToSaveIntoFile, player->GetName(), FileName, ".csv", AddDateTime);
}
