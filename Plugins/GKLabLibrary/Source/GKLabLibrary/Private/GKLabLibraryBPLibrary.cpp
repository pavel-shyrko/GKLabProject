#include "GKLabLibrary.h"
#include "GKLabLibraryBPLibrary.h"
#include "FileManagerGeneric.h"
#include "CsvRow.h"

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

std::vector<CsvMaterial> UGKLabLibraryBPLibrary::ProjectMaterials;

std::vector<CsvElement> UGKLabLibraryBPLibrary::ProjectElements;

UGKLabLibraryBPLibrary::UGKLabLibraryBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ProjectElements.clear();

	FString gameConfigDir = FPaths::GameConfigDir();

	FString elementsFilePath = gameConfigDir + "/Elements.csv";

	std::ifstream elementsFile(TCHAR_TO_UTF8(*elementsFilePath));

	CSVRow elementCsvRow;
	while (elementsFile >> elementCsvRow)
	{
		CsvElement* element = new CsvElement();

		element->Num = is_number(elementCsvRow[0]) ? std::stoi(elementCsvRow[0]) : 0;
		element->Level = elementCsvRow[1];
		element->Actor = elementCsvRow[2];
		element->ActorFName = elementCsvRow[3];
		element->UniqueID = elementCsvRow[4];
		element->ActorFullName = elementCsvRow[5];
		element->Mesh = elementCsvRow[6];
		element->Element = is_number(elementCsvRow[7]) ? std::stoi(elementCsvRow[7]) : 0;
		element->Comment = elementCsvRow[8];
		element->DocumentPlaceholder = elementCsvRow[9];
		element->DocumentText = elementCsvRow[10];

		ProjectElements.push_back(*element);
	}

	ProjectMaterials.clear();

	FString materialsFilePath = gameConfigDir + "/Materials.csv";

	std::ifstream materialFile(TCHAR_TO_UTF8(*materialsFilePath));

	CSVRow materialCsvRow;
	while (materialFile >> materialCsvRow)
	{
		CsvMaterial* material = new CsvMaterial();

		material->Num = is_number(materialCsvRow[0]) ? std::stoi(materialCsvRow[0]) : 0;
		material->Material = materialCsvRow[1];
		material->MaterialPath = materialCsvRow[2];
		material->MaterialFullName = materialCsvRow[3];
		material->Texture = materialCsvRow[4];
		material->ColorRed = materialCsvRow[5];
		material->ColorGreen = materialCsvRow[6];
		material->ColorBlue = materialCsvRow[7];
		material->Comment = materialCsvRow[8];
		material->DocumentText = materialCsvRow[9];

		ProjectMaterials.push_back(*material);
	}
}

