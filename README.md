# CppUTest for the Super-Simple-Tasker (library only)

A CppUTest support library for unit testing tasks intended for 
the Super Simple Tasker (SST). At this time, only supporting the 
C++ version of the SST.

To learn more about the Super Simple Tasker, [see its repo on GitHub](https://github.com/QuantumLeaps/Super-Simple-Tasker).

Copyright Matthew Eshleman, MIT Licensed (free!)

If the SST approach is too simple for your firmware project,
check out QP/C or QP/C++, along with my associated cpputest-for-qp* 
libraries. 

If this project happens to inspire your team to instead select 
a QP framework for commercial use, please note
"Matthew Eshleman" or "Cove Mountain Software" in the referral
field when acquiring a commercial license from Quantum Leaps. Referrals
encourage and support efforts like this. Thank you!


# Introduction

The `cpputest-for-sst` library project enables CppUTest for the
Super-Simple-Tasker (SST). This project provides for the
following capabilities:

* A CppUTest compatible port of the SST,
  enabling host based unit and integration testing of SST 
  Tasks (active objects.)
* Supporting utilities to simplify unit testing of SST
  based Tasks. 
* A separate examples project providing usage/examples of this library (TODO)

Benefits of this approach to unit testing active objects include:
* No surprises. The active object under test interacts with the
  SST framework, ensuring tested behavior will match target
  behavior. The test support code provided in this project
  enables precise control of when an active object executes within
  the framework. It is effectively running a copy of SST0, 
  the non-preemptive kernel, which is only appropriate for unit testing.
* No surprises (again). There are no threads. Each test explicitly
  'gives' CPU time to the active objects under test as needed by
  each test.
* Accelerated timer related testing. Seconds of 'time' can be
  tested in microseconds.
* Host based testing is fast. TDD (Test Driven Development) demands
  fast execution of tests.
* Host based testing is easier to use in a continuous integration (CI)
  system. Run all the tests with every commit. No surprises when
  it is time to generate a target release.

**An example project demonstrating the features of this library and
how to import and use from with a CMake build environment is available
here: TODO **
