#include "CsvElement.h"
#include "CsvMaterial.h"
#include <vector>

#pragma once

#include "Engine.h"
#include "GKLabLibraryBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UGKLabLibraryBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/*UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "GKLab"), Category = "GKLab")
	static float GKLabLibrarySampleFunction(float Param);*/
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create Specification File", CompactNodeTitle = "Create Spec", Keywords = "GKLab"), Category = "GKLab")
	static void CreateSpecification(FString FileName, bool AddDateTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create File of Elements", CompactNodeTitle = "Create File Elem", Keywords = "GKLab"), Category = "GKLab")
	static void CreateFileOfElements(FString FileName, bool AddDateTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create File of Materials", CompactNodeTitle = "Create File Materials", Keywords = "GKLab"), Category = "GKLab")
	static void CreateFileOfMaterials(FString FileName, bool AddDateTime);

	static std::vector<CsvMaterial> ProjectMaterials;

	static std::vector<CsvElement> ProjectElements;

	static bool FindElement(std::wstring& level, std::wstring& actor, std::wstring& mesh, int element, CsvElement& result);
	
	static bool UGKLabLibraryBPLibrary::FindMaterial(std::wstring& material, std::wstring& texture, CsvMaterial& result);
};
