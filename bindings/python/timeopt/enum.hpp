// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_python_muscod_enum_hpp__
#define __timeopt_python_muscod_enum_hpp__

#include <boost/python.hpp>
#include <string>

namespace timeopt
{
  namespace python
  {
    
    void exposeEnumEndeffectorID(const std::string & enum_name);
    void exposeEnumPhaseType(const std::string & enum_name);
  } // namespace python
} // namespace timeopt


#endif // ifndef __timeopt_python_muscod_enum_hpp__