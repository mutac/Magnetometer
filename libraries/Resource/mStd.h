
#ifndef STD_H_06c7ac8b_2eb5_4f6f_8e77_183aadad9001
#define STD_H_06c7ac8b_2eb5_4f6f_8e77_183aadad9001

namespace mStd
{
  /**
   */
  template <class T>
  void swap(T& a, T& b)
  {
    T c(a);
    a = b; 
    b = c;
  }
};

#endif // header guard
