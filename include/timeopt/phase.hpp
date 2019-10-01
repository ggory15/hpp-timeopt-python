// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_phase_hpp__
#define __timeopt_phase_hpp__

#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <vector>

#include "timeopt/fwd.hpp"

  namespace timeopt
  {
    struct PhaseInfo
    {
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      typedef size_t Index;
      typedef Eigen::DenseIndex DenseIndex;
      typedef double Scalar;
      typedef Eigen::MatrixXd MatrixX;
      typedef Eigen::VectorXd VectorX;
      typedef Eigen::Vector3d Vector3d;
      typedef Eigen::Matrix3d Matrix3d;
      typedef std::vector< VectorX,Eigen::aligned_allocator<VectorX> > VectorXVector;
      typedef Eigen::Matrix<double, 10, 1> FootPrint;

      /// \brief Default constructor
      PhaseInfo();

      /// \brief Copy constructor
      PhaseInfo(const PhaseInfo & other);

      /// \brief constructor
      PhaseInfo(const EndeffectorID & ee, const double & stime, const double & etime, const Vector3d& pos, const Matrix3d& rot);
      
      /// \brief Copy operator
      PhaseInfo & operator=(const PhaseInfo & other)
      {
        ee_id = other.ee_id;
        start_time = other.start_time;
        end_time = other.end_time;
        pos = other.pos;
        rot = other.rot; 
        
        return *this;
      }
      
      
      /// \brief Comparison operator
      bool operator==(const PhaseInfo & other) const
      {
        return ee_id == other.ee_id
        && start_time == other.start_time
        && end_time == other.end_time
        && pos == other.pos
        && rot == other.rot
        ;
      }
      
      bool operator!=(const PhaseInfo & other) const
      { return !(*this == other); }
      
      FootPrint toVector(){
        FootPrint print;
        print(0) = start_time;
        print(1) = end_time;
        print.segment(2, 3) = pos;
        Eigen::Quaterniond quat(rot);
        print.segment(5, 4) << quat.w(), quat.x(), quat.y(), quat.z();
        print(9) = 1;

        return print; 
      }

      /* Bounds on the duration */
      double start_time, end_time;

      /* Position & rot*/ 
      Vector3d pos;
      Matrix3d rot; 
                  
      /* End_effector id */
      EndeffectorID ee_id;
      
      /* Integrator */
      PhaseType phase_type;
      
      
    }; // struct PhaseInfo
  }

#endif // ifndef __timeopt_phase_hpp__
