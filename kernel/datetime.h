#include "types.h"

#ifndef DATETIME_H
#define DATETIME_H

struct datetime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};
int datetime(struct datetime *dt);
void epoch_to_datetime(uint64 ts, struct datetime *dt);

#endif
