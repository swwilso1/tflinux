################################################################################
#####
##### Tectiform TFLinux CMake Configuration File
##### Created by: Steve Wilson
#####
################################################################################

include(CheckIncludeFile)
include(CheckIncludeFileCXX)

if(HEADER_TEST_COMPILE_FLAGS)
    set(CMAKE_REQUIRED_FLAGS ${HEADER_TEST_COMPILE_FLAGS})
endif()

# Standard C header files

CHECK_INCLUDE_FILE(assert.h HAS_ASSERT_H)
CHECK_INCLUDE_FILE(complex.h HAS_COMPLEX_H)
CHECK_INCLUDE_FILE(ctype.h HAS_CTYPE_H)
CHECK_INCLUDE_FILE(errno.h HAS_ERRNO_H)
CHECK_INCLUDE_FILE(fenv.h HAS_FENV_H)
CHECK_INCLUDE_FILE(float.h HAS_FLOAT_H)
CHECK_INCLUDE_FILE(inttypes.h HAS_INTTYPES_H)
CHECK_INCLUDE_FILE(iso646.h HAS_ISO646_H)
CHECK_INCLUDE_FILE(limits.h HAS_LIMITS_H)
CHECK_INCLUDE_FILE(locale.h HAS_LOCALE_H)
CHECK_INCLUDE_FILE(math.h HAS_MATH_H)
CHECK_INCLUDE_FILE(setjmp.h HAS_SETJMP_H)
CHECK_INCLUDE_FILE(signal.h HAS_SIGNAL_H)
CHECK_INCLUDE_FILE(stdalign.h HAS_STDALIGN_H)
CHECK_INCLUDE_FILE(stdarg.h HAS_STDARG_H)
CHECK_INCLUDE_FILE(stdatomic.h HAS_STDATOMIC_H)
CHECK_INCLUDE_FILE(stdbool.h HAS_STDBOOL_H)
CHECK_INCLUDE_FILE(stddef.h HAS_STDDEF_H)
CHECK_INCLUDE_FILE(stdint.h HAS_STDINT_H)
CHECK_INCLUDE_FILE(stdio.h HAS_STDIO_H)
CHECK_INCLUDE_FILE(stdlib.h HAS_STDLIB_H)
CHECK_INCLUDE_FILE(stdnoreturn.h HAS_STDNORETURN_H)
CHECK_INCLUDE_FILE(string.h HAS_STRING_H)
CHECK_INCLUDE_FILE(tgmath.h HAS_TGMATH_H)
CHECK_INCLUDE_FILE(threads.h HAS_THREADS_H)
CHECK_INCLUDE_FILE(time.h HAS_TIME_H)
CHECK_INCLUDE_FILE(uchar.h HAS_UCHAR_H)
CHECK_INCLUDE_FILE(wchar.h HAS_WCHAR_H)
CHECK_INCLUDE_FILE(wctype.h HAS_WCTYPE_H)

# Standard C++ header files

