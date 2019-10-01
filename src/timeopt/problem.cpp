// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include "timeopt/problem.hpp"
#include <momentumopt/cntopt/ContactPlanFromFile.hpp>

#include <fstream>
#include <iomanip>

namespace timeopt
{   
  ProblemInfo::ProblemInfo()
  : phases_() // pre-allocation
  , init_com_(Vector3d::Ones()*1000)
  , final_com_(Vector3d::Ones()*1000)
  , mass_(0.0)
  {
    init_state_ = new InitialState();
    contact_state_ = new ContactState();
    planner_ = new ContactPlanner();
  }
  
  ProblemInfo::ProblemInfo(const Index size)
  : phases_(size) // pre-allocation
  , init_com_(Vector3d::Ones()*1000)
  , final_com_(Vector3d::Ones()*1000)
  , mass_(0.0)
  {
    init_state_ = new InitialState();
    contact_state_ = new ContactState(size);
    planner_ = new ContactPlanner();
  }
  
  void ProblemInfo::setConfigurationFile(const std::string & file_location) const throw (std::invalid_argument)
  {
    if (init_com_.norm() > 1000){
      const std::string exception_message("Initial COM does not seem to be a valid value.");
    }
    if (final_com_.norm() > 1000){
      const std::string exception_message("Final COM does not seem to be a valid value.");
    } 
    if (mass_ < 0.001){
      const std::string exception_message("Robot's Mass does not seem to be a valid value.");
    } 
    
    init_state_->save();
    contact_state_->save();   
    planner_->initialize(file_location, *init_state_, *contact_state_);
    planner_->setTimehorizon(contact_state_->getTimeHorizon());
    planner_->saveToFile();
  }
  void ProblemInfo::setTimeoptSolver(const std::string & file){
    std::string cfg_file;
    cfg_file = file.substr(0, file.size()-5)+"_final.yaml";
    
    planner_setting_.initialize(cfg_file);
    dynamic_state_.fillInitialRobotState(cfg_file);
    ref_sequence_.resize(planner_setting_.get(momentumopt::PlannerIntParam_NumTimesteps));
  }
  void ProblemInfo::solve(){
    momentumopt::ContactPlanFromFile contact_plan;
    contact_plan.initialize(planner_setting_);
    contact_plan.optimize(dynamic_state_);

    dyn_optimizer_.initialize(planner_setting_, dynamic_state_, &contact_plan);
    dyn_optimizer_.optimize(ref_sequence_);

    time_traj_.resize(getNumSize());
      int size = getNumSize();
      time_traj_(0) = dyn_optimizer_.dynamicsSequence().dynamicsState(0).time();
      for (int i=1; i<size; i++)
        time_traj_(i) = dyn_optimizer_.dynamicsSequence().dynamicsState(i).time() + time_traj_(i-1);
  }
  
  
}
