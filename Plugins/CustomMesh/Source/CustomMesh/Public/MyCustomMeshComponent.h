// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MaterialDomain.h"
#include "MeshMaterialShader.h"
#include "Components/StaticMeshComponent.h"
#include "MyCustomMeshComponent.generated.h"

struct FCustomMeshVertexFactory: public FLocalVertexFactory {
    DECLARE_VERTEX_FACTORY_TYPE(FCustomMeshVertexFactory);
    
public:
    FCustomMeshVertexFactory(ERHIFeatureLevel::Type InFeatureLevel)
        : FLocalVertexFactory(InFeatureLevel, "FCustomMeshVertexFactory")
    {
        // bSupportsManualVertexFetch = false;
    }

    static bool ShouldCompilePermutation(const FVertexFactoryShaderPermutationParameters& Parameters) {
        const bool b1 = (Parameters.MaterialParameters.MaterialDomain == MD_Surface
            && Parameters.MaterialParameters.ShadingModels == MSM_Unlit);
        const bool b2 = Parameters.MaterialParameters.bIsDefaultMaterial;
        return b1 || b2;
    }

    static void ModifyCompilationEnvironment(const FVertexFactoryShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) {
        const bool ContainsManualVertexFetch = OutEnvironment.GetDefinitionsAsCommentedCode().Contains("MANUAL_VERTEX_FETCH");
        if (!ContainsManualVertexFetch) {
            OutEnvironment.SetDefine(TEXT("MANUAL_VERTEX_FETCH"), TEXT("0"));
        }

        OutEnvironment.SetDefine(TEXT("MY_CUSTOM_MESH"), TEXT("1"));
    }

	virtual void InitRHI(FRHICommandListBase& RHICmdList) override;

    void Init(const FStaticMeshVertexBuffers* VertexBuffers);

    virtual void ReleaseResource() override; 
};

/**
 * 
 */
UCLASS(HideCategories=(Object, LOD), meta=(BlueprintSpawnableComponent), ClassGroup=Rendering)
class CUSTOMMESH_API UMyCustomMeshComponent : public UStaticMeshComponent {
	GENERATED_BODY()
    
public:
    virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	
};
