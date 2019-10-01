// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#include "timeopt/phase.hpp"

namespace timeopt
{
  PhaseInfo::PhaseInfo()
  : ee_id(EE_Undefined)
  , start_time(-1.)
  , end_time(-1.)
  , pos(Vector3d(0,0,0))
  , rot(Matrix3d::Zero())
  {}

  PhaseInfo::PhaseInfo(const PhaseInfo & other)
  : ee_id(other.ee_id)
  , start_time(other.start_time)
  , end_time(other.end_time)
  , pos(other.pos)
  , rot(other.rot)
  {}

  PhaseInfo::PhaseInfo(const EndeffectorID & ee, const double & stime, const double & etime, const Vector3d& pos, const Matrix3d& rot)
  : ee_id(ee)
  , start_time(stime)
  , end_time(etime)
  , pos(pos)
  , rot(rot)
  {}
}

