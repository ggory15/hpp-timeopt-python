// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __timeopt_fwd_hpp__
#define __timeopt_fwd_hpp__

# include <vector>
# include <Eigen/Core>


  namespace timeopt
  {
    enum EndeffectorID{
      RF = 0,
      LF = 1,
      RH = 2,
      LH = 3,
      EE_Undefined
    };
    enum PhaseType{
      SS = 0,
      DS = 1,
      TS = 2,
      PT_Undefined
    };
  }


#endif // ifndef __timeopt_fwd_hpp__
