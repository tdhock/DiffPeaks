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
	return chromStart;
    }
    int get_chromEnd(){
	return chromEnd;
    }
    int get_track(){
	return track;
    }
    Peak(int s, int e, int t){
	chromStart = s;
	chromEnd = e;
	track = t;
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
	return peak_list_chromEnd;
    }
    int add_peak(Peak *peak){
	PeakList::iterator peak_it;
	peak_list.push_front(peak);
	peak_it = peak_list.begin();
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
	    return; // there is no previous peak in this group.
	}
	int Union, Intersection, leftEnd, rightEnd;
	if(peak_it->chromEnd < last_it->chromEnd){
	    rightEnd = last_it->chromEnd;
	    leftEnd = peak_it->chromEnd;
	}else{
	    rightEnd = peak_it->chromEnd;
	    leftEnd = last_it->chromEnd;
	}
	Union = rightEnd - last_it->chromStart;
	Intersection = leftEnd - peak_it->chromStart;
	double jaccard = (double)Union/(double)Intersection;
	Match *match;
	match = new Match(last_it, peak_it);
	JaccardMatch jm = JaccardMatch(jaccard, match);
	sorted_matches.insert(jm);
	last_it->matches.push_front(match);
	peak_it->matches.push_front(match);
    }
    void remove_matching(){
	Match *match;
	SortedMatches::iterator match_it;
	while(!sorted_matches.empty()){
	    match_it = sorted_matches.rbegin();
	    match = *match_it;
	    remove_peak(match->one);
	    remove_peak(match->two);
	}
    }
// http://www.cplusplus.com/reference/list/list/erase/
// If position (or the range) is valid, list::erase never throws
// exceptions (no-throw guarantee).
// Otherwise, it causes undefined behavior.
    void remove_peak(PeakList::iterator peak_it){
	SortedMatches::iterator match_it;
	Match *match;
	Peak *peak;
	while(!peak_it->matches.empty()){
	    match_it = peak_it->matches.begin();
	    match = *match_it;
	    sorted_matches.erase(match_it);
	    delete match;
	}
	peak = *peak_it;
	peak_list.erase(peak_it);
	delete peak;
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
