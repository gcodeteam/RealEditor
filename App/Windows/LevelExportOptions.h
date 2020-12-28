#pragma once
#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/filepicker.h>

#include <filesystem>

#include <Utils/AConfiguration.h>
#include <Utils/TextureProcessor.h>

struct LevelExportContext {

  static LevelExportContext LoadFromAppConfig();
  static void SaveToAppConfig(const LevelExportContext& ctx);

  const char* DataDirName = "S1Game";
  const char* StaticMeshStorage = "StaticMeshes";
  const char* SkeletalMeshStorage = "SkeletalMeshes";
  const char* SpeedTreeStorage = "SpeedTrees";
  const char* TextureStorage = "Textures";
  const char* MaterialStorage = "Materials";
  const char* MaterialMapStorage = "MaterialMap";
  const char* MasterMaterialStorage = "MasterMaterials.txt";
  const char* MaterialsListStorage = "MaterialsList.txt";
  const char* TerrainStorage = "Terrains";

  inline std::filesystem::path GetStaticMeshDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / StaticMeshStorage / DataDirName;
  }

  inline std::filesystem::path GetSkeletalMeshDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / SkeletalMeshStorage / DataDirName;
  }

  inline std::filesystem::path GetSpeedTreeDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / SpeedTreeStorage / DataDirName;
  }

  inline std::filesystem::path GetTextureDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / TextureStorage / DataDirName;
  }

  // Contains material parameters dumps
  inline std::filesystem::path GetMaterialDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / MaterialStorage;
  }

  // Per-actor material map
  inline std::filesystem::path GetMaterialMapDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / MaterialMapStorage;
  }

  // A list of master materials
  inline std::filesystem::path GetMasterMaterialsPath() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / MasterMaterialStorage;
  }

  // A list of all materials for RE Helper plugin
  inline std::filesystem::path GetMaterialsListPath() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / MaterialsListStorage;
  }

  inline std::filesystem::path GetTerrainDir() const
  {
    return std::filesystem::path(Config.RootDir.WString()) / TerrainStorage;
  }

  inline TextureProcessor::TCFormat GetTextureFormat() const
  {
    switch (Config.TextureFormat)
    {
    case 0:
      return TextureProcessor::TCFormat::TGA;
    case 1:
      return TextureProcessor::TCFormat::PNG;
    }
    return TextureProcessor::TCFormat::DDS;
  }

  FMapExportConfig Config;

  struct ComponentTransform {
    FVector PrePivot;
    FVector Translation;
    FVector Scale3D = FVector(1, 1, 1);
    FRotator Rotation;
    float Scale = 1.;

    inline bool operator==(const ComponentTransform& a)
    {
      auto ta = std::tie(a.PrePivot.X, a.PrePivot.Y, a.PrePivot.Z, a.Translation.X, a.Translation.Y, a.Translation.Z, a.Scale3D.X, a.Scale3D.Y, a.Scale3D.Z, a.Rotation.Pitch, a.Rotation.Yaw, a.Rotation.Roll, a.Scale);
      auto tb = std::tie(PrePivot.X, PrePivot.Y, PrePivot.Z, Translation.X, Translation.Y, Translation.Z, Scale3D.X, Scale3D.Y, Scale3D.Z, Rotation.Pitch, Rotation.Yaw, Rotation.Roll, Scale);
      return ta == tb;
    }
  };

  std::vector<std::string> Errors;
  std::vector<UObject*> UsedMaterials;
  std::map<std::string, UObject*> MasterMaterials;
  std::map<std::string, std::vector<ComponentTransform>> FbxComponentTransformMap;
  std::vector<std::string> ComplexCollisions;
  std::map<std::string, std::vector<std::string>> MLODs;
  int CurrentProgress = 0;
  int StaticMeshActorsCount = 0;
  int SkeletalMeshActorsCount = 0;
  int SpeedTreeActorsCount = 0;
  int PointLightActorsCount = 0;
  int SpotLightActorsCount = 0;
  int DirectionalLightActorsCount = 0;
  int SkyLightActorsCount = 0;
  int HeightFogCount = 0;
  int UntypedActorsCount = 0;
};

class LevelExportOptionsWindow : public wxDialog {
public:
  LevelExportOptionsWindow(wxWindow* parent, const LevelExportContext& ctx = LevelExportContext());
  ~LevelExportOptionsWindow();

  void SetExportContext(const LevelExportContext& ctx);
  LevelExportContext GetExportContext() const;

protected:
  void FillActorsTable(const FMapExportConfig& cfg);
  int32 GetActorsTableMask() const;
  void OnDirChanged(wxFileDirPickerEvent& event);
  void OnDefaultsClicked(wxCommandEvent& event);
  void OnExportClicked(wxCommandEvent& event);
  void OnCancelClicked(wxCommandEvent& event);
  void OnAllClicked(wxCommandEvent&);
  void OnNoneClicked(wxCommandEvent&);
  void OnActorTableValueChanged(wxDataViewEvent&);
  void OnFirstIdle(wxIdleEvent&);

protected:
  wxDirPickerCtrl* PathPicker = nullptr;
  wxDataViewCtrl* ActorTable = nullptr;
  wxButton* TurnOnAllButton = nullptr;
  wxButton* TurnOffAllButton = nullptr;

  wxCheckBox* OverrideFiles = nullptr;
  wxCheckBox* SplitT3d = nullptr;
  wxCheckBox* Materials = nullptr;
  wxTextCtrl* PointLightMultiplier = nullptr;
  wxTextCtrl* SpotLightMultiplier = nullptr;
  wxCheckBox* LightInvSqrt = nullptr;
  wxCheckBox* DynamicShadows = nullptr;
  wxCheckBox* ResampleTerrain = nullptr;
  wxCheckBox* SplitTerrainWeightMaps = nullptr;
  wxCheckBox* Textures = nullptr;
  wxChoice* TextureFormatSelector = nullptr;
  wxCheckBox* ExportLods = nullptr;
  wxCheckBox* ExportMLods = nullptr;
  wxCheckBox* ConvexCollisions = nullptr;
  wxCheckBox* ExportLightmapUVs = nullptr;
  wxCheckBox* IgnoreHidden = nullptr;
  wxButton* DefaultsButton = nullptr;
  wxButton* ExportButton = nullptr;
  wxButton* CancelButton = nullptr;

  float PointLightMultiplierValue = 1.;
  float SpotLightMultiplierValue = 1.;
  int32 DelayedTextureFormat = 0;
};