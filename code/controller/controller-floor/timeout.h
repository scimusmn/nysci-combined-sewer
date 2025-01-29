#pragma once


#define NUM_TIMEOUTS 8


typedef void (timefunc_t)(void*);


class Timeout {
  public:
  bool set(unsigned long ms, timefunc_t *f, void *data);
  void update();

  private:
  unsigned long time = 0;
  timefunc_t *f = nullptr;
  void *data = nullptr;
};


class Timeouts {
  public:
  bool setTimeout(unsigned long ms, timefunc_t *f, void *data=nullptr);
  void update();
  private:
  Timeout timeouts[NUM_TIMEOUTS];
};
