// Copyright 2016-2017 Pavel Shyrko. All Rights Reserved.
#include <vector>

#pragma once

#include "Engine.h"
#include "GKLabLibraryBPLibrary.generated.h"

struct CsvElement
{
	int Num;
	std::string Actor;
	std::string Mesh;
	int Element;
	std::string Comment;
	std::string Description;
};

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
	static void CreateSpecification();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create File of Elements", CompactNodeTitle = "Create File Elem", Keywords = "GKLab"), Category = "GKLab")
	static void CreateFileOfElements();

	static std::vector<CsvElement> Elements;

	static bool FindElement(std::string& actor, std::string& mesh, int element, CsvElement& result);
};