CHECK_INCLUDE_FILE_CXX(any HAS_ANY)
CHECK_INCLUDE_FILE_CXX(algorithm HAS_ALGORITHM)
CHECK_INCLUDE_FILE_CXX(array HAS_ARRAY)
CHECK_INCLUDE_FILE_CXX(atomic HAS_ATOMIC)
CHECK_INCLUDE_FILE_CXX(bitset HAS_BITSET)
CHECK_INCLUDE_FILE_CXX(cassert HAS_CASSERT)
CHECK_INCLUDE_FILE_CXX(cctype HAS_CCTYPE)
CHECK_INCLUDE_FILE_CXX(cerrno HAS_CERRNO)
CHECK_INCLUDE_FILE_CXX(cfenv HAS_CFENV)
CHECK_INCLUDE_FILE_CXX(cfloat HAS_CFLOAT)
CHECK_INCLUDE_FILE_CXX(chrono HAS_CHRONO)
CHECK_INCLUDE_FILE_CXX(cinttypes HAS_CINTTYPES)
CHECK_INCLUDE_FILE_CXX(climits HAS_CLIMITS)
CHECK_INCLUDE_FILE_CXX(clocale HAS_CLOCALE)
CHECK_INCLUDE_FILE_CXX(cmath HAS_CMATH)
CHECK_INCLUDE_FILE_CXX(codecvt HAS_CODECVT)
CHECK_INCLUDE_FILE_CXX(complex HAS_COMPLEX)
CHECK_INCLUDE_FILE_CXX(condition_variable HAS_CONDITION_VARIABLE)
CHECK_INCLUDE_FILE_CXX(cstdio HAS_CSTDIO)
CHECK_INCLUDE_FILE_CXX(cstdlib HAS_CSTDLIB)
CHECK_INCLUDE_FILE_CXX(csignal HAS_CSIGNAL)
CHECK_INCLUDE_FILE_CXX(csetjmp HAS_CSETJMP)
CHECK_INCLUDE_FILE_CXX(cstdarg HAS_CSTDARG)
CHECK_INCLUDE_FILE_CXX(cstddef HAS_CSTDDEF)
CHECK_INCLUDE_FILE_CXX(cstdint HAS_CSTDINT)
CHECK_INCLUDE_FILE_CXX(cstring HAS_CSTRING)
CHECK_INCLUDE_FILE_CXX(ctime HAS_CTIME)
CHECK_INCLUDE_FILE_CXX(cuchar HAS_CUCHAR)
CHECK_INCLUDE_FILE_CXX(cwchar HAS_CWCHAR)
CHECK_INCLUDE_FILE_CXX(cwctype HAS_CWCTYPE)
CHECK_INCLUDE_FILE_CXX(deque HAS_DEQUE)
CHECK_INCLUDE_FILE_CXX(exception HAS_EXCEPTION)
CHECK_INCLUDE_FILE_CXX(execution HAS_EXECUTION)
CHECK_INCLUDE_FILE_CXX(filesystem HAS_FILESYSTEM)
CHECK_INCLUDE_FILE_CXX(forward_list HAS_FORWARD_LIST)
CHECK_INCLUDE_FILE_CXX(fstream HAS_FSTREAM)
CHECK_INCLUDE_FILE_CXX(functional HAS_FUNCTIONAL)
CHECK_INCLUDE_FILE_CXX(future HAS_FUTURE)
CHECK_INCLUDE_FILE_CXX(initializer_list HAS_INITIALIZER_LIST)
CHECK_INCLUDE_FILE_CXX(iomanip HAS_IOMANIP)
CHECK_INCLUDE_FILE_CXX(ios HAS_IOS)
CHECK_INCLUDE_FILE_CXX(iosfwd HAS_IOSFWD)
CHECK_INCLUDE_FILE_CXX(iostream HAS_IOSTREAM)
CHECK_INCLUDE_FILE_CXX(istream HAS_ISTREAM)
CHECK_INCLUDE_FILE_CXX(iterator HAS_ITERATOR)
CHECK_INCLUDE_FILE_CXX(limits HAS_LIMITS)
CHECK_INCLUDE_FILE_CXX(list HAS_LIST)
CHECK_INCLUDE_FILE_CXX(locale HAS_LOCALE)
CHECK_INCLUDE_FILE_CXX(map HAS_MAP)
CHECK_INCLUDE_FILE_CXX(memory HAS_MEMORY)
CHECK_INCLUDE_FILE_CXX(mutex HAS_MUTEX)
CHECK_INCLUDE_FILE_CXX(new HAS_NEW)
CHECK_INCLUDE_FILE_CXX(numeric HAS_NUMERIC)
CHECK_INCLUDE_FILE_CXX(optional HAS_OPTIONAL)
CHECK_INCLUDE_FILE_CXX(ostream HAS_OSTREAM)
CHECK_INCLUDE_FILE_CXX(queue HAS_QUEUE)
CHECK_INCLUDE_FILE_CXX(random HAS_RANDOM)
CHECK_INCLUDE_FILE_CXX(ratio HAS_RATIO)
CHECK_INCLUDE_FILE_CXX(regex HAS_REGEX)
CHECK_INCLUDE_FILE_CXX(scoped_allocator HAS_SCOPED_ALLOCATOR)
CHECK_INCLUDE_FILE_CXX(set HAS_SET)
CHECK_INCLUDE_FILE_CXX(shared_mutex HAS_SHARED_MUTEX)
CHECK_INCLUDE_FILE_CXX(sstream HAS_SSTREAM)
CHECK_INCLUDE_FILE_CXX(stack HAS_STACK)
CHECK_INCLUDE_FILE_CXX(stdexcept HAS_STDEXCEPT)
CHECK_INCLUDE_FILE_CXX(streambuf HAS_STREAMBUF)
CHECK_INCLUDE_FILE_CXX(string HAS_STRING)
CHECK_INCLUDE_FILE_CXX(string_view HAS_STRING_VIEW)
CHECK_INCLUDE_FILE_CXX(strstream HAS_STRSTREAM)
CHECK_INCLUDE_FILE_CXX(system_error HAS_SYSTEM_ERROR)
CHECK_INCLUDE_FILE_CXX(thread HAS_THREAD)
CHECK_INCLUDE_FILE_CXX(tuple HAS_TUPLE)
CHECK_INCLUDE_FILE_CXX(typeinfo HAS_TYPEINFO)
CHECK_INCLUDE_FILE_CXX(typeindex HAS_TYPEINDEX)
CHECK_INCLUDE_FILE_CXX(type_traits HAS_TYPE_TRAITS)
CHECK_INCLUDE_FILE_CXX(unordered_map HAS_UNORDERED_MAP)
CHECK_INCLUDE_FILE_CXX(unordered_set HAS_UNORDERED_SET)
CHECK_INCLUDE_FILE_CXX(utility HAS_UTILITY)
CHECK_INCLUDE_FILE_CXX(valarray HAS_VALARRAY)
CHECK_INCLUDE_FILE_CXX(variant HAS_VARIANT)
CHECK_INCLUDE_FILE_CXX(vector HAS_VECTOR)

# Platform specific headers
CHECK_INCLUDE_FILE(libudev.h HAVE_LIBUDEV_H)
if(NOT HAVE_LIBUDEV_H)
    message(FATAL_ERROR "Can not find libudev.h, libudev is necessary for the library")
endif()

