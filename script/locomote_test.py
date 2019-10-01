# Copyright (c) 2018-2019, CNRS
# Authors: Sanghyun Kim <ggory15@snu.ac.kr>

from configs.talos_nav import *
import numpy as np
import timeopt
import locomote
from numpy.linalg import norm

#from config import *
from locomote import WrenchCone,SOC6,ControlType,IntegratorType,ContactPatch, ContactPhaseHumanoid, ContactSequenceHumanoid

print "load previous contact sequence"
cs = ContactSequenceHumanoid(0)
CONTACT_SEQUENCE_XML_TAG = "ContactSequence"

cs.loadFromXML("contact_sequence.xml", CONTACT_SEQUENCE_XML_TAG)
q_init = cs.contact_phases[0].reference_configurations[0].copy()
num_phases = cs.size()

com0 = cs.contact_phases[0].init_state[:3]
q_ref = cs.contact_phases[0].reference_configurations[0]

posture_l = []
posture_l.append(q_ref)

t0 = 0.
RF_time = [];
LF_time = [];

time_lf = 0.0;
time_rf = 0.0;
duration_ds_r = 0.0;
duration_ds_l = 0.0;

duration_ss_r = 0.0;
duration_ss_l = 0.0;

isInit = True
isFinal = True

for k in range(num_phases):
    contact_phase = cs.contact_phases[k]
    init_guess_provided = (len(contact_phase.state_trajectory) > 0)

    if k < num_phases - 1:
        next_phase = cs.contact_phases[k + 1]

    num_active_phases = contact_phase.numActivePatches()

    assert (num_active_phases > 0)
    phase_type = locomote.HumanoidPhaseType.HUMANOID_PHASE_UNDEFINED

    # Determine the phase type
    if num_active_phases == 1:
        phase_type = locomote.HumanoidPhaseType.SINGLE_SUPPORT
    elif num_active_phases == 2:
        phase_type = locomote.HumanoidPhaseType.DOUBLE_SUPPORT
    elif num_active_phases == 3:
        phase_type = locomote.HumanoidPhaseType.TRIPLE_SUPPORT
    elif num_active_phases == 4:
        phase_type = locomote.HumanoidPhaseType.QUADRUPLE_SUPPORT
    else:
        assert False, "Must never happened"

    if phase_type == locomote.HumanoidPhaseType.SINGLE_SUPPORT:
        assert k > 0, "This phase must no be the initial one"
        assert k < num_phases - 1, "This phase must no be the final one"

        if contact_phase.RF_patch.active:
            if init_guess_provided:
                duration_ss_r = max(contact_phase.time_trajectory[0], DURATION_SS)
            else:
                duration_ss_r = DURATION_SS

            time_lf = time_lf + duration_ss_r
        elif contact_phase.LF_patch.active:
            if init_guess_provided:
                duration_ss_l = max(contact_phase.time_trajectory[0], DURATION_SS)
            else:
                duration_ss_l = DURATION_SS
            time_rf = time_rf + duration_ss_l

    elif phase_type == locomote.HumanoidPhaseType.DOUBLE_SUPPORT:
        if not next_phase.RF_patch.active:
            if init_guess_provided:
                duration_ds_l = max(contact_phase.time_trajectory[0], DURATION_DS)
            else:
                duration_ds_l = DURATION_DS
        elif not next_phase.LF_patch.active:
            if init_guess_provided:
                duration_ds_r = max(contact_phase.time_trajectory[0], DURATION_DS)
            else:
                duration_ds_r = DURATION_DS

        if k == 0 and isInit:
            if not next_phase.RF_patch.active:
                duration_ds_l = DURATION_INIT
            elif not next_phase.RF_patch.active:
                duration_ds_r = DURATION_INIT

        if k == num_phases - 1 and isFinal:  # last phase
            duration_ds_fin = DURATION_FINAL

        if not next_phase.RF_patch.active:
            RF_time.append(np.matrix([time_rf, time_rf + duration_ds_l + duration_ss_r + duration_ds_r]))
            time_rf = time_rf + duration_ds_l + duration_ss_r + duration_ds_r
        elif not next_phase.LF_patch.active:
            LF_time.append(np.matrix([time_lf, time_lf + duration_ds_l + duration_ss_l + duration_ds_r]))
            time_lf = time_lf + duration_ds_l + duration_ss_l + duration_ds_r

        if k == num_phases - 1:
            if cs.contact_phases[k - 1].RF_patch.active:
                RF_time.append(np.matrix([time_rf, time_rf + duration_ds_fin + duration_ss_r + duration_ds_r]))
                LF_time.append(np.matrix([time_lf, time_lf + duration_ds_fin]))
            elif cs.contact_phases[k - 1].LF_patch.active:
                LF_time.append(np.matrix([time_lf, time_lf + duration_ds_fin + duration_ss_l + duration_ds_l]))
                RF_time.append(np.matrix([time_rf, time_rf + duration_ds_fin]))

    elif phase_type == locomote.HumanoidPhaseType.TRIPLE_SUPPORT:
        duration = max(contact_phase.time_trajectory[0], DURATION_TS)
    else:
        assert False, "Must never happened"


