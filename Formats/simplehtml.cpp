//  2006-2008 (c) Viva64.com Team
//  2008-2020 (c) OOO "Program Verification Systems"
//  2020-2022 (c) PVS-Studio LLC

#include <algorithm>
#include "simplehtml.h"

namespace PlogConverter
{

using namespace std;

SimpleHTMLOutput::SimpleHTMLOutput(const ProgramOptions &opt) : IOutput(opt, "html")
{

}

SimpleHTMLOutput::~SimpleHTMLOutput() = default;

const int SimpleHTMLOutput::DefaultCweColumnWidth = 6;  //%
const int SimpleHTMLOutput::DefaultSASTColumnWidth = 9;  //%
const int SimpleHTMLOutput::DefaultMessageColumnWidth = 65; //%

static char HtmlHead[] = R"(
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head>
  <title>Messages</title>
  <meta http-equiv="content-type" content="text/html; charset=utf-8" />
  <style type="text/css">
    td {
      padding: 0;
      text-align: left;
      vertical-align: top;
    }

    legend {
      color: blue;
      font: 1.2em bold Comic Sans MS Verdana;
      text-align: center;
    }
  </style>
</head>
<body>
  <table style="width: 100%; font: 12pt normal Century Gothic;">
)";

static char HtmlEnd[] = R"(
  </table>
</body>
</html>
)";

void SimpleHTMLOutput::PrintHtmlStart()
{
  m_ostream << HtmlHead << std::endl;
}

void SimpleHTMLOutput::PrintHtmlEnd()
{
  m_ostream << HtmlEnd << std::endl;
}

void SimpleHTMLOutput::PrintHeading(const std::string &text)
{
  m_ostream << R"(    <tr style='background: lightcyan;'>)" << endl;
  m_ostream << R"(      <td colspan='5' style='color: red; text-align: center; font-size: 1.2em;'>)" << text << R"(</td>)" << endl;
  m_ostream << R"(    </tr>)" << endl;
}

int SimpleHTMLOutput::GetMessageColumnWidth() const
{
  int width = DefaultMessageColumnWidth;

  bool showSAST = false;
  bool showCWE = false;

  DetectShowTags(showCWE, showSAST);

  if (showCWE)
    width -= GetCweColumnWidth();

  if (showSAST)
    width -= GetSASTColumnWidth();


  return width;
}

int SimpleHTMLOutput::GetCweColumnWidth() const
{
  return DefaultCweColumnWidth;
}

int SimpleHTMLOutput::GetSASTColumnWidth() const
{
  return DefaultSASTColumnWidth;
}

void SimpleHTMLOutput::PrintTableCaption()
{
#ifdef WIN32
  m_ostream << R"(    <caption style="font-weight: bold;background: #fff;color: #000;border: none !important;">MESSAGES</caption>)" << endl;
  m_ostream << R"(    <tr style="background: black; color: white;">)" << endl;
  m_ostream << R"(      <th style="width: 10%;">Project</th>)" << endl;
  m_ostream << R"(      <th style="width: 20%;">File</th>)" << endl;
  m_ostream << R"(      <th style="width: 5%;">Code</th>)" << endl;
  m_ostream << R"(      <th style="width: 45%;">Message</th>)" << endl;
  m_ostream << R"(      <th style="width: 20%;">Analyzed Source File(s)</th>)" << endl;
  m_ostream << R"(    </tr>)" << endl;
#else
  m_ostream << R"(    <caption style="font-weight: bold;background: #fff;color: #000;border: none !important;">MESSAGES</caption>)" << endl;
  m_ostream << R"(    <tr style="background: black; color: white;">)" << endl;
  m_ostream << R"(      <th style="width: 30%;">Location</th>)" << endl;
  m_ostream << R"(      <th style="width: 5%;">Code</th>)" << endl;

  bool showCwe = false;
  bool showSast = false;

  DetectShowTags(showCwe, showSast);

  if (showCwe) 
  {
    m_ostream << R"(      <th style="width: )" << GetCweColumnWidth() << R"(%;">CWE</th>)" << endl;
  }
  if (showSast) 
  {
    m_ostream << R"(      <th style="width: )" << GetSASTColumnWidth() << R"(%;">SAST</th>)" << endl;
  }

  m_ostream << R"(      <th style="width: 65%;">Message</th>)" << endl;
  m_ostream << R"(    </tr>)" << endl;
#endif
}

