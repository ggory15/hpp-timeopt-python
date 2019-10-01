# Copyright (c) 2018-2019, CNRS
# Authors: Sanghyun Kim <ggory15@snu.ac.kr>

import timeopt
import numpy as np

######
rot = np.matrix(np.zeros((3, 3)))
rot[0, 0] = 1.0
rot[1, 1] = 1.0
rot[2, 2] = 1.0
######

tp = timeopt.problem(6)

tp.setInitialCOM(np.matrix((0, 0.15, -0.2)).transpose())

tp.setInitialPose(True, np.matrix((0.086, 0.15,-0.92)).transpose(), rot, timeopt.EndeffectorID.RF)
tp.setInitialPose(True, np.matrix((-0.086, 0.15,-0.92)).transpose(), rot, timeopt.EndeffectorID.LF)
tp.setInitialPose(True, np.matrix((0.4, 0.3, 0.0)).transpose(), rot, timeopt.EndeffectorID.RH)
tp.setInitialPose(True, np.matrix((-0.4, 0.3, 0.0)).transpose(), rot, timeopt.EndeffectorID.LH)
tp.setMass(60.0);
tp.setFinalCOM(np.matrix((0.2, 1.2, 0.4)).transpose())

tp.setPhase(0, timeopt.phase(timeopt.EndeffectorID.RF, 0.0, 1.0, np.matrix((0.086, 0.15,-0.92)).transpose(), rot))
tp.setPhase(1, timeopt.phase(timeopt.EndeffectorID.RF, 2.0, 4.5, np.matrix((0.500, 0.45, -0.76)).transpose(), rot))
tp.setPhase(2, timeopt.phase(timeopt.EndeffectorID.RF, 6.0, 9.9, np.matrix((0.450, 0.98, -0.27)).transpose(), rot))
tp.setPhase(3, timeopt.phase(timeopt.EndeffectorID.LF, 0.0, 2.5, np.matrix((-0.086, 0.15, -0.92)).transpose(), rot))
tp.setPhase(4, timeopt.phase(timeopt.EndeffectorID.LF, 4.0, 6.5, np.matrix((-0.080, 0.70, -0.52)).transpose(), rot))
tp.setPhase(5, timeopt.phase(timeopt.EndeffectorID.LF, 8.5, 9.9, np.matrix((-0.080, 1.25, -0.25)).transpose(), rot))

import os
cfg_path=str(os.path.dirname(os.path.abspath(__file__))) + '/../config/' + 'cfg_momSc_demo01.yaml'
tp.setConfigurationFile(cfg_path)

tp.setTimeoptSolver(cfg_path)
tp.solve()

# For resultant trajectory size
size = tp.getTrajectorySize()

# For ith COM
com = tp.getCOM(10)

print "The result was created in config folder"


