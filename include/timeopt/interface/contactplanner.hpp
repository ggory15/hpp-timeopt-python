// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_interface_dynamicplanner_hpp__
#define __timeopt_interface_dynamicplanner_hpp__

#include "timeopt/fwd.hpp"
#include "timeopt/phase.hpp"

#include <momentumopt/setting/Definitions.hpp>
#include <momentumopt/dynopt/DynamicsOptimizer.hpp>

#include "yaml-cpp/yaml.h"
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

  namespace timeopt
  {
    class InitialState
    {
      public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef Eigen::Matrix<double, 8, 1> Vector8d;

      public:
        InitialState();
        ~InitialState(){};
        
        inline void setCOM(const Eigen::Vector3d& com) {
          ini_com_ = com;
        }
        inline void setAMOM(const Eigen::Vector3d& amom) {
          amom_ = amom;
        }
        inline void setLMOM(const Eigen::Vector3d& lmom) {
          lmom_ = lmom;
        }
        const YAML::Node& get(){
          return init_cfg;
        };

        void setEEForceRatio(const Eigen::Vector3d& ratio, EndeffectorID ee);
        void setInitialPose(const bool active, const Eigen::Vector3d& pos, const Eigen::Quaterniond& quat, EndeffectorID ee);
        void save();
        void saveToFile();
        
      private:
        Eigen::Vector3d ini_com_, amom_, lmom_;
        Eigen::Vector3d eef_frc_rf_;
        Eigen::Vector3d eef_frc_lf_;
        Eigen::Vector3d eef_frc_lh_;
        Eigen::Vector3d eef_frc_rh_;
        Vector8d eef_rf, eef_lf, eef_rh, eef_lh;
        EndeffectorID ee_id;
        YAML::Node init_cfg;
    };
    class ContactState
    {
      public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef std::vector< PhaseInfo,Eigen::aligned_allocator<PhaseInfo> > PhaseInfoVector;
        typedef Eigen::Matrix<double, 10, 1> FootPrint;
        typedef Eigen::Matrix<unsigned int, 4, 1> Vector4i;
        typedef std::vector<FootPrint,Eigen::aligned_allocator<FootPrint>> FootPrintVector;

      public:
        ContactState()
        : phases_()
        {};
        ContactState(const size_t size)
        : phases_(size)
        {};
      
        ~ContactState(){};
        void SetPhase(const size_t index, const PhaseInfo & ph){
          phases_[index] = ph;
        };
        const YAML::Node& get(){
          return contact_cfg;
        };
        void resize(const size_t index){
          phases_.resize(index);
        }

        void save();
        void saveToFile();
        double getTimeHorizon(){
          return phases_[phases_.size()-1].end_time;
        };

      private:
        PhaseInfoVector phases_;
        Vector4i num_contacts_;
        YAML::Node contact_cfg;
        Eigen::Vector3d time_horizon;
    };


    class ContactPlanner 
    {
      public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
        typedef Eigen::Matrix<double, 4, 1> Viapoint;
        typedef std::vector<Viapoint,Eigen::aligned_allocator<Viapoint>> ViapointVector;

      public:
        ContactPlanner();
        ~ContactPlanner(){};
        
        void initialize(const std::string cfg_path, const InitialState& init_state, const ContactState& contact_state);

        inline void setTimehorizon(const double& time){
          time_horizon_ = time;
        }
        inline void setRobotmass(const double& mass) {
          robot_mass_ = mass;
        }
        inline void setFinalCOM(const Eigen::Vector3d & end_com){
          end_com_ = end_com;
        }
        inline void setViapoint(const double& time, const Eigen::Vector3d & via){
          Viapoint v;
          v << time, via(0), via(1), via(2);     
          viapoints_.push_back(v);
        }
        void saveToFile();

      private:
        double robot_mass_, time_horizon_;
        Eigen::Vector3d com_displacement_, end_com_;
        unsigned int num_com_viapoints_;
        ViapointVector viapoints_;

        std::string file_location_;
        
        momentumopt::PlannerSetting planner_setting_;
        InitialState init_state_;
        ContactState contact_state_;
    };
  }

#endif // ifndef __timeopt_interface_dynamicplanner_hpp__