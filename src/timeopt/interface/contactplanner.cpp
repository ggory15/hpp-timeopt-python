// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include "timeopt/interface/contactplanner.hpp"

namespace timeopt
{
  InitialState::InitialState()
        : ini_com_(Eigen::Vector3d(.0, .0, .0))
        , amom_(Eigen::Vector3d(.0, .0, .0))
        , lmom_(Eigen::Vector3d(.0, .0, .0))
        , eef_frc_rf_(Eigen::Vector3d(.0, .0, .5))
        , eef_frc_lf_(Eigen::Vector3d(.0, .0, .5))
        , eef_frc_rh_(Eigen::Vector3d(.0, .0, .0))
        , eef_frc_lh_(Eigen::Vector3d(.0, .0, .0))
    {
          eef_rf.setZero();
          eef_lf.setZero();
          eef_rh.setZero();
          eef_lh.setZero();
    }
  void InitialState::setEEForceRatio(const Eigen::Vector3d& ratio, EndeffectorID ee){
    switch (ee){
      case RF:
        eef_frc_rf_ = ratio;
        break;
      case LF:
        eef_frc_lf_ = ratio;
        break;
      case RH:
        eef_frc_rh_ = ratio;
        break;
      case LH:
        eef_frc_lh_ = ratio;
        break;
      case EE_Undefined:
        std::cout << "Warn::Wrong End-effector ID for ForceRatio" << std:: endl;
        break;
    }
  }
  void InitialState::setInitialPose(const bool active, const Eigen::Vector3d& pos, const Eigen::Quaterniond& quat, EndeffectorID ee){
    switch (ee){
      case RF:
        eef_rf(0) = active;
        eef_rf.segment(1, 3) = pos;
        eef_rf.segment(4, 4) << quat.w(), quat.x(), quat.y(), quat.z();
        break;
      case LF:
        eef_lf(0) = active;
        eef_lf.segment(1, 3) = pos;
        eef_lf.segment(4, 4) << quat.w(), quat.x(), quat.y(), quat.z();
        break;
      case RH:
        eef_rh(0) = active;
        eef_rh.segment(1, 3) = pos;
        eef_rh.segment(4, 4) << quat.w(), quat.x(), quat.y(), quat.z();
        break;
      case LH:
        eef_lh(0) = active;
        eef_lh.segment(1, 3) = pos;
        eef_lh.segment(4, 4) << quat.w(), quat.x(), quat.y(), quat.z();
        break;
      case EE_Undefined:
        std::cout << "Warn::Wrong End-effector ID for ForceRatio" << std:: endl;
        break;
    }
  }
  void InitialState::save(){
    init_cfg.SetStyle(YAML::EmitterStyle::Block);
    init_cfg["initial_robot_configuration"]["com"] = ini_com_;
    init_cfg["initial_robot_configuration"]["com"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["amom"] = amom_;
    init_cfg["initial_robot_configuration"]["amom"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["lmom"] = lmom_;
    init_cfg["initial_robot_configuration"]["lmom"].SetStyle(YAML::EmitterStyle::Flow);

    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_rf"] = eef_frc_rf_;
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_rf"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_lf"] = eef_frc_lf_;
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_lf"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_rh"] = eef_frc_rh_;
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_rh"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_lh"] = eef_frc_lh_;
    init_cfg["initial_robot_configuration"]["eef_ctrl"]["eef_frc_lh"].SetStyle(YAML::EmitterStyle::Flow);

    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_rf"] = eef_rf;
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_rf"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_lf"] = eef_lf;
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_lf"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_rh"] = eef_rh;
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_rh"].SetStyle(YAML::EmitterStyle::Flow);
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_lh"] = eef_lh;
    init_cfg["initial_robot_configuration"]["eef_pose"]["eef_lh"].SetStyle(YAML::EmitterStyle::Flow);
  }

  void InitialState::saveToFile(){
    InitialState::save();
    std::ofstream file_out("init_state.yaml");
    file_out << init_cfg;
  }

  void ContactState::save(){
    const size_t num_phases = phases_.size();
    FootPrintVector FootPrints_RF, FootPrints_LF, FootPrints_RH, FootPrints_LH;

    for (size_t i =0; i < num_phases; ++i){
      switch (phases_[i].ee_id) {
        case RF:
          FootPrints_RF.push_back(phases_[i].toVector());
        break;
        case LF:
          FootPrints_LF.push_back(phases_[i].toVector());
        break;
        case RH:
          FootPrints_RH.push_back(phases_[i].toVector());
        break;
        case LH:
          FootPrints_LH.push_back(phases_[i].toVector());
        break;
        case EE_Undefined:
          FootPrints_RF.push_back(phases_[i].toVector());
        break;
      }
    }
    num_contacts_(0) = FootPrints_RF.size();
    num_contacts_(1) = FootPrints_LF.size();
    num_contacts_(2) = FootPrints_RH.size();
    num_contacts_(3) = FootPrints_LH.size();
    
    contact_cfg.SetStyle(YAML::EmitterStyle::Block);
    contact_cfg["contact_plan"]["num_contacts"] = num_contacts_;
    contact_cfg["contact_plan"]["num_contacts"].SetStyle(YAML::EmitterStyle::Flow);
    
    for (size_t i=0; i<num_contacts_(0); i++){
      contact_cfg["contact_plan"]["effcnt_rf"]["cnt"+std::to_string(i)]= FootPrints_RF[i];
      contact_cfg["contact_plan"]["effcnt_rf"]["cnt"+std::to_string(i)].SetStyle(YAML::EmitterStyle::Flow);
    }
    for (size_t i=0; i<num_contacts_(1); i++){
      contact_cfg["contact_plan"]["effcnt_lf"]["cnt"+std::to_string(i)]= FootPrints_LF[i];
      contact_cfg["contact_plan"]["effcnt_lf"]["cnt"+std::to_string(i)].SetStyle(YAML::EmitterStyle::Flow);
    }
    if (num_contacts_(2) == 0){
        contact_cfg["contact_plan"]["effcnt_rh"] = std::string();
    } 
    else{
      for (size_t i=0; i<num_contacts_(2); i++){
        contact_cfg["contact_plan"]["effcnt_rh"]["cnt"+std::to_string(i)]= FootPrints_RH[i];
        contact_cfg["contact_plan"]["effcnt_rh"]["cnt"+std::to_string(i)].SetStyle(YAML::EmitterStyle::Flow);
      }
    }
    if (num_contacts_(3) == 0){
        contact_cfg["contact_plan"]["effcnt_lh"] = std::string();
    } 
    else{
      for (size_t i=0; i<num_contacts_(3); i++){
        contact_cfg["contact_plan"]["effcnt_lh"]["cnt"+std::to_string(i)]= FootPrints_LH[i];
        contact_cfg["contact_plan"]["effcnt_lh"]["cnt"+std::to_string(i)].SetStyle(YAML::EmitterStyle::Flow);
      }
    }
  }

  void ContactState::saveToFile(){
    ContactState::save();
    std::ofstream file_out("contact.yaml");
    file_out << contact_cfg;
  }

  ContactPlanner::ContactPlanner()
        : time_horizon_(0.0)
        , robot_mass_(0.0)
        , com_displacement_(Eigen::Vector3d(.0, .0, .0))
        {}
  void ContactPlanner::initialize(const std::string cfg_path, const InitialState& init_state, const ContactState& contact_state){
    planner_setting_.initialize(cfg_path);
    file_location_ = cfg_path;
    init_state_ = init_state;
    contact_state_ = contact_state;
  }

  void ContactPlanner::saveToFile(){
    YAML::Node cfg_pars = YAML::LoadFile(planner_setting_.get(momentumopt::PlannerStringParam_ConfigFile));
    YAML::Node init_cfg = init_state_.get();
    YAML::Node contact_cfg = contact_state_.get();

    cfg_pars["planner_variables"]["robot_mass"] = robot_mass_;
    cfg_pars["planner_variables"]["time_horizon"] = time_horizon_;

    
    Eigen::Vector3d init_com;
    YAML::Node planner_vars = init_cfg["initial_robot_configuration"];
    momentumopt::readParameter(planner_vars, "com", init_com);
    com_displacement_ = end_com_ - init_com;
    cfg_pars["planner_variables"]["com_displacement"] = com_displacement_;
    cfg_pars["planner_variables"]["com_displacement"].SetStyle(YAML::EmitterStyle::Flow);

    cfg_pars["planner_variables"]["num_com_viapoints"] = viapoints_.size();
    
    for (size_t i=0; i<viapoints_.size(); i++){
      cfg_pars["planner_variables"]["com_viapoints"]["via"+std::to_string(i)] = viapoints_[i];
      cfg_pars["planner_variables"]["com_viapoints"]["via"+std::to_string(i)].SetStyle(YAML::EmitterStyle::Flow);
    }
    std::ofstream file_out(file_location_.substr(0, file_location_.size()-5)+"_final.yaml");
    file_out << init_cfg << endl << endl << contact_cfg << endl << endl << cfg_pars;
  }
}
