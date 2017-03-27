// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Boilerplate C++ definitions for a single module.
	This is automatically generated by UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/GKLabLibrary.h"
#include "GKLabLibrary.generated.dep.h"
PRAGMA_DISABLE_OPTIMIZATION
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCode1GKLabLibrary() {}
	void UGKLabLibraryBPLibrary::StaticRegisterNativesUGKLabLibraryBPLibrary()
	{
		FNativeFunctionRegistrar::RegisterFunction(UGKLabLibraryBPLibrary::StaticClass(), "CreateFileOfElements",(Native)&UGKLabLibraryBPLibrary::execCreateFileOfElements);
		FNativeFunctionRegistrar::RegisterFunction(UGKLabLibraryBPLibrary::StaticClass(), "CreateFileOfMaterials",(Native)&UGKLabLibraryBPLibrary::execCreateFileOfMaterials);
		FNativeFunctionRegistrar::RegisterFunction(UGKLabLibraryBPLibrary::StaticClass(), "CreateSpecification",(Native)&UGKLabLibraryBPLibrary::execCreateSpecification);
	}
	IMPLEMENT_CLASS(UGKLabLibraryBPLibrary, 3070590027);
#if USE_COMPILED_IN_NATIVES
// Cross Module References
	ENGINE_API class UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();

	GKLABLIBRARY_API class UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfElements();
	GKLABLIBRARY_API class UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfMaterials();
	GKLABLIBRARY_API class UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateSpecification();
	GKLABLIBRARY_API class UClass* Z_Construct_UClass_UGKLabLibraryBPLibrary_NoRegister();
	GKLABLIBRARY_API class UClass* Z_Construct_UClass_UGKLabLibraryBPLibrary();
	GKLABLIBRARY_API class UPackage* Z_Construct_UPackage__Script_GKLabLibrary();
	UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfElements()
	{
		struct GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms
		{
			FString FileName;
			bool AddDateTime;
		};
		UObject* Outer=Z_Construct_UClass_UGKLabLibraryBPLibrary();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("CreateFileOfElements"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04022401, 65535, sizeof(GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms));
			CPP_BOOL_PROPERTY_BITMASK_STRUCT(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms, bool);
			UProperty* NewProp_AddDateTime = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("AddDateTime"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms), 0x0010000000000080, CPP_BOOL_PROPERTY_BITMASK(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms), sizeof(bool), true);
			UProperty* NewProp_FileName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("FileName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(FileName, GKLabLibraryBPLibrary_eventCreateFileOfElements_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("CompactNodeTitle"), TEXT("Create File Elem"));
			MetaData->SetValue(ReturnFunction, TEXT("DisplayName"), TEXT("Create File of Elements"));
			MetaData->SetValue(ReturnFunction, TEXT("Keywords"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/GKLabLibraryBPLibrary.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfMaterials()
	{
		struct GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms
		{
			FString FileName;
			bool AddDateTime;
		};
		UObject* Outer=Z_Construct_UClass_UGKLabLibraryBPLibrary();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("CreateFileOfMaterials"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04022401, 65535, sizeof(GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms));
			CPP_BOOL_PROPERTY_BITMASK_STRUCT(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms, bool);
			UProperty* NewProp_AddDateTime = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("AddDateTime"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms), 0x0010000000000080, CPP_BOOL_PROPERTY_BITMASK(AddDateTime, GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms), sizeof(bool), true);
			UProperty* NewProp_FileName = new(EC_InternalUseOnlyConstructor, ReturnFunction, TEXT("FileName"), RF_Public|RF_Transient|RF_MarkAsNative) UStrProperty(CPP_PROPERTY_BASE(FileName, GKLabLibraryBPLibrary_eventCreateFileOfMaterials_Parms), 0x0010000000000080);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("CompactNodeTitle"), TEXT("Create File Materials"));
			MetaData->SetValue(ReturnFunction, TEXT("DisplayName"), TEXT("Create File of Materials"));
			MetaData->SetValue(ReturnFunction, TEXT("Keywords"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/GKLabLibraryBPLibrary.h"));
#endif
		}
		return ReturnFunction;
	}
	UFunction* Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateSpecification()
	{
		UObject* Outer=Z_Construct_UClass_UGKLabLibraryBPLibrary();
		static UFunction* ReturnFunction = NULL;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("CreateSpecification"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), NULL, 0x04022401, 65535);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("CompactNodeTitle"), TEXT("Create Spec"));
			MetaData->SetValue(ReturnFunction, TEXT("DisplayName"), TEXT("Create Specification File"));
			MetaData->SetValue(ReturnFunction, TEXT("Keywords"), TEXT("GKLab"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/GKLabLibraryBPLibrary.h"));
			MetaData->SetValue(ReturnFunction, TEXT("ToolTip"), TEXT("UFUNCTION(BlueprintCallable, meta = (DisplayName = \"Execute Sample function\", Keywords = \"GKLab\"), Category = \"GKLab\")\n       static float GKLabLibrarySampleFunction(float Param);"));
#endif
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UGKLabLibraryBPLibrary_NoRegister()
	{
		return UGKLabLibraryBPLibrary::StaticClass();
	}
	UClass* Z_Construct_UClass_UGKLabLibraryBPLibrary()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_UBlueprintFunctionLibrary();
			Z_Construct_UPackage__Script_GKLabLibrary();
			OuterClass = UGKLabLibraryBPLibrary::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20000080;

				OuterClass->LinkChild(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfElements());
				OuterClass->LinkChild(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfMaterials());
				OuterClass->LinkChild(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateSpecification());

				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfElements(), "CreateFileOfElements"); // 191881417
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateFileOfMaterials(), "CreateFileOfMaterials"); // 1288744197
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_UGKLabLibraryBPLibrary_CreateSpecification(), "CreateSpecification"); // 355435393
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("GKLabLibraryBPLibrary.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/GKLabLibraryBPLibrary.h"));
				MetaData->SetValue(OuterClass, TEXT("ToolTip"), TEXT("*      Function library class.\n*      Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.\n*\n*      When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.\n*      BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.\n*      BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.\n*      DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.\n*                              Its lets you name the node using characters not allowed in C++ function names.\n*      CompactNodeTitle - the word(s) that appear on the node.\n*      Keywords -      the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.\n*                              Good example is \"Print String\" node which you can find also by using keyword \"log\".\n*      Category -      the category your node will be under in the Blueprint drop-down menu.\n*\n*      For more info on custom blueprint nodes visit documentation:\n*      https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UGKLabLibraryBPLibrary(Z_Construct_UClass_UGKLabLibraryBPLibrary, &UGKLabLibraryBPLibrary::StaticClass, TEXT("UGKLabLibraryBPLibrary"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UGKLabLibraryBPLibrary);
	UPackage* Z_Construct_UPackage__Script_GKLabLibrary()
	{
		static UPackage* ReturnPackage = NULL;
		if (!ReturnPackage)
		{
			ReturnPackage = CastChecked<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), NULL, FName(TEXT("/Script/GKLabLibrary")), false, false));
			ReturnPackage->SetPackageFlags(PKG_CompiledIn | 0x00000000);
			FGuid Guid;
			Guid.A = 0x96A24DDC;
			Guid.B = 0xB65D73E8;
			Guid.C = 0x00000000;
			Guid.D = 0x00000000;
			ReturnPackage->SetGuid(Guid);

		}
		return ReturnPackage;
	}
#endif

PRAGMA_ENABLE_DEPRECATION_WARNINGS