bool UGKLabLibraryBPLibrary::FindElement(std::string& actor, std::string& mesh, int element, CsvElement& result)
{
	std::vector<CsvElement>::iterator it = std::find_if(ProjectElements.begin(), ProjectElements.end(), [&](const CsvElement& o)
	{
		return (actor.empty() || o.Actor == actor) && (mesh.empty() || o.Mesh == mesh) && (o.Element == element);
	});

	if (it == ProjectElements.end())
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
				TArray<UMaterialInterface*> materials = StaticMeshComponent->GetMaterials();
				FString staticMesh = StaticMesh->GetName();

				FString strMesh = objName + " Mesh:\t" + staticMesh;
				//GEngine->AddOnScreenDebugMessage(-1, delayDebugMessage, FColor::Black, strMesh);
				strToSaveIntoFile += "\t" + strMesh + "\r\n";

				for (int32 j = 0; j < materials.Num(); j++)
				{
					UMaterialInstance* originalMaterial = (UMaterialInstance*)materials[j];
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
							description = FString(fObj.DocumentText.c_str());
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

void LoadMaterialsInFolder(
	FString Directory,
	FString Path,
	TArray<UMaterialInstance*>* materialInstances,
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
			materialInstances->Add(materialInstance);
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
	TArray<UMaterialInstance*>* materialInstances,
	TArray<UMaterialInterface*>* materials)
{
	if (FPaths::DirectoryExists(Directory))
	{
		LoadMaterialsInFolder(Directory, Path, materialInstances, materials);

		FString path = Directory + "/*.*";
		TArray<FString> subDirs;
		subDirs.Empty();
		FFileManagerGeneric::Get().FindFiles(subDirs, *path, false, true);
		for (int i = 0; i < subDirs.Num(); i++)
		{
			FString subDirectory = Directory + "/" + subDirs[i];
			FString subPath = Path + "/" + subDirs[i];
			GetMaterialsInFolder(subDirectory, subPath, materialInstances, materials);
		}
	}
}

void UGKLabLibraryBPLibrary::CreateFileOfMaterials(FString FileName, bool AddDateTime)
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

	FString contentDir = FPaths::GameContentDir();

	FPaths::NormalizeDirectoryName(contentDir);

	TArray<UMaterialInstance*>* materialInstances = new TArray<UMaterialInstance*>();
	TArray<UMaterialInterface*>* materials = new TArray<UMaterialInterface*>();

	GetMaterialsInFolder(contentDir, "/Game", materialInstances, materials);

	FString strToSaveIntoFile = "Num,Material,\"Material Path\",\"Material FullName\",Texture,ColorR,ColorG,ColorB,Comment,\"Document Text\"\r\n";
	int csvRecordNum = 1;

	for (int32 i = 0; i < materialInstances->Num(); i++)
	{
		UMaterialInstance* currentMaterialInstance = (*materialInstances)[i];

		FString strCurrentName = currentMaterialInstance->GetName();
		FString strCurrentPath = currentMaterialInstance->GetPathName();

		FString strCurrentTexture;
		UTexture* currentTexture;
		if (currentMaterialInstance->GetTextureParameterOverrideValue(FName("Diffuse"), currentTexture))
		{
			strCurrentTexture = currentTexture->GetName();
		}

		FLinearColor currentColor = FLinearColor();
		/*FString strCurrentColor;
		if (currentMaterial->GetVectorParameterValue(FName("Color"), currentColor))
		{
		strCurrentColor = """rgb(" + FString::FromInt((currentColor.R * 255))
		+ "," + FString::FromInt((currentColor.G * 255))
		+ "," + FString::FromInt((currentColor.B * 255))
		+ ")""";
		}*/

		strToSaveIntoFile += FString::FromInt(csvRecordNum)
			+ "," + strCurrentName
			+ "," + strCurrentPath
			+ "," + strCurrentTexture
			+ "," + FString::SanitizeFloat(currentColor.R)
			+ "," + FString::SanitizeFloat(currentColor.G)
			+ "," + FString::SanitizeFloat(currentColor.B)
			//+ "," + strCurrentColor
			+ ",\r\n"; // Comment,""Document Text"" are empty

		csvRecordNum++;
	}

	for (int32 i = 0; i < materials->Num(); i++)
	{
		UMaterialInterface* currentMaterial = (*materials)[i];

		FString strCurrentName = currentMaterial->GetName();
		FString strCurrentPath = currentMaterial->GetPathName();

		FString strCurrentTexture;
		UTexture* currentTexture;
		if (currentMaterial->GetTextureParameterOverrideValue(FName("Diffuse"), currentTexture))
		{
			strCurrentTexture = currentTexture->GetName();
		}

		FLinearColor currentColor = FLinearColor();		
		/*FString strCurrentColor;
		if (currentMaterial->GetVectorParameterValue(FName("Color"), currentColor))
		{
			strCurrentColor = """rgb(" + FString::FromInt((currentColor.R * 255))
				+ "," + FString::FromInt((currentColor.G * 255))
				+ "," + FString::FromInt((currentColor.B * 255))
				+ ")""";
		}*/

		strToSaveIntoFile += FString::FromInt(csvRecordNum)
			+ "," + strCurrentName
			+ "," + strCurrentPath
			+ "," + strCurrentTexture
			+ "," + FString::SanitizeFloat(currentColor.R)
			+ "," + FString::SanitizeFloat(currentColor.G)
			+ "," + FString::SanitizeFloat(currentColor.B)
			//+ "," + strCurrentColor
			+ ",,\r\n"; // Comment,""Document Placeholder"",""Document Text"" are empty

		csvRecordNum++;
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

//void GetMaterialsInFolder(
//	FString Directory,
//	FString Path,
//	TArray<UMaterialInstance*>* materialInstances,
//	TArray<UMaterialInterface*>* materials,
//	UObjectLibrary* lib)
//{
//	TArray<FString> output;
//	output.Empty();
//	if (FPaths::DirectoryExists(Directory))
//	{
//		FString path = Directory + "/*.*";
//		FFileManagerGeneric::Get().FindFiles(output, *path, false, true);
//
//		for (int i = 0; i < output.Num(); i++)
//		{
//			FString subDirectory = Directory + "/" + output[i];
//
//			FString subPath = Path + "/" + output[i];
//			lib->LoadAssetDataFromPath(subPath);
//			TArray<FAssetData> assetData;
//			lib->GetAssetDataList(assetData);
//
//			for (FAssetData asset : assetData) {
//				UObject* obj = asset.GetAsset();
//				UMaterialInstance* mi = Cast<UMaterialInstance>(obj);
//				if (mi) {
//					materialInstances->Add(mi);
//				}
//				else
//				{
//					UMaterialInterface* m = Cast<UMaterialInterface>(obj);
//					if (m) {
//						materials->Add(m);
//					}
//				}
//			}
//
//			GetMaterialsInFolder(subDirectory, subPath, materialInstances, materials, lib);
//		}
//	}
//}

//void UGKLabLibraryBPLibrary::CreateFileOfMaterials(FString FileName, bool AddDateTime)
//{
//	FString contentDir = FPaths::GameContentDir();
//
//	FPaths::NormalizeDirectoryName(contentDir);
//
//	TArray<UMaterialInstance*>* materialInstances = new TArray<UMaterialInstance*>();
//	TArray<UMaterialInterface*>* materials = new TArray<UMaterialInterface*>();
//
//	UObjectLibrary *lib = UObjectLibrary::CreateLibrary(UMaterialInstance::StaticClass(), false, true);
//
//	GetMaterialsInFolder(contentDir, "/Game", materialInstances, materials, lib);
//}

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
