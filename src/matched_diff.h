#include <list>
#include <map>

class Match;
typedef std::multimap< double, Match* > SortedMatches;
typedef std::pair< double, Match* > JaccardMatch;

class Peak;
typedef std::list< Peak* > PeakList;

class Match {
    PeakList::iterator one;
    PeakList::iterator two;
public:
};

class Peak {
    int chromStart;
    int chromEnd;
    int track;
    std::list< SortedMatches::iterator > matches;
public:
    int get_chromStart(){
	return this->chromStart;
    }
    int get_chromEnd(){
	return this->chromEnd;
    }
    int get_track(){
	return this->track;
    }
    Peak(int chromStart, int chromEnd, int track){
	this->chromStart = chromStart;
	this->chromEnd = chromEnd;
	this->track = track;
    }
};

int matched_diff(int*, int*, int*, int, int*, int*);

#define ERROR_UNRECOGNIZED_PEAK_TRACK -1

class PeakMatches {
    SortedMatches sorted_matches;
    PeakList peak_list;
    int peak_list_chromEnd;
    PeakList::iterator last_one_it, last_two_it;
public:
    void init(){
	last_one_it = last_two_it = peak_list.end(); // NULL.
	int peak_list_chromEnd = 0;
    }
    int get_chromEnd(){
	return this->peak_list_chromEnd;
    }
    int add_peak(Peak *peak){
	PeakList::iterator peak_it;
	this->peak_list.push_front(peak);
	peak_it = this->peak_list.begin();
	if(peak_list_chromEnd < peak->get_chromEnd()){
	    peak_list_chromEnd = peak->get_chromEnd();
	}
	switch(peak->get_track()) {
	case 1:
	    add_match(last_two_it, peak_it);
	    last_one_it = peak_it;
	    break;
	case 2:
	    add_match(last_one_it, peak_it);
	    last_two_it = peak_it;
	    break;
	default:
	    return ERROR_UNRECOGNIZED_PEAK_TRACK;
	}
	return 0;
    }
    void add_match(PeakList::iterator last_it, PeakList::iterator peak_it){
	if(last_it == peak_list.end()){
	}
    }
    void remove_matching(){
	while(sorted_matches.size()){
	}
    }
    bool has_peaks(){
	return !peak_list.empty();
    }
    Peak* pop_peak(){
	PeakList::iterator peak_it;
	Peak *peak;
	peak_it = peak_list.begin();
	peak = *peak_it;
	peak_list.erase(peak_it);
	return peak;
    }
};
