/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#ifndef cmQtAutoGenerators_h
#define cmQtAutoGenerators_h

#include <cmConfigure.h> // IWYU pragma: keep
#include <cmFilePathChecksum.h>
#include <cmsys/RegularExpression.hxx>

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class cmMakefile;

class cmQtAutoGenerators
{
public:
  cmQtAutoGenerators();
  bool Run(const std::string& targetDirectory, const std::string& config);

private:
  // - Configuration
  bool ReadAutogenInfoFile(cmMakefile* makefile,
                           const std::string& targetDirectory,
                           const std::string& config);

  bool MocEnabled() const { return !this->MocExecutable.empty(); }
  bool UicEnabled() const { return !this->UicExecutable.empty(); }
  bool RccEnabled() const { return !this->RccExecutable.empty(); }

  // - Settings file
  void SettingsFileRead(cmMakefile* makefile,
                        const std::string& targetDirectory);
  bool SettingsFileWrite(const std::string& targetDirectory);

  bool GenerateAllAny() const
  {
    return (this->GenerateAllMoc || this->GenerateAllRcc ||
            this->GenerateAllUic);
  }

  // - Init and run
  void Init(cmMakefile* makefile);
  bool RunAutogen();

  // - Content analysis
  bool MocRequired(const std::string& text,
                   std::string* macroName = CM_NULLPTR);
  bool MocSkip(const std::string& absFilename) const;
  bool UicSkip(const std::string& absFilename) const;

  bool ParseSourceFile(
    const std::string& absFilename,
    std::map<std::string, std::string>& mocsIncluded,
    std::map<std::string, std::vector<std::string> >& includedUis,
    bool relaxed);

  void SearchHeadersForSourceFile(const std::string& absFilename,
                                  std::set<std::string>& mocHeaderFiles,
                                  std::set<std::string>& uicHeaderFiles) const;

  void ParseHeaders(
    const std::set<std::string>& mocHeaderFiles,
    const std::set<std::string>& uicHeaderFiles,
    const std::map<std::string, std::string>& mocsIncluded,
    std::map<std::string, std::string>& mocsNotIncluded,
    std::map<std::string, std::vector<std::string> >& includedUis);

  void ParseContentForUic(
    const std::string& fileName, const std::string& contentsString,
    std::map<std::string, std::vector<std::string> >& includedUis);

  bool ParseContentForMoc(const std::string& absFilename,
                          const std::string& contentsString,
                          std::map<std::string, std::string>& mocsIncluded,
                          bool relaxed);

  // - Moc file generation
  bool MocGenerateAll(
    const std::map<std::string, std::string>& mocsIncluded,
    const std::map<std::string, std::string>& mocsNotIncluded);
  bool MocGenerateFile(const std::string& sourceFile,
                       const std::string& mocFileName,
                       const std::string& subDirPrefix);

  // - Uic file generation
  bool UicGenerateAll(
    const std::map<std::string, std::vector<std::string> >& includedUis);
  bool UicGenerateFile(const std::string& realName,
                       const std::string& uiInputFile,
                       const std::string& uiOutputFile);

  // - Rcc file generation
  bool RccGenerateAll();
  bool RccGenerateFile(const std::string& qrcInputFile,
                       const std::string& qrcOutputFile, bool unique_n);

  // - Logging
  void LogErrorNameCollision(
    const std::string& message,
    const std::multimap<std::string, std::string>& collisions);
  void LogBold(const std::string& message);
  void LogInfo(const std::string& message);
  void LogWarning(const std::string& message);
  void LogError(const std::string& message);
  void LogCommand(const std::vector<std::string>& command);

  // - Utility
  bool NameCollisionTest(
    const std::map<std::string, std::string>& genFiles,
    std::multimap<std::string, std::string>& collisions) const;
  std::string ChecksumedPath(const std::string& sourceFile,
                             const char* basePrefix,
                             const char* baseSuffix) const;
  bool MakeParentDirectory(const std::string& filename);

  bool FindHeader(std::string& header, const std::string& testBasePath) const;
  bool FindHeaderGlobal(std::string& header,
                        const std::string& testBasePath) const;
  std::string FindMocHeader(const std::string& basePath,
                            const std::string& baseName,
                            const std::string& subDir) const;
  bool FindInIncludeDirectories(std::string& file_n,
                                const std::string& searchString) const;

  // - Target names
  std::string OriginTargetName;
  std::string AutogenTargetName;
  // - Directories
  std::string ProjectSourceDir;
  std::string ProjectBinaryDir;
  std::string CurrentSourceDir;
  std::string CurrentBinaryDir;
  std::string AutogenBuildSubDir;
  // - Qt environment
  std::string QtMajorVersion;
  std::string MocExecutable;
  std::string UicExecutable;
  std::string RccExecutable;
  // - File lists
  std::vector<std::string> Sources;
  std::vector<std::string> Headers;
  // - Settings
  std::string SettingsStringMoc;
  std::string SettingsStringUic;
  std::string SettingsStringRcc;
  // - Moc
  std::string MocCppFilenameRel;
  std::string MocCppFilenameAbs;
  std::vector<std::string> MocSkipList;
  std::vector<std::string> MocIncludePaths;
  std::vector<std::string> MocIncludes;
  std::vector<std::string> MocDefinitions;
  std::vector<std::string> MocOptions;
  // - Uic
  std::vector<std::string> UicSkipList;
  std::vector<std::string> UicTargetOptions;
  std::map<std::string, std::string> UicOptions;
  // - Rcc
  std::vector<std::string> RccSources;
  std::map<std::string, std::string> RccOptions;
  std::map<std::string, std::vector<std::string> > RccInputs;
  // - Utility
  cmFilePathChecksum fpathCheckSum;
  std::vector<std::string> HeaderExtensions;
  typedef std::pair<std::string, cmsys::RegularExpression> MacroFilter;
  MacroFilter MacroFilters[2];
  cmsys::RegularExpression RegExpMocInclude;
  cmsys::RegularExpression RegExpUicInclude;
  // - Flags
  bool IncludeProjectDirsBefore;
  bool Verbose;
  bool ColorOutput;
  bool RunMocFailed;
  bool RunUicFailed;
  bool RunRccFailed;
  bool GenerateAllMoc;
  bool GenerateAllUic;
  bool GenerateAllRcc;
  bool MocRelaxedMode;
};

#endif
