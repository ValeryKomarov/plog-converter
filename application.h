//  2006-2008 (c) Viva64.com Team
//  2008-2020 (c) OOO "Program Verification Systems"
//  2020-2022 (c) PVS-Studio LLC

#ifndef APPLICATION_H
#define APPLICATION_H
#include "configs.h"
#include <string>
#include <vector>
#include "iworker.h"
#include "outputfactory.h"

namespace PlogConverter
{

Analyzer ParseEnabledAnalyzer(const std::string &str);
void ParseEnabledAnalyzers(std::string str, std::vector<Analyzer>& analyzers);

class Application
{
public:
  static const std::string AppName_Default;
  static const std::string AppName_Win;
  static const std::string AboutPVSStudio;

  static const char CmdAnalyzerFlagName_Short;
  static const std::string CmdAnalyzerFlagName_Full;

  int Exec(int argc, const char **argv);
  void AddWorker(std::unique_ptr<IWorker> worker);

  OutputFactory outputFactory;

private:
  void SetCmdOptions(int argc, const char** argv);
  void SetConfigOptions(const std::string& path);

  ProgramOptions m_options;
  std::vector<std::unique_ptr<IWorker>> m_workers;
};

}

#endif // APPLICATION_H
