//
// File: ideal.cc  
//        -- Instantiations of classes gBasis and gIdeal
// 
// @(#)ideal.cc	1.1 01/07/98
//

#include "ideal.imp"

//template class gIdeal<int>;
//template class gBasis<int>;
//template gOutput &operator<<(gOutput &f, const gBasis<int> &y);

template class gIdeal<gRational>;
//template class gBasis<gRational>;
//template gOutput &operator<<(gOutput &f, const gBasis<gRational> &y);

template class gIdeal<double>;
//template class gBasis<double>;
//template gOutput &operator<<(gOutput &f, const gBasis<double> &y);

template class gIdeal<gDouble>;


