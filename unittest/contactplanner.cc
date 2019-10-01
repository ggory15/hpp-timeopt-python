// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <timeopt/interface/contactplanner.hpp>


BOOST_AUTO_TEST_SUITE ( BOOST_TEST_MODULE )
BOOST_AUTO_TEST_CASE ( test_YAML_save )
{
    std::cout << "test Contact planner" << std::endl;
    using namespace timeopt;
    using namespace Eigen;

    InitialState init_state;

    init_state.setCOM(Eigen::Vector3d(0.0, 0.15, -0.2));
    init_state.setInitialPose(true, Vector3d(0.086, 0.15,-0.92), Quaterniond(1, 0, 0, 0), RF);
    init_state.setInitialPose(true, Vector3d(-0.086, 0.15,-0.92), Quaterniond(1, 0, 0, 0), LF);
    init_state.setInitialPose(false, Vector3d(0.4, 0.3, 0.0), Quaterniond(1, 0, 0, 0), RH);
    init_state.setInitialPose(false, Vector3d(-0.4, 0.3, 0.0), Quaterniond(1, 0, 0, 0), LH);    
    init_state.save();
    
    ContactState contact_state(6);
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
    planner.setTimehorizon(9.5);
    planner.setRobotmass(60.0);
    planner.setFinalCOM(Eigen::Vector3d(0.2, 1.2, 0.4));
    
    planner.saveToFile();   
}
BOOST_AUTO_TEST_SUITE_END ()
