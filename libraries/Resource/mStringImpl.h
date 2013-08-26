
#ifndef _STRING_IMPL_H_e48fa270_5be7_433c_b93b_a97aa405bcf4
#define _STRING_IMPL_H_e48fa270_5be7_433c_b93b_a97aa405bcf4

// Do not inclue directly

template<>
mString to_string(const long& from);

template<>
mString to_string(const int& from);

template<>
mString to_string(const float& from);

template<>
mString to_string(const double& from);


#endif // header guard
