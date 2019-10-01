// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include "timeopt/bindings/python/timeopt/expose-problem.hpp"
#include "timeopt/bindings/python/timeopt/enum.hpp"

#include <boost/python/enum.hpp>

namespace timeopt
{
  namespace python
  {
    namespace bp = boost::python;
    
    using namespace timeopt;
    
    void exposeEnumEndeffectorID()
    {
      bp::enum_<EndeffectorID>("EndeffectorID")
      .value("RF",RF)
      .value("LF",LF)
      .value("RH",RH)
      .value("LH",LH)
      .value("EE_Undefined",EE_Undefined)
      ;
    }
    
    void exposeEnumPhaseType()
    {
      bp::enum_<PhaseType>("PhaseType")
      .value("SS",SS)
      .value("DS",DS)
      .value("TS",TS)
      ;
    }
    
    void exposeTimeoptEnums()
    {
      exposeEnumEndeffectorID();
      exposeEnumPhaseType();
    }
  }
}