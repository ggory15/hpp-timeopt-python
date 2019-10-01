// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __hpp_timeopt_python_phase_hpp__
#define __hpp_timeopt_python_phase_hpp__

#include <boost/python.hpp>
#include <string>
#include <eigenpy/eigenpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "timeopt/phase.hpp"

namespace timeopt{
  namespace python
  {    
    namespace bp = boost::python;
    
    template<typename Phase>
    struct PhasePythonVisitor
    : public boost::python::def_visitor< PhasePythonVisitor<Phase> >
    {
      typedef std::vector<std::string> std_vec;
      typedef Eigen::Matrix<double,3,Eigen::Dynamic> Matrix3x;

      template<class PyClass>     

      void visit(PyClass& cl) const
      {
        cl
        .def(bp::init<>("Defulat Constructor"))
        .def(bp::init<Phase>(bp::args("other"),"Copy constructor."))
        .def(bp::init<timeopt::EndeffectorID, double, double, Eigen::Vector3d, Eigen::Matrix3d>(bp::args("Endeffector", "start time", "end time", "position", "rotation"), "Other constructor"))

        .def_readwrite("ee_id",&Phase::ee_id,"Endeffector ID")
        .def_readwrite("start_time",&Phase::start_time,"start_time of the phase.")
        .def_readwrite("end_time",&Phase::end_time,"end_time of the phase.")
        .def_readwrite("pos",&Phase::pos,"Foot Position")
        .def_readwrite("rot",&Phase::rot,"Foot Rotation.")
                
        .def(bp::self == bp::self)
        .def(bp::self != bp::self)
        
        .def("assign",&assign,bp::args("other"),"Copy other into *this.")  
        ;
      }
   
      static void expose(const std::string & class_name)
      {
        std::string doc = "Phase info.";
        bp::class_<Phase>(class_name.c_str(),
                          doc.c_str(),
                          bp::no_init)
        .def(PhasePythonVisitor<Phase>());
        ;
      }
      protected:
        static void assign(Phase & self, const Phase & other) { self = other; }
    };
  }
}


#endif // ifndef __hpp_timeopt_python_phase_hpp__
