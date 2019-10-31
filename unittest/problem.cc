// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <iomanip>
#include <yaml-cpp/yaml.h>
#include <timeopt/problem.hpp>

BOOST_AUTO_TEST_SUITE ( BOOST_TEST_MODULE )
BOOST_AUTO_TEST_CASE ( test_Time_opt )
{
    std::cout << "test Problem" << std::endl;
    using namespace Eigen;
    using namespace momentumopt;
    
    std::cout << "Get Contact Information" << std::endl;
    using namespace timeopt;
    
    ProblemInfo tp(6);
    tp.setInitialCOM(Eigen::Vector3d(0.0, 0.15, -0.2));
    tp.setInitialPose(true, Vector3d(0.086, 0.15,-0.92), Matrix3d::Identity(), RF);
    tp.setInitialPose(true, Vector3d(-0.086, 0.15,-0.92), Matrix3d::Identity(), LF);
    tp.setInitialPose(false, Vector3d(0.4, 0.3, 0.0), Matrix3d::Identity(), RH);
    tp.setInitialPose(false, Vector3d(-0.4, 0.3, 0.0), Matrix3d::Identity(), LH);    
    tp.setMass(60.0);
    tp.setFinalCOM(Eigen::Vector3d(0.2, 1.2, 0.4));

    tp.setPhase(0, PhaseInfo(RF, 0.0, 1.0, Vector3d(0.086, 0.15,-0.92), Matrix3d::Identity()));
    tp.setPhase(1, PhaseInfo(RF, 2.0, 4.5, Vector3d(0.500, 0.45, -0.76), Matrix3d::Identity()));
    tp.setPhase(2, PhaseInfo(RF, 6.0, 9.9, Vector3d(0.450, 0.98, -0.27), Matrix3d::Identity()));
    tp.setPhase(3, PhaseInfo(LF, 0.0, 2.5, Vector3d(-0.086, 0.15, -0.92), Matrix3d::Identity()));
    tp.setPhase(4, PhaseInfo(LF, 4.0, 6.5, Vector3d(-0.080, 0.70, -0.52), Matrix3d::Identity()));
    tp.setPhase(5, PhaseInfo(LF, 8.5, 9.9, Vector3d(-0.080, 1.25, -0.25), Matrix3d::Identity()));
    
    std::string planner_path = std::string(CONFIG_DIR)+"/"+"cfg_momSc_demo01.yaml";
    tp.setConfigurationFile(planner_path);
   
    //  Set Timeopt 
    tp.setTimeoptSolver(planner_path);    
    tp.solve();
}
BOOST_AUTO_TEST_SUITE_END ()
