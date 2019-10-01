// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <timeopt/interface/contactplanner.hpp>
#include <momentumopt/dynopt/DynamicsOptimizer.hpp>
#include <momentumopt/cntopt/ContactPlanFromFile.hpp>

BOOST_AUTO_TEST_SUITE ( BOOST_TEST_MODULE )
BOOST_AUTO_TEST_CASE ( test_Time_opt )
{
    std::cout << "test Time Optimization" << std::endl;
    using namespace Eigen;
    using namespace momentumopt;
    
    std::cout << "Get Contact Information" << std::endl;
    using namespace timeopt;
    using namespace Eigen;

    InitialState init_state;

    init_state.setCOM(Eigen::Vector3d(0.0, 0.15, -0.2));
    init_state.setInitialPose(true, Vector3d(0.086, 0.15,-0.92), Quaterniond(1, 0, 0, 0), RF);
    init_state.setInitialPose(true, Vector3d(-0.086, 0.15,-0.92), Quaterniond(1, 0, 0, 0), LF);
    init_state.setInitialPose(false, Vector3d(0.4, 0.3, 0.0), Quaterniond(1, 0, 0, 0), RH);
    init_state.setInitialPose(false, Vector3d(-0.4, 0.3, 0.0), Quaterniond(1, 0, 0, 0), LH);    
    init_state.save();
    
    timeopt::ContactState contact_state(6);
    contact_state.SetPhase(0, PhaseInfo(RF, 0.0, 1.0, Vector3d(0.086, 0.15,-0.92), Matrix3d::Identity()));
    contact_state.SetPhase(1, PhaseInfo(RF, 2.0, 4.5, Vector3d(0.500, 0.45, -0.76), Matrix3d::Identity()));
    contact_state.SetPhase(2, PhaseInfo(RF, 6.0, 9.9, Vector3d(0.450, 0.98, -0.27), Matrix3d::Identity()));

    contact_state.SetPhase(3, PhaseInfo(LF, 0.0, 2.5, Vector3d(-0.086, 0.15, -0.92), Matrix3d::Identity()));
    contact_state.SetPhase(4, PhaseInfo(LF, 4.0, 6.5, Vector3d(-0.080, 0.70, -0.52), Matrix3d::Identity()));
    contact_state.SetPhase(5, PhaseInfo(LF, 8.5, 9.9, Vector3d(-0.080, 1.25, -0.25), Matrix3d::Identity()));
    contact_state.save();

    ContactPlanner planner;
    std::string planner_path;
    planner_path = std::string(CONFIG_DIR)+"/"+"cfg_momSc_demo01.yaml";
    
    planner.initialize(planner_path, init_state, contact_state);
    planner.setTimehorizon(contact_state.getTimeHorizon());
    planner.setRobotmass(60.0);
    planner.setFinalCOM(Eigen::Vector3d(0.2, 1.2, 0.4));
    
    planner.saveToFile();   
    std::cout << "Done" << std::endl;
    // Until this, finished planner setting 
     

    std::cout << "Do Time Optimization" << std::endl;
    std::string cfg_file;
    cfg_file = planner_path.substr(0, planner_path.size()-5)+"_final.yaml";
    PlannerSetting planner_setting;
    planner_setting.initialize(cfg_file);

    // define robot initial state
    DynamicsState ini_state;
    ini_state.fillInitialRobotState(cfg_file);

    // define reference dynamic sequence
    momentumopt::DynamicsSequence ref_sequence;
    ref_sequence.resize(planner_setting.get(PlannerIntParam_NumTimesteps));

    // define contact plan
    ContactPlanFromFile contact_plan;
    contact_plan.initialize(planner_setting);
    contact_plan.optimize(ini_state);

    // optimize motion
    DynamicsOptimizer dyn_optimizer;
    dyn_optimizer.initialize(planner_setting, ini_state, &contact_plan);
    dyn_optimizer.optimize(ref_sequence);

    std::cout << "Done" << std::endl;
    
}
BOOST_AUTO_TEST_SUITE_END ()
