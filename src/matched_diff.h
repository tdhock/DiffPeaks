#include <list>
#include <map>

class Match;
typedef std::multimap< int, Match* > SortedMatches;
typedef std::pair< int, Match* > JaccardMatch;

class Peak;
typedef std::list< Peak* > Peaks;

class Match {
public:
    Peaks::iterator one;
    Peaks::iterator two;
}

class Peak {
public:
    int chromStart;
    int chromEnd;
    int track;
    std::list< SortedMatches::iterator > matches;
}

int matched_diff(int*, int*, int*, int, int*, int*);

