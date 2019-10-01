// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __hpp_serialization_fwd_hpp__
#define __hpp_serialization_fwd_hpp__

#include <boost/serialization/nvp.hpp>

#define BOOST_SERIALIZATION_MAKE_NVP(member) boost::serialization::make_nvp(##member,member)

#endif // ifndef __hpp_serialization_fwd_hpp__
