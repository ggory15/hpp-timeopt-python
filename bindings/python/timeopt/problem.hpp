// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_python_problem_hpp__
#define __timeopt_python_problem_hpp__

#include <boost/python.hpp>
#include <string>
#include <eigenpy/eigenpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "timeopt/bindings/python/container/visitor.hpp"

#include "timeopt/problem.hpp"

namespace timeopt{
  namespace python
  {    
    namespace bp = boost::python;
    
    template<typename Problem>
    struct ProblemPythonVisitor
    : public boost::python::def_visitor< ProblemPythonVisitor<Problem> >
    {
      typedef typename Problem::Index Index;
      typedef typename Problem::PhaseInfoVector PhaseInfoVector;

      template<class PyClass>     

      void visit(PyClass& cl) const
      {
        cl
        .def(bp::init<>("Defulat Constructor"))
        .def(bp::init<Index>(bp::args("size"),"Default constructor with given size."))

        .def("numPhases",&Problem::numPhases,"Returns the number of phases contained in *this")
        .def("getInitialCOM",&Problem::getInitialCOM,"Returns initial COM")
        .def("getFinalCOM",&Problem::getFinalCOM,"Returns final COM")

        .def("setInitialCOM",&Problem::setInitialCOM, (bp::arg("COM")),"Set Initial COM (Vector3d).")
        .def("setInitialPose",&Problem::setInitialPose, (bp::args("active", "pos", "rot", "EE_id")),"Set Initial Pose.")
        .def("setPhase",&Problem::setPhase, (bp::arg("index"), bp::arg("phase")),"Set ith phase.")
        .def("setMass",&Problem::setMass, (bp::args("mass")),"Set robot mass.")
        .def("setFinalCOM",&Problem::setFinalCOM, (bp::arg("COM")),"Set Final COM (Vector3d).")
        .def("setViapoint",&Problem::setViapoint, (bp::args("time", "viapoint(Vector3d)")), "Set Viapoint (time, Vector3d).")  

        .def("resize",&Problem::resize, (bp::args("size")),"Resize phases size.")
        .def("getTrajectorySize", &Problem::getNumSize, "get Trajectory length")
        .def("getTime", &Problem::getTimetrajectory, bp::args("cnt"), "Get resultant time trajectory" )
        .def("getCOM", &Problem::getCOMtrajectory, bp::args("cnt"), "Get resultant com trajectory" )
        .def("getLMOM", &Problem::getLMOMtrajectory, bp::args("cnt"), "Get resultant linear momentum trajectory" )

        .def("getAMOM", &Problem::getAMOMtrajectory, bp::args("cnt"), "Get resultant angular momentum trajectory" )
        .def("getContactForce", &Problem::getContactForce, bp::args("id"), bp::args("cnt"), "get Contact Force during CD")
        .def("setConfigurationFile", &Problem::setConfigurationFile, bp::args("string"), "Set Configuration File for time optimization solver" )
        .def("setTimeoptSolver", &Problem::setTimeoptSolver, bp::args("string"), "Set Time optimization Problem" )
        .def("solve", &Problem::solve, "Solve Time optimization Problem" )

        .def("getMass", &Problem::getMass, "Get Robot Mass")
        ;
      }
   
      static void expose(const std::string & class_name)
      {
        std::string doc = "Problem info.";
        bp::class_<Problem>(class_name.c_str(),
                          doc.c_str(),
                          bp::no_init)
        .def(ProblemPythonVisitor<Problem>());
        ;

        VectorPythonVisitor<PhaseInfoVector>::expose("PhaseInfoVector");
      }

    };
  }
}


#endif // ifndef __hpp_timeopt_python_phase_hpp__
