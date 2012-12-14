
#include <set>
#include <iostream>
#include <string.h>
#include <vector>
using std::set;
using std::cout;
using std::string;

// Naive comparator.
// Provides a default for any type that has an operator<
// and an operator==.
//
template<class T>
class comp {
public:
    bool precedes(T a, T b) {
        // your code here
        return a < b;
    }
    bool equals(T a, T b) {
        // your code here
        return a == b;
    }
};

// Abstract base class from which all sets are derived.
//
template<class T, class C = comp<T> >
class simple_set {
public:
    virtual ~simple_set<T, C>() {}
        // destructor should be virtual so that we call the right
        // version when saying, e.g.,
        // simple_set* S = new derived_set(args);
        //  ...
        // delete S;
    virtual simple_set<T, C>& operator+=(T item) = 0;
        // inserts item into set
        // returns a ref so you can say, e.g.
        // S += a += b += c;
    virtual simple_set<T, C>& operator-=(T item) = 0;
        // removes item from set, if it was there (otherwise does nothing)
    virtual bool contains(T item) = 0;
        // indicates whether item is in set
};

//---------------------------------------------------------------

// Example of a set that implements the simple_set interface.
// Borrows the balanced tree implementation of the standard template
// library.  Note that you are NOT to use any STL collections in your
// code (though you may use strings and streams).
//
template<class T>
class stl_simple_set : public virtual simple_set<T>, protected set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
public:
    virtual ~stl_simple_set<T>() {}   // will invoke std::~set<T>()
    virtual stl_simple_set<T>& operator+=(T item) {
        set<T>::insert(item);
        return *this;
    }
    virtual stl_simple_set<T>& operator-=(T item) {
        (void) set<T>::erase(item);
        return *this;
    }
    virtual bool contains(T item) {
        return (set<T>::find(item) != set<T>::end());
    }
};

//---------------------------------------------------------------

// Characteristic array implementation of set.
// Requires instantiation with guaranteed low and one-more-than-high
// bounds on elements that can be placed in the set.  Should compile
// and run correctly for any element class T that can be cast to int.
// Throws out_of_bounds exception when appropriate.
//
class out_of_bounds {};     // exception
template<class T>
class carray_simple_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
    // needs some data members
    int *carr;
	int low;
	int high;
public:
   
    carray_simple_set(T l, T h) {
		low= (int) l;
		high= (int) h;
		carr= new int[high-low];	
    }   // constructor
    virtual ~carray_simple_set() { }
    virtual carray_simple_set<T>& operator+=(T item) {         				

		int intrep = (int)item;
		//structure that accounts for overflow error.
		if (((intrep < low) == false) && ((intrep < high)==true)){
			carr[intrep] = 1;			
		}
		else{
			cout << "\n\nYOU TRIED TO INSERT AN ELEMENT NOT IN THE RANGE OF INSERTABLE ELEMENTS.  ERROR.\n\n";
		}
		return *this;	
    }
    virtual carray_simple_set<T>& operator-=(T item) { 
		//sets the bit that represents whether item is in the set to false.		
		carr[(int)item] = 0;
		return *this;
    }
    virtual bool contains(T item) { 
    	//returns true if item is in the characteristic array set.
    	int intrep = (int) item;
    	if (carr[intrep] == 1){
    		return true;
    	}
    	else{
    		return false;
    	}
    	
    	return false;
    	
    }
};

//---------------------------------------------------------------

// Naive hash function object.
// Provides a default for any type that can be cast to int.
//
template<class T>
class cast_to_int {
	public:
    int operator()(T n) {
        return (int) n;
    }
};

// Hash table implementation of set.
// Requires instantiation with guaranteed upper bound on number of elements
// that may be placed in set.  Throws overflow if bound is exceeded.
// Can be instantiated without second parameter if element type can
// be cast to int; otherwise requires hash function object.
//
class overflow {};          // exception
template<class T, class F = cast_to_int<T> >
class hashed_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
    // needs some data members
    int *hashTable;
    F hash;
    int nextern;
    int nexterncopy;
