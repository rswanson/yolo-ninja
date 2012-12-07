/*
    Starter code for assignment 6, CSC 2/454, Fall 2012

    Provides skeleton of code for a simple hierarchy of set
    abstractions.

    Everything but /main/ should be moved to a .h file, which should
    then be #included from here.
*/

#include <set>
#include <iostream>
#include <string.h>
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
public:
    // fill in these methods:
    carray_simple_set(T l, T h) { }     // constructor
    virtual ~carray_simple_set() { }
    virtual carray_simple_set<T>& operator+=(T item) { }
    virtual carray_simple_set<T>& operator-=(T item) { }
    virtual bool contains(T item) { }
};

//---------------------------------------------------------------

// Naive hash function object.
// Provides a default for any type that can be cast to int.
//
template<class T>
class cast_to_int {
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
public:
    hashed_set(int n) { }
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
public:
    // and some methods
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

//---------------------------------------------------------------

// insert an appropriate hashed_range_set declaration here

//---------------------------------------------------------------

// insert an appropriate bin_search_range_set declaration here

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

// Some miscellaneous code to get you started on testing your sets.
// (These tests all worked until I gutted the code.)
//
int main() {

    stl_range_set<double> Q;
    Q += 3.0;
    Q += range<double>(1.23, 2.34);

    stl_simple_set<int> R;
    R += 3;

    simple_set<double>* S = new stl_simple_set<double>();
    *S += 3.14;
    if (S->contains(3.14)) cout << "pi is in S\n";
    if (!S->contains(2.718)) cout << "e is not in S\n";

    simple_set<weekday>* V = new carray_simple_set<weekday>(mon, (weekday)5);
    *V += wed;

    stl_simple_set<string> U;
    U += "hello";
    if (U.contains("hello")) cout << "hello is in U\n";
    if (!U.contains("foo")) cout << "foo is not in U\n";

    range<string> r1("a", "f");
    if (r1.contains("b")) cout << "b is in r1\n";
    if (r1.contains("aaa")) cout << "aaa is in r1\n";
    if (!r1.contains("faa")) cout << "faa is not in r1\n";

    range<const char*, lexico_less> r2("a", "f");
    if (r2.contains("b")) cout << "b is in r2\n";
    if (r2.contains("aaa")) cout << "aaa is in r2\n";
    if (!r2.contains("faa")) cout << "faa is not in r2\n";

    range_set<int>* X = new stl_range_set<int>();
    *X += range<int>(5, 8);
    if (X->contains(4)) cout << "4 is in X\n";
    if (X->contains(5)) cout << "5 is in X\n";      // should print
    if (X->contains(6)) cout << "6 is in X\n";      // should print
    if (X->contains(7)) cout << "7 is in X\n";      // should print
    if (X->contains(8)) cout << "8 is in X\n";
    if (X->contains(9)) cout << "9 is in X\n";
    *X -= range<int>(6, 10);
    if (X->contains(4)) cout << "4 is now in X\n";
    if (X->contains(5)) cout << "5 is now in X\n";      // should print
    if (X->contains(6)) cout << "6 is now in X\n";
    if (X->contains(7)) cout << "7 is now in X\n";
    if (X->contains(8)) cout << "8 is now in X\n";
    if (X->contains(9)) cout << "9 is now in X\n";
}

