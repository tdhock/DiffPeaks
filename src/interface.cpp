/* -*- compile-command: "R CMD INSTALL .. && R CMD check .." -*- */

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
  matched_diff(peak_chromStart,
	       peak_chromEnd,
	       peak_track,
	       *peak_count,
	       diff_chromStart,
	       diff_chromEnd);
}

}
