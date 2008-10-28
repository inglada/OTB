## This file should be placed in the root directory of your project.
## Then modify the CMakeLists.txt file in the root directory of your
## project to incorporate the testing dashboard.
## # The following are required to uses Dart and the Cdash dashboard
##   ENABLE_TESTING()
##   INCLUDE(Dart)
set(CTEST_PROJECT_NAME "OTB")
set(CTEST_NIGHTLY_START_TIME "20:00:00 CEST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "www.orfeo-toolbox.org")
set(CTEST_DROP_LOCATION "/Dashboard/submit.php?project=OTB")
set(CTEST_DROP_SITE_CDASH TRUE)
