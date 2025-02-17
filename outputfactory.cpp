//  2006-2008 (c) Viva64.com Team
//  2008-2020 (c) OOO "Program Verification Systems"
//  2020-2022 (c) PVS-Studio LLC

#include <stdexcept>
#include "outputfactory.h"
#include "Formats/xmloutput.h"
#include "Formats/csvoutput.h"
#include "Formats/errorfileoutput.h"
#include "Formats/errorfileverboseoutput.h"
#include "Formats/tasklistoutput.h"
#include "Formats/tasklistverboseoutput.h"
#include "Formats/htmloutput.h"
#include "Formats/simplehtml.h"
#include "Formats/teamcityoutput.h"
#include "Formats/sarifoutput.h"
#include "Formats/sarifvscodeoutput.h"
#include "Formats/misracomplianceoutput.h"
#include "Formats/jsonoutput.h"
#include "Formats/totalsoutput.h"

namespace PlogConverter
{

template <typename T>
static auto FactoryFunction()
{
  return [](const ProgramOptions &opt) { return std::make_unique<T>(opt); };
}

OutputFactory::OutputFactory()
  : m_formats {
      { "csv",                 FactoryFunction<CSVOutput>() },
      { "xml",                 FactoryFunction<XMLOutput>() },
      { "fullhtml",            FactoryFunction<HTMLOutput>() },
      { "errorfile",           FactoryFunction<ErrorFileOutput>() },
      { "errorfile-verbose",   FactoryFunction<ErrorFileVerboseOutput>() },
      { "tasklist",            FactoryFunction<TaskListOutput>() },
      { "tasklist-verbose",    FactoryFunction<TaskListVerboseOutput>() },
      { "html",                FactoryFunction<SimpleHTMLOutput>() },
      { "teamcity",            FactoryFunction<TeamCityOutput>() },
      { "sarif",               FactoryFunction<SarifOutput>() },
      { "sarif-vscode",        FactoryFunction<SarifVSCodeOutput>() },
      { "misra_compliance",    FactoryFunction<MisraComplianceOutput>() },
      { "json",                FactoryFunction<JsonOutput>() },
      { "totals",              FactoryFunction<TotalsOutput>() }
    }
{
}

std::unique_ptr<IOutput> OutputFactory::createOutput(const ProgramOptions &os, const std::string &format)
{
  auto it = m_formats.find(format);
  if (it == m_formats.end())
  {
    throw std::runtime_error("Incorrect format given");
  }

  return std::unique_ptr<IOutput>(it->second(os));
}

void OutputFactory::registerOutput(const std::string& format, AllocFunction f)
{
  if (!f)
  {
    throw std::invalid_argument("Factory function is nullptr");
  }

  auto &fn = m_formats[format];
  if (fn)
  {
    throw std::runtime_error("Format with such name already registered");
  }

  fn = std::move(f);
}

}

