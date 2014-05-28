/* -*- compile-command: "rm -f *.so *.o && R CMD INSTALL .. && R CMD check .." -*- */

#include <R.h>
#include <functional>
#include "matched_diff.h"

extern "C" {

void 
matched_diff_interface
(int *peak_chromStart,
 int *peak_chromEnd,
 int *peak_track,
 int *peak_count,
 int *diff_chromStart,
 int *diff_chromEnd){
  int diff_count_or_error = 
  matched_diff(peak_chromStart,
	       peak_chromEnd,
	       peak_track,
	       *peak_count,
	       diff_chromStart,
	       diff_chromEnd);
  if(diff_count_or_error == ERROR_UNRECOGNIZED_PEAK_TRACK){
    error("unrecognized peak track");
  }
  if(diff_count_or_error < 0){
    error("undefined error code");
  }
  *peak_count = diff_count_or_error;
}

}
