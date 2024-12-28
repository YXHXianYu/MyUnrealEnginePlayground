// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomMeshComponent.h"

#include "Operations/EmbedSurfacePath.h"

IMPLEMENT_VERTEX_FACTORY_TYPE(FCustomMeshVertexFactory, "/CustomMeshVS/LocalVertexFactory.ush", EVertexFactoryFlags::None)

void FCustomMeshVertexFactory::InitRHI(FRHICommandListBase& RHICmdList) {
    FLocalVertexFactory::InitRHI(RHICmdList);

    check(HasValidFeatureLevel());

    FVertexDeclarationElementList Elements;
    FVertexDeclarationElementList PosOnlyElements;

    if (Data.PositionComponent.VertexBuffer != nullptr) {
        Elements.Add(AccessStreamComponent(Data.PositionComponent, 0));
        PosOnlyElements.Add(AccessStreamComponent(Data.PositionComponent, 0, EVertexInputStreamType::PositionOnly));
    }

    InitDeclaration(PosOnlyElements, EVertexInputStreamType::PositionOnly);

    if (Data.TextureCoordinates.Num()) {
        constexpr int32 BaseTextureCoordinateAttribute = 4;
        for (int32 CoordinateIndex = 0; CoordinateIndex < Data.TextureCoordinates.Num(); CoordinateIndex++) {
            Elements.Add(AccessStreamComponent(
                Data.TextureCoordinates[CoordinateIndex],
                BaseTextureCoordinateAttribute + CoordinateIndex
            ));
        }

        for (int32 CoordinateIndex = Data.TextureCoordinates.Num(); CoordinateIndex < MAX_STATIC_TEXCOORDS / 2; CoordinateIndex++) {
            Elements.Add(AccessStreamComponent(
                Data.TextureCoordinates[Data.TextureCoordinates.Num() - 1],
                BaseTextureCoordinateAttribute + CoordinateIndex
            ));
        }
    }

    check(Streams.Num() > 0);

    InitDeclaration(Elements);

    check(IsValidRef(GetDeclaration()));
}

void FCustomMeshVertexFactory::Init(const FStaticMeshVertexBuffers* VertexBuffers) {
    ENQUEUE_RENDER_COMMAND(StaticMeshVertexBuffersLegacyInit) ( [this, VertexBuffers](FRHICommandListImmediate& RHICmdList) {
        FLocalVertexFactory::FDataType Data;
        VertexBuffers->PositionVertexBuffer.BindPositionVertexBuffer(this, Data);
        VertexBuffers->StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(this, Data);
        this->SetData(Data);

        if (!this->IsInitialized()) {
            this->InitResource();
        } else {
            this->UpdateRHI();
        }
    });
}

void FCustomMeshVertexFactory::ReleaseResource() {
    FLocalVertexFactory::ReleaseResource();
}

FPrimitiveSceneProxy* UMyCustomMeshComponent::CreateSceneProxy() {
    // return Super::CreateSceneProxy();
    if (!SceneProxy) {
        // return new FCustomMesh
    }
};

