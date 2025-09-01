#ifndef _LIB_VERSION_HH
#define _LIB_VERSION_HH

#define __STR_HELPER(X) #X
#define __STR(X) __STR_HELPER(X)

#ifndef VERSION_MAJOR
  #define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
  #define VERSION_MINOR 0
#endif

#ifndef VERSION_REV
  #define VERSION_REV 0
#endif

#ifndef VERSION_RELEASE_STREAM
  #define VERSION_RELEASE_STREAM "dev"
#endif

#define VERSION_STR __STR(VERSION_MAJOR) "." __STR(VERSION_MINOR) "." __STR(VERSION_REV) "#" __STR(VERSION_RELEASE_STREAM)


#endif /* _LIB_VERSION_HH */