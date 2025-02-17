﻿//  2006-2008 (c) Viva64.com Team
//  2008-2020 (c) OOO "Program Verification Systems"
//  2020-2022 (c) PVS-Studio LLC

#include <filesystem>

#include "sourceroottransformer.h"
#include "utils.h"

namespace PlogConverter
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;

  namespace
  {

  const std::string &GetSourceTreeRootMarker()
  {
    static auto sourceTreeRootMarker { "|?|"s };
    return sourceTreeRootMarker;
  }

  const std::string &GetPathSeparator()
  {
#ifdef _WIN32
    static auto sep { "\\"s };
#else
    static auto sep { "/"s  };
#endif

    return sep;
  }

  void ReplacePathPrefix(std::string &toReplace, std::string_view replacer)
  {
    static std::string_view exception { "pvs-studio.com/en/docs/warnings/"sv };

    if (toReplace.substr(0, exception.length()) == exception)
    {
      return;
    }

    std::error_code rc;
    auto proximated = std::filesystem::proximate(toReplace, replacer, rc); //-V821

    if (!rc)
    {
      toReplace = GetSourceTreeRootMarker() + GetPathSeparator() + proximated.string();
    }
  }

  void ReplaceRelativeRoot(std::string& str, const std::string& root)
  {
    Replace(str, GetSourceTreeRootMarker(), root);
  }

  void ReplaceAbsolutePrefix(std::string& str, const std::string& root)
  {
      ReplacePathPrefix(str, root);
  }
  }

  SourceRootTransformer::SourceRootTransformer(IOutput* output, const ProgramOptions& options)
    : ITransform(output)
    , m_options(options)
  {

  }

  SourceRootTransformer::~SourceRootTransformer() = default;

  Warning SourceRootTransformer::Transform(Warning message) const
  {
    if (!m_options.projectRoot.empty())
    {
      for (auto &position : message.positions)
      {
        if (m_options.pathTransformationMode == PathTransformationMode::ToAbsolute)
        {
          ReplaceRelativeRoot(position.file, m_options.projectRoot);
        }
        else if (m_options.pathTransformationMode == PathTransformationMode::ToRelative)
        {
          ReplaceAbsolutePrefix(position.file, m_options.projectRoot);
        }
      }
    }
    
    return message;
  }
}
