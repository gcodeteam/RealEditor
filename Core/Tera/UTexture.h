#pragma once
#include "UObject.h"
#include <Utils/TextureTravaller.h>

#include <array>

namespace osg
{
  class Image;
}

class UTexture : public UObject {
public:
  DECL_UOBJ(UTexture, UObject);

  UPROP(bool, SRGB, true);
  UPROP(TextureCompressionSettings, CompressionSettings, TC_Default);

  bool RegisterProperty(FPropertyTag* property) override;

  void Serialize(FStream& s) override;

protected:
  FByteBulkData SourceArt;
};

class UTexture2D : public UTexture {
public:

  static EPixelFormat GetEffectivePixelFormat(const EPixelFormat format, bool bSRGB);

  DECL_UOBJ(UTexture2D, UTexture);

  ~UTexture2D() override;

  bool RegisterProperty(FPropertyTag* property) override;

  UPROP(EPixelFormat, Format, PF_Unknown);
  UPROP(int32, SizeX, 0);
  UPROP(int32, SizeY, 0);
  UPROP(TextureAddress, AddressX, TA_Wrap);
  UPROP(TextureAddress, AddressY, TA_Wrap);
  UPROP(TextureGroup, LODGroup, TEXTUREGROUP_World);
  UPROP(int32, MipTailBaseIdx, 0);
  UPROP(int32, FirstResourceMemMip, 0);
  UPROP(bool, bNoTiling, false);
  UPROP(bool, NeverStream, false);
  UPROP(FName*, TextureFileCacheName, nullptr);

  bool RenderTo(osg::Image* target, int32 maxWidth = 0, int32 maxHeight = 0);

  friend bool TextureTravaller::Visit(UTexture2D* texture);

  void Serialize(FStream& s) override;

  unsigned int Hash() const;

  // Disable texture caching, pull max mipmap, delete smaller mips
  // Needed for cross-region mods
  void DisableCaching();

protected:
  void PostLoad() override;
  void DeleteStorage();

public:
  std::vector<FTexture2DMipMap*> Mips;
protected:
  FString SourceFilePath;
  std::vector<FTexture2DMipMap*> CachedMips;
  std::vector<FTexture2DMipMap*> CachedAtiMips;
  FByteBulkData CachedFlashMip;
  std::vector<FTexture2DMipMap*> CachedEtcMips;
  FGuid TextureFileCacheGuid;
  int32 MaxCachedResolution = 0;
};

class UTerrainWeightMapTexture : public UTexture2D {
public:
  DECL_UOBJ(UTerrainWeightMapTexture, UTexture2D);
};

class UTextureCube : public UTexture {
public:
  DECL_UOBJ(UTextureCube, UTexture);
  UPROP(UTexture2D*, FacePosX, nullptr);
  UPROP(UTexture2D*, FaceNegX, nullptr);
  UPROP(UTexture2D*, FacePosY, nullptr);
  UPROP(UTexture2D*, FaceNegY, nullptr);
  UPROP(UTexture2D*, FacePosZ, nullptr);
  UPROP(UTexture2D*, FaceNegZ, nullptr);

  bool RegisterProperty(FPropertyTag* property) override;

  // targets order: FacePosX, FaceNegX, FacePosY, FaceNegY, FacePosZ, FaceNegZ
  bool RenderTo(osg::Image* targets);

  std::array<UTexture2D*, 6> GetFaces()
  {
    return { FacePosX, FaceNegX, FacePosY, FaceNegY, FacePosZ, FaceNegZ };
  }

protected:
  void PostLoad() override;
};