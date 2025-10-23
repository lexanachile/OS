#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <boost/thread.hpp>
#include <boost/atomic.hpp>

struct GlobalSync {
  boost::mutex csMutex;

  boost::mutex startMutex;
  boost::condition_variable startCV;
  bool isStarted;

  boost::mutex waitAllMutex;
  boost::condition_variable waitAllCV;
  int pausedThreadsCount;
};

struct ThreadControl {
  boost::mutex mtx;
  boost::condition_variable cv;
  bool stopSignal;
  bool continueSignal;
};

struct ThreadParams {
  int id;
  int* array;
  int arraySize;

  GlobalSync* globalSync;
  ThreadControl* control;

  boost::thread* threadObj;
  bool isActive;
};

#endif