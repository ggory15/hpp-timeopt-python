// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_problem_hpp__
#define __timeopt_problem_hpp__

#include <Eigen/StdVector>
#include <vector>
#include <string>
#include <stdexcept>

#include "timeopt/fwd.hpp"
#include "timeopt/phase.hpp"
#include "timeopt/interface/contactplanner.hpp"

#include <momentumopt/dynopt/DynamicsOptimizer.hpp>

  namespace timeopt
  {
    struct ProblemInfo
    {
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      typedef std::vector< PhaseInfo,Eigen::aligned_allocator<PhaseInfo> > PhaseInfoVector;
      typedef std::vector<std::string> StringVector;
      typedef size_t Index;
      typedef Eigen::Vector3d Vector3d;
      typedef Eigen::VectorXd VectorXd;
      typedef Eigen::Matrix3d Matrix3d;
      typedef Eigen::Quaterniond Quaterniond;

      ProblemInfo();
      ProblemInfo(const Index size);

      void setConfigurationFile(const std::string & cfg_path) const throw (std::invalid_argument);
      void setTimeoptSolver(const std::string & file);
      void solve();
      
      Index numPhases() const { return phases_.size(); }
      Vector3d getInitialCOM() const {return init_com_;}
      Vector3d getFinalCOM() const {return final_com_;}

      void setInitialCOM(const Vector3d& com){
        init_com_ = com;
        init_state_->setCOM(com);
      }
      void setInitialPose(const bool active, const Vector3d& pos, const Matrix3d& rot, EndeffectorID ee){
        Quaterniond quat(rot);
        init_state_->setInitialPose(active, pos, quat, ee);
      }
      void setPhase(const size_t index, const PhaseInfo & ph){
        phases_[index] = ph;
        contact_state_->SetPhase(index, ph);
      }
      void setMass(const double& mass){
        mass_ = mass;
        planner_->setRobotmass(mass);
      }
      void setFinalCOM(const Vector3d& com){
        final_com_ = com;
        planner_->setFinalCOM(com);
      }
      void setViapoint(const double& time, const Vector3d& point){
        planner_->setViapoint(time, point);
      }
      void resize(const Index index){
        phases_.resize(index);
        contact_state_->resize(index);
      }
      double getTimetrajectory(const int & cnt){
        return time_traj_(cnt); 
      }
      Vector3d getCOMtrajectory(const int & cnt){
        return dyn_optimizer_.dynamicsSequence().dynamicsState(cnt).centerOfMass();
      }
      Vector3d getLMOMtrajectory(const int & cnt){
        return dyn_optimizer_.dynamicsSequence().dynamicsState(cnt).linearMomentum();
      }
      Vector3d getAMOMtrajectory(const int & cnt){
        return dyn_optimizer_.dynamicsSequence().dynamicsState(cnt).angularMomentum();
      }
      Vector3d getContactForce(const int & id, const int & cnt){
        return dyn_optimizer_.dynamicsSequence().dynamicsState(cnt).endeffectorForce(id);
      }
      int getNumSize() {
        return std::floor(planner_setting_.get(momentumopt::PlannerDoubleParam_TimeHorizon) / planner_setting_.get(momentumopt::PlannerDoubleParam_TimeStep));
      }
      double getMass(){
        return mass_;
      }
      private:  
        PhaseInfoVector phases_;
        Vector3d init_com_, final_com_;
        double mass_;
        VectorXd time_traj_;
        double timehorizon_;

      private: 
        momentumopt::PlannerSetting planner_setting_;
        momentumopt::DynamicsState dynamic_state_;
        momentumopt::DynamicsSequence ref_sequence_;
        momentumopt::DynamicsOptimizer dyn_optimizer_;  

      private:
        InitialState* init_state_;
        timeopt::ContactState* contact_state_;
        ContactPlanner* planner_;       
             
    }; // struct ProblemInfo
  }


#endif // ifndef __timeopt_problem_hpp__
