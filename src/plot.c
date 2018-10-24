#include <tgmath.h>

#include "nvtop/plot.h"

static inline int data_level(double rows, double data, double increment) {
  return (int)(rows - floor(data / increment));
}

void nvtop_line_plot(WINDOW *win, size_t num_data, const double *data,
                     double min, double max) {
  if (num_data == 0)
    return;
  int rows, cols;
  getmaxyx(win, rows, cols);
  rows -= 1; // Maximum drawable character
  double increment = (max - min) / (double)(rows);
  int low_data_level = data_level(0, min, increment);
  fprintf(stderr, "Increment %f, max %f, rows %d\n", increment,
          (max - min) / increment, rows);
  int level_previous = data_level(rows, data[0], increment) - low_data_level;
  int level_next, level_current;
  for (size_t i = 0; i < num_data || i < (size_t)cols; ++i) {
    level_next = i + 1 == num_data ? level_next
                                   : data_level(rows, data[i + 1], increment) -
                                         low_data_level;
    level_current = data_level(rows, data[i], increment) - low_data_level;
    int top, bottom;
    if (level_current == level_previous) {
      mvwaddch(win, level_current, i, ACS_HLINE);
    } else {
      if (level_current < level_previous) {
        top = level_previous;
        bottom = level_current;
      } else {
        top = level_current;
        bottom = level_previous;
      }
      for (int j = bottom + 1; j < top; j++) {
        mvwaddch(win, j, i, ACS_VLINE);
      }
      if (level_current > level_previous) {
        mvwaddch(win, level_current, i, ACS_LLCORNER);
        mvwaddch(win, level_previous, i, ACS_URCORNER);
      } else {
        mvwaddch(win, level_current, i, ACS_ULCORNER);
        mvwaddch(win, level_previous, i, ACS_LRCORNER);
      }
    }
    level_previous = level_current;
  }
}

void nvtop_bar_plot(WINDOW *win, size_t num_data, const double *data,
                    double min, double max) {
  if (num_data == 0)
    return;
  int rows, cols;
  getmaxyx(win, rows, cols);
  rows -= 1;
  double increment = (max - min) / (double)(rows);
  int low_data_level = data_level(0, min, increment);
  for (size_t i = 0; i < num_data || i < (size_t)cols; ++i) {
    for (int j = data_level(rows, data[i], increment) - low_data_level;
         j <= rows; j++) {
      mvwaddch(win, j, i, ACS_CKBOARD);
    }
  }
}
