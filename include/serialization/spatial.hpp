// Copyright (c) 2018-2019, CNRS
// Authors: Sanghyun Kim <ggory15@snu.ac.kr>

#ifndef __hpp_serialization_spatial_hpp__
#define __hpp_serialization_spatial_hpp__

#include <pinocchio/spatial/se3.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>

namespace boost{
  
  namespace serialization{
    
    template <class Archive, typename _Scalar, int _Options>
    void save(Archive & ar, const se3::SE3Tpl<_Scalar,_Options> & M, const unsigned int /*version*/)
    {
      ar & make_nvp("translation",make_array(M.translation().data(),3));
      ar & make_nvp("rotation",make_array(M.rotation().data(),9));
    }
    
    template <class Archive, typename _Scalar, int _Options>
    void load(Archive & ar, se3::SE3Tpl<_Scalar,_Options> & M, const unsigned int /*version*/)
    {
      ar >> make_nvp("translation",make_array(M.translation().data(),3));
      ar >> make_nvp("rotation",make_array(M.rotation().data(),9));
    }
    
    template <class Archive, typename _Scalar, int _Options>
    void serialize(Archive & ar, se3::SE3Tpl<_Scalar,_Options> & M, const unsigned int version)
    {
      split_free(ar,M,version);
    }
    
  }
  
}

#endif // ifndef __hpp_serialization_spatial_hpp__
