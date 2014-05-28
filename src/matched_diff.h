#include <cstdio>
#include <list>
#include <map>
#include <set>

class Match;
typedef std::multimap< double, Match*, std::greater<double> > SortedMatches;
typedef std::pair< double, Match* > JaccardMatch;
typedef std::map< int, SortedMatches::iterator > MatchMap;
typedef std::pair< int, SortedMatches::iterator > IDMatch;
typedef std::set<int> IDSet;

class Peak;
typedef std::list< Peak* > PeakList;

class Match {
public:
    PeakList::iterator one;
    PeakList::iterator two;
    Match(PeakList::iterator o, PeakList::iterator t){
	one = o;
	two = t;
    }
};

class Peak {
    int chromStart;
    int chromEnd;
    int track;
public:
    MatchMap matches;
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
    SortedMatches possible_matches;
    IDSet matched;
    int peak_list_chromEnd, new_match_id;
    PeakList::iterator last_one_it, last_two_it;
public:
    PeakList peak_list;
    void init(){
	last_one_it = last_two_it = peak_list.end(); // NULL.
	peak_list_chromEnd = 0;
	new_match_id = 0;
	matched.clear();
    }
    int get_chromEnd(){
	return peak_list_chromEnd;
    }
    int add_peak(Peak *peak){
	PeakList::iterator peak_it;
	int before, after;
	before = peak_list.size();
	peak_list.push_front(peak);
	after = peak_list.size();
	peak_it = peak_list.begin();
	if(peak_list_chromEnd < peak->get_chromEnd()){
	    peak_list_chromEnd = peak->get_chromEnd();
	}
	int track = peak->get_track();
	/* printf("add_peak %d-%d track=%d chromEnd=%d size=%d\n", */
	/*        peak->get_chromStart(), */
	/*        peak->get_chromEnd(), */
	/*        track, peak_list_chromEnd, */
	/*        peak_list.size()); */
	switch(track) {
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
	Peak *peak, *last;
	peak = *peak_it;
	last = *last_it;
	int Union, Intersection, leftEnd, rightEnd;
	if(peak->get_chromEnd() < last->get_chromEnd()){
	    rightEnd = last->get_chromEnd();
	    leftEnd = peak->get_chromEnd();
	}else{
	    rightEnd = peak->get_chromEnd();
	    leftEnd = last->get_chromEnd();
	}
	Union = rightEnd - last->get_chromStart();
	Intersection = leftEnd - peak->get_chromStart();
	double jaccard = (double)Intersection/(double)Union;
	/* printf("%d-%d %d-%d union=%d intersection=%d jaccard=%f\n", */
	/*        last->get_chromStart(), */
	/*        last->get_chromEnd(), */
	/*        peak->get_chromStart(), */
	/*        peak->get_chromEnd(), */
	/*        Union, Intersection, jaccard); */
	Match *match;
	match = new Match(last_it, peak_it);
	JaccardMatch jm = JaccardMatch(jaccard, match);
	SortedMatches::iterator sorted_it = possible_matches.insert(jm);
	IDMatch id_match = IDMatch(new_match_id, sorted_it);
	new_match_id++;
	last->matches.insert(id_match);
	peak->matches.insert(id_match);
    }
    void remove_matching(){
	Match *match;
	JaccardMatch jm;
	SortedMatches::iterator match_it;
	while(!possible_matches.empty()){
	    match_it = possible_matches.begin();
	    jm = *match_it;
	    match = jm.second;
	    /* printf("remove_matching Jaccard %f %d-%d %d-%d\n", jm.first, */
	    /* 	   (*match->one)->get_chromStart(), */
	    /* 	   (*match->one)->get_chromEnd(), */
	    /* 	   (*match->two)->get_chromStart(), */
	    /* 	   (*match->two)->get_chromEnd()); */
	    remove_peak(match->one);
	    remove_peak(match->two);
	}
    }
    void remove_peak(PeakList::iterator peak_it){
	MatchMap::iterator matches_it;
	SortedMatches::iterator sorted_it;
	IDMatch id_match;
	JaccardMatch jm;
	int id;
	Peak *peak;
	Match *match;
	peak = *peak_it;
	/* printf("begin remove_peak peak_list=%d peak->matches=%d possible_matches=%d matched=%d\n",  */
	/*        peak_list.size(), peak->matches.size(),  */
	/*        possible_matches.size(), matched.size()); */
	while(!peak->matches.empty()){
	    matches_it = peak->matches.begin();
	    id_match = *matches_it;
	    id = id_match.first;
	    sorted_it = id_match.second;
	    if(matched.find(id) == matched.end()){
		//printf("marking id==%d\n", id);
		matched.insert(id); // mark as already matched.
		jm = *sorted_it;
		match = jm.second;
		delete match;
		possible_matches.erase(sorted_it);
	    }
	    peak->matches.erase(matches_it);
	}
	peak_list.erase(peak_it);
	delete peak;
	/* printf("end   remove_peak peak_list=%d peak->matches=%d possible_matches=%d matched=%d\n",  */
	/*        peak_list.size(), peak->matches.size(),  */
	/*        possible_matches.size(), matched.size()); */
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