public:
    hashed_set(int n) { 
		hash = F();			
		hashTable = new int[n];
		//null value is represented by -1.
		for(int i=0;i<n;i++){
			hashTable[i] = -1;
		}
		nextern = n;
		nexterncopy = n;		
	}
	virtual ~hashed_set() { }
    virtual hashed_set<T>& operator+=(T item) {
		int hashcode = hash(item);		
		if (hashcode == 0 || hashcode == -1) hashcode = 2;
		int p;
		//prime finding
		if(isprime(nextern)){
			p = nextern;
		}
		else{
			//finds the next prime greater than n.
			while(!isprime(nexterncopy)){
				nexterncopy++;
			}
			p = nexterncopy;
		}

		int k = 1;
		//adds to the hash table based on what k is.  If there are 25 or more collisions, you can assume the array is full.
		while (k!=25){
			if(hashTable[hashcode%p] == -1){
				hashTable[hashcode%p] = (int) item;
				break;
			}			
			else{
				k++;				
				hashcode = hashcode * k;
				if (hashcode == 0 || hashcode == -1) hashcode = 2;
			}
		}
		if (k==25){
			cout << "ARRAY FULL!  CANNOT INSERT ANY MORE VALUES\n";
		}
		return *this;	
    }
    virtual hashed_set<T>& operator-=(T item) { 
		//this has the same implementation as the + operator.
		int hashcode = hash(item);
		if (hashcode == 0 || hashcode == -1) hashcode = 2;
		int p;
		nexterncopy = nextern;
		if(isprime(nextern)){
			p = nextern;
		}
		else{
			while(!isprime(nexterncopy)){
				nexterncopy++;
			}
			p = nexterncopy;
		}
		int k = 1;
		while (k!=25){
			if(hashTable[hashcode%p] == item){
				hashTable[hashcode%p] = -1;
				break;
			}
			else{
				k++;				
				hashcode = hashcode * k;
				if (hashcode == 0 || hashcode == -1) hashcode = 2;
			}
		}
		return *this;
    }
    virtual bool contains(T item) { 
    	int hashcode = hash(item);
    	if (hashcode == 0 || hashcode == -1) hashcode = 2;
		int p;
		nexterncopy = nextern;
		if(isprime(nextern)){
			p = nextern;
		}
		else{
			while(!isprime(nexterncopy)){
				nexterncopy++;
			}
			p = nexterncopy;
		}
		int k = 1;
		while (k!=25){
			if(hashTable[hashcode%p] == item){
				return true;
			}
			else if (hashTable[hashcode%p] == 0){
				return false;
			}
			else{
				k++;				
				hashcode = hashcode * k;
				if (hashcode == 0 || hashcode == -1) hashcode = 2;
			}
		}
		if (k==25){
			cout << "\n\nARRAY IS FULL.  YOU CANNOT INSERT ANY MORE. \n\n";
		}
		return false;
    	
    }
    // idea for isprime function from http://www.java-samples.com/showtutorial.php?tutorialid=1361
    virtual bool isprime(int x){
    	for(int i=2; i<x; i++){
			if (x % i == 0){
				return false;
			}
		}
		return true;
    }
        // I recommend you pick a hash table size p that is a prime
        // number >= n, use F(e) % p as your hash function, and rehash
        // with kF(e) % p after the kth collision.  (But make sure that
        // F(e) is never 0.)
//  more methods here
};

//---------------------------------------------------------------

// Sorted array implementation of set; supports binary search.
// Requires instantiation with guaranteed upper bound on number of
// elements that may be placed in set.  Throws overflow if bound is
// exceeded.
//
template<class T>
class bin_search_simple_set : public virtual simple_set<T> {
	 // 'virtual' on simple_set ensures single copy if multiply inherited
    // needs some data members
    int *binSetarr;
    int nextern;
public:
    bin_search_simple_set(int n) { 
		binSetarr = new int[n];
		for(int i=0;i<n;i++){
			binSetarr[i] = -1;
		}
		nextern = n;
	}
	virtual ~bin_search_simple_set() { }
    virtual bin_search_simple_set<T>& operator+=(T item) {
		//inserts while keeping the list sorted and handles overflow.
		if(binSetarr[0] == -1){
			binSetarr[0] = item;
		}
		else{
			int i=0;			
			
			while(binSetarr[i] != -1){
				i++;
				if (i>nextern){
					cout << "\n\nARRAY IS FULL.  YOU CANNOT INSERT ANY MORE. \n\n";
					return *this; 
				}
			}
			
			if(binSetarr[i-1] < item){
				binSetarr[i] = item;
			}
			else{
				binSetarr[i] = binSetarr[i-1];
				binSetarr[i-1] = item;				
			}
		}
		return *this;	
    }
    virtual bin_search_simple_set<T>& operator-=(T item) { 
		int itempos;
		itempos = binsearch(binSetarr,0,nextern-1,item);
		binSetarr[itempos] = -1;
		return *this;
    }
    virtual bool contains(T item) {     	
		if(binsearch(binSetarr,0,nextern-1,item) != -1){
			return true;
		}
		else{
			return false;
		}    	
    }
    //returns position of element if it is found, or -1 if not found.
    //idea that helped me implement binary search from http://mathbits.com/MathBits/CompSci/Arrays/Binary.htm
    virtual int binsearch(int* x, int low, int high, int searchfor){
    	int middleofarray = (low+high)/2;
    	while((x[middleofarray] != searchfor) && (low <= high)){
    		if(x[middleofarray] > searchfor){
    			high=middleofarray-1;
    		}
    		else{
    			low=middleofarray+1;
    		}
    		middleofarray = (low+high)/2;
    	}
    	if (low <= high){
    		return middleofarray;
    	}
    	else{
    		return -1;
    	}    	
    }
};

//===============================================================
// RANGE SETS

// Function object for incrementing.
// Provides a default for any type that has an operator++.
//
template<class T>
class increment {
public:
    T operator()(T a) {
        // your code here
        return  a + 1;
    }
};

