Plog Converter
==============
[![Docs](	https://img.shields.io/readthedocs/pip.svg)](https://pvs-studio.com/en/docs/manual/0036/) ![Platforms](https://img.shields.io/badge/platform-linux%20|%20macos-green)

**Note**. This page is about the tool for Linux and macOS. The appropriate page about the tool for Windows [is available here](https://github.com/viva64/PlogConverter-MSBuild-VS).

To convert the analyzer bug report to different formats (xml, tasks and so on) you can use the Plog Converter.
It is applicable for cross-platform working scenarios (C++, Java) on all supported operating systems when checking Makefile, CMake, QMake, Ninja, WAF projects.

More detailed description is available on the [documentation page](https://pvs-studio.com/en/docs/manual/0036/), section "Plog Converter Utility".

Compilation
--------------

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Usage
-------------

An example of a command that will be suitable for most users for opening the report in QtCreator:


```
plog-converter -a GA:1,2 -t tasklist -o /path/to/project.tasks /path/to/project.log
```

///dvbfgfg