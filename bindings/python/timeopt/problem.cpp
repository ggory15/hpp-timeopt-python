// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include "timeopt/bindings/python/timeopt/expose-problem.hpp"
#include "timeopt/bindings/python/timeopt/problem.hpp"


  namespace timeopt{
        namespace python
        {
            void exposeProblemInfo()
            {
                ProblemPythonVisitor<timeopt::ProblemInfo>::expose("problem");
            }
        }
    }