// Range type.  Uses comp<T> by default, but you can provide your
// own replacement if you want, e.g. for C strings.
//
template<class T, class C = comp<T> >
class range {
    T L;        // represents all elements from L, inclusive
    T H;        // through H, exclusive
    C cmp;
public:
    range(T l, T h) : L(l), H(h) {}     // constructor
    // no destructor needed
    T low() { return L; }
    T high() { return H; }
    bool contains(T item)
        { return cmp.precedes(item, H) && !cmp.precedes(item, L); }
};

// This is the abstract class from which all range-supporting sets are derived.
//
template<class T, class C = comp<T> >
class range_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
public:
    virtual range_set<T>& operator+=(range<T, C> r) = 0;
    virtual range_set<T>& operator-=(range<T, C> r) = 0;
};

//---------------------------------------------------------------

// As implemented in the STL, sets contain individual elements, not ranges.
// (There are range insert and erase operators, but (a) they use iterators,
// (b) they take time proportional to the number of elements in the range, and
// (c) they require, for deletion, that the endpoints of the range actually
// be in the set.  An stl_range_set, as defined here, avoids shortcomings
// (a) and (c), but not (b).  Your bin_search_range_set should avoid
// (b), though it will have slow insert and remove operations.  An
// tree_range_set (search tree -- extra credit) would have amortized
// log-time insert and remove for individual elements _and_ ranges.
//

template<class T, class C = comp<T>, class I = increment<T> >
class stl_range_set : public virtual range_set<T, C>, public stl_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    I inc;
public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in stl_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).
    virtual stl_simple_set<T>& operator+=(T item) {
        return stl_simple_set<T>::operator+=(item);
    }
    virtual stl_simple_set<T>& operator-=(T item) {
        return stl_simple_set<T>::operator-=(item);
    }
    virtual bool contains(T item) {
        return stl_simple_set<T>::contains(item);
    }
    virtual range_set<T>& operator+=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};

//---------------------------------------------------------------

// insert an appropriate carray_range_set declaration here

template<class T, class C = comp<T>, class I = increment<T> >
class carray_range_set: public virtual range_set<T, C>, public virtual carray_simple_set<T> {
    I inc;
public:
    // constructor
    carray_range_set(T l, T h) : range_set<T, C>(), carray_simple_set<T>(l, h)  {}	
    virtual carray_simple_set<T>& operator+=(T item) { 
        return carray_simple_set<T>::operator+=(item);
    }
    // sets the proper methods to use 
    virtual carray_simple_set<T>& operator-=(T item) {
        return carray_simple_set<T>::operator-=(item);
    }
    virtual bool contains(T item) {
        return carray_simple_set<T>::contains(item);
    }
    // Inserts and removes a set of values into the carray
    virtual range_set<T>& operator+=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

// insert an appropriate hashed_range_set declaration here
template<class T, class C = comp<T>, class I = increment<T> >
class hashed_range_set: public virtual range_set<T, C>, public virtual hashed_set<T> {
    I inc;
public:
    // constructor
    hashed_range_set(T n) : range_set<T, C>(), hashed_set<T>(n)  {}	
    // sets the proper methods to use 
    virtual hashed_set<T>& operator+=(T item) {
        return hashed_set<T>::operator+=(item);
    }
    virtual hashed_set<T>& operator-=(T item) {
        return hashed_set<T>::operator-=(item);
    }
    virtual bool contains(T item) {
        return hashed_set<T>::contains(item);
    }
    // Inserts and removes a set of values into the hashed set 
    virtual range_set<T>& operator+=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

//---------------------------------------------------------------

// insert an appropriate bin_search_range_set declaration here

template<class T, class C = comp<T>, class I = increment<T> >
class bin_search_range_set: public virtual range_set<T, C>, public virtual bin_search_simple_set<T> {
    I inc;
public:
    // constructor
     bin_search_range_set(T n) : range_set<T, C>(), bin_search_range_set<T>(n)  {}	
    // sets the proper methods to use
    virtual bin_search_simple_set<T>& operator+=(T item) {
        return bin_search_simple_set<T>::operator+=(item);
    }
    virtual bin_search_simple_set<T>& operator-=(T item) {
        return bin_search_simple_set<T>::operator-=(item);
    }
    virtual bool contains(T item) {
        return bin_search_simple_set<T>::contains(item);
    }
    // inserts a range of values into the bin_search set
    virtual range_set<T>& operator+=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
					try {	
            *this += i;
					} catch (const std::exception& e) {}
        }
        return *this;
    }
    virtual range_set<T>& operator-=(range<T, C> r) {
        for (T i = r.low(); r.contains(i); i = inc(i)) {
					try {
            *this -= i;
					} catch (const std::exception& e) {}
        }
        return *this;
    }
};
//===============================================================

// comparator for C strings
//
class lexico_less {
public:
    bool precedes(const char *a, const char *b) {
        return strcmp(a, b) < 0;
    }
    bool equals(const char *a, const char *b) {
        return strcmp(a, b) == 0;
    }
};

typedef enum{mon, tue, wed, thu, fri} weekday;

