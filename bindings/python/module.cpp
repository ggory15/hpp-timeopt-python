// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include <eigenpy/eigenpy.hpp>
#include <eigenpy/geometry.hpp>

#include "timeopt/bindings/python/timeopt/expose-phase.hpp"
#include "timeopt/bindings/python/timeopt/expose-problem.hpp"

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/to_python_converter.hpp>

namespace bp = boost::python;
using namespace timeopt::python;

BOOST_PYTHON_MODULE(libtimeopt_pywrap)
{
  eigenpy::enableEigenPy();
  eigenpy::exposeAngleAxis();
  eigenpy::exposeQuaternion();

  exposePhase();
  exposeProblem();
}
