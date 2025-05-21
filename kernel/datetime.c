#include "datetime.h"


int is_leap(int y) {
  return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

void epoch_to_datetime(uint64 ts, struct datetime *dt) {
  int days_in_month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
  uint64 seconds = ts;

  dt->second = seconds % 60;
  seconds /= 60;
  dt->minute = seconds % 60;
  seconds /= 60;
  dt->hour = seconds % 24;
  seconds /= 24;

  int days = (int)seconds;
  int year = 1970;

  while (1) {
    int days_in_year = is_leap(year) ? 366 : 365;
    if (days >= days_in_year) {
      days -= days_in_year;
      year++;
    } else break;
  }

  dt->year = year;
  int month = 0;
  while (1) {
    int dim = days_in_month[month];
    if (month == 1 && is_leap(year)) dim++;  // February leap year
    if (days >= dim) {
      days -= dim;
      month++;
    } else break;
  }

  dt->month = month + 1;
  dt->day = days + 1;
}