RF = [];
LF = [];
for k in range((num_phases - 1) /2):
    if not cs.contact_phases[2*k + 1].RF_patch.active:
      RF.append(cs.contact_phases[2 * k].RF_patch.placement)

    if not cs.contact_phases[2 * k + 1].LF_patch.active:
      LF.append(cs.contact_phases[2 * k].LF_patch.placement)

RF.append(cs.contact_phases[num_phases-1].RF_patch.placement)
LF.append(cs.contact_phases[num_phases-1].LF_patch.placement)

tp = timeopt.problem(len(RF) + len(LF))
rot = np.matrix(np.zeros((3, 3)))
rot[0, 0] = 1.0
rot[1, 1] = 1.0
rot[2, 2] = 1.0
tp.setInitialCOM(cs.contact_phases[0].init_state[:3])
tp.setInitialPose(True, np.matrix((0.086, 0.15,-0.92)).transpose(), rot, timeopt.EndeffectorID.RF)
tp.setInitialPose(True, np.matrix((-0.086, 0.15,-0.92)).transpose(), rot, timeopt.EndeffectorID.LF)
tp.setInitialPose(True, np.matrix((0.4, 0.3, 0.0)).transpose(), rot, timeopt.EndeffectorID.RH)
tp.setInitialPose(True, np.matrix((-0.4, 0.3, 0.0)).transpose(), rot, timeopt.EndeffectorID.LH)

tp.setMass(88.0);
tp.getMass();
tp.setFinalCOM(cs.contact_phases[num_phases-1].final_state[0:3])

for i in range(len(RF)):
    tp.setPhase(i, timeopt.phase(timeopt.EndeffectorID.RF, RF_time[i][0, 0], RF_time[i][0, 1], RF[i].translation, RF[i].rotation))
for i in range(len(LF)):
    tp.setPhase(i + len(RF), timeopt.phase(timeopt.EndeffectorID.LF, LF_time[i][0, 0], LF_time[i][0, 1], LF[i].translation, LF[i].rotation))

print "set configuration file for time-optimization"
import os
cfg_path=str(os.path.dirname(os.path.abspath(__file__))) + '/../config/' + 'cfg_timeopt_demo01.yaml'

tp.setConfigurationFile(cfg_path)
tp.setTimeoptSolver(cfg_path)
tp.solve()

state=[];
s_time=0.0;
cnt = 0;
for k in range(tp.getTrajectorySize()):
  if norm(tp.getContactForce(0, k)) > 0.0 and norm(tp.getContactForce(1, k)) > 0.0:
    state.append('ds')
  elif  norm(tp.getContactForce(0, k)) > 0.0 and norm(tp.getContactForce(1, k)) == 0.0:
    state.append('ssr')
  elif norm(tp.getContactForce(1, k)) > 0.0 and norm(tp.getContactForce(0, k)) == 0.0:
    state.append('ssl')

u = [0] * 6
x = [0] * 9
MASS = tp.getMass()

for k in range(tp.getTrajectorySize()):
  if k == 0 :
      a = 1
      cs.contact_phases[cnt].time_trajectory[0] = tp.getTime(k)
  else:
      if not state[k-1] == state[k]:
          cnt = cnt+1
          cs.contact_phases[cnt].time_trajectory[0] = (tp.getTime(k))
      else:
          cnt = cnt
          cs.contact_phases[cnt].time_trajectory.append(tp.getTime(k))

  u[0:3] = tp.getLMOM(k).tolist()
  u[3:6] = tp.getAMOM(k).tolist()
  cs.contact_phases[cnt].control_trajectory.append(np.matrix(u))

  x[0:3] = tp.getCOM(k).tolist()
  x[3:6] = tp.getLMOM(k).tolist()

  if k == 0:
    x[6:9] = ((tp.getLMOM(k)/MASS) / tp.getTime(k)).tolist()
  x[6:9] = (((tp.getLMOM(k)/MASS) - (tp.getLMOM(k-1)/MASS)) / (tp.getTime(k)-tp.getTime(k-1))).tolist()
  cs.contact_phases[cnt].state_trajectory.append(np.matrix(x))

print "save resultant trajectories by time optimization"
cs.saveAsXML("Result.xml", "ContactSequence")
print "Generated Result.xml"
print("")