void SimpleHTMLOutput::PrintMessages(const std::vector<Warning> &messages, const std::string &caption)
{
  if (messages.empty())
  {
    return;
  }

  PrintHeading(caption);
  for (const auto &err : messages)
  {
#ifdef WIN32
    std::string fileUTF8 = err.GetFileUTF8();
    m_ostream << R"(    <tr>)" << endl;
    m_ostream << R"(      <td style='width: 10%;'></td>)" << endl;
    m_ostream << R"(      <td style='width: 20%;'><div title=")" << EscapeHtml(fileUTF8) << R"(">)" << FileBaseName(fileUTF8)
              << " (" << err.GetLine() << R"()</div></td>)" << endl;
    m_ostream << R"(      <td style='width: 5%;'><a target="_blank" href=')" << err.GetVivaUrl() << R"('>)"
              << err.code << R"(</a></td>)" << endl;
    m_ostream << R"(      <td style='width: 45%;'>)" << EscapeHtml(err.message) << R"(</td>)" << endl;
    m_ostream << R"(      <td style='width: 20%;'></td>)" << endl;
    m_ostream << R"(    </tr>)" << endl;
#else
    m_ostream << R"(    <tr>)" << endl;
    m_ostream << R"(      <td style='width: 30%;'><div title=")"
              << EscapeHtml(err.GetFile())
              << R"(">)"
              << FileBaseName(err.GetFile())
              << " (" << err.GetLine()
              << R"()</div></td>)"
              << endl;
    m_ostream << R"(      <td style='width: 5%;'><a target="_blank" href=')"
              << err.GetVivaUrl()
              << R"('>)"
              << err.code
              << R"(</a></td>)"
              << endl;

    bool showCwe = false;
    bool showSast = false;

    DetectShowTags(showCwe, showSast);

    if (showCwe)
    {
      if (err.HasCWE())
        m_ostream << R"(      <td style='width: )" << GetCweColumnWidth() << R"(%;'><a target="_blank" href=')"
        << err.GetCWEUrl() << R"('>)" << err.GetCWEString() << R"(</a></td>)" << endl;
      else
        m_ostream << R"(      <th style="width: )" << GetCweColumnWidth() << R"(%;"></th>)" << endl;
    }

    if (showSast)
    {
      if (err.HasSAST())
        m_ostream << R"(      <td style='width: )" << GetSASTColumnWidth() << R"(%;'>)"
         << err.sastId << R"(</td>)" << endl;
      else
        m_ostream << R"(      <th style="width: )" << GetSASTColumnWidth() << R"(%;"></th>)" << endl;
    }

    m_ostream << R"(      <td style='width: )" << GetMessageColumnWidth() << R"(%;'>)" << EscapeHtml(err.message) << R"(</td>)" << endl;
    m_ostream << R"(    </tr>)" << endl;
#endif
  }
}

void SimpleHTMLOutput::PrintTableBody()
{
  PrintMessages(m_info, "Fails/Info");
  PrintMessages(m_ga, "General Analysis (GA)");
  PrintMessages(m_op, "Micro-optimizations (OP)");
  PrintMessages(m_64, "64-bit errors (64)");
  PrintMessages(m_cs, "Customers Specific (CS)");
  PrintMessages(m_misra, "MISRA");
  PrintMessages(m_autosar, "AUTOSAR");
  PrintMessages(m_owasp, "OWASP");
}

void SimpleHTMLOutput::Start()
{
  PrintHtmlStart();
}

bool SimpleHTMLOutput::Write(const Warning& msg)
{
  if (msg.IsDocumentationLinkMessage())
  {
    return false;
  }

  AnalyzerType analyzerType = msg.GetType();
  if (analyzerType == AnalyzerType::General)
    m_ga.push_back(msg);
  else if (analyzerType == AnalyzerType::Optimization)
    m_op.push_back(msg);
  else if (analyzerType == AnalyzerType::Viva64)
    m_64.push_back(msg);
  else if (analyzerType == AnalyzerType::CustomerSpecific)
    m_cs.push_back(msg);
  else if (analyzerType == AnalyzerType::Misra)
    m_misra.push_back(msg);
  else if (analyzerType == AnalyzerType::Autosar)
    m_autosar.push_back(msg);
  else if (analyzerType == AnalyzerType::Owasp)
    m_owasp.push_back(msg);
  else
    m_info.push_back(msg);

  return true;
}

void SimpleHTMLOutput::Finish()
{
  if (m_ga.empty() && m_op.empty() && m_64.empty() && m_cs.empty() && m_misra.empty() && m_info.empty() && m_owasp.empty() && m_autosar.empty())
  {
    m_ostream << "No messages generated" << std::endl;
  }
  else
  {
    PrintTableCaption();
    PrintTableBody();
  }

  PrintHtmlEnd();
}

}