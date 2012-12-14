/*
    Starter code for assignment 6, CSC 2/454, Fall 2012

    Provides skeleton of code for a simple hierarchy of set
    abstractions.

    Everything but /main/ should be moved to a .h file, which should
    then be #included from here.
*/
#include "sets.h"
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
	
    //testing for simple sets to make sure they work.
    simple_set<weekday>* V = new carray_simple_set<weekday>(mon, (weekday)5);
    simple_set<weekday>* Z = new hashed_set<weekday>(3);
    simple_set<weekday>* W = new bin_search_simple_set<weekday>(3);
		
    *V += wed;
    *Z += thu;
    *Z += tue;
    *Z += fri;
    *Z += mon;
	*W += fri;
	*W += wed;
	*W += tue;
	*W += mon;

    if(V->contains(wed)) cout << "WEDNESDAY\n";
    if(Z->contains(fri)) cout << "THURSDAY AND MONDAY\n";
    if(W->contains(fri)) cout << "FRIDAY IN W\n"; 
		if(!(W->contains(mon))) cout << "WORKS\n";
    
	//range set testing
		range_set<int>* E = new carray_range_set<int>(1, 5);
		*E += range<int>(3,5);
		if (E->contains(1)) cout << "1 is in E\n";
		if (E->contains(4)) cout << "4 is in E\n";
		*E -= range<int>(4,5);
		
		range_set<int>* H = new hashed_range_set<int>(3);
		*H += range<int>(1,3);
		if (H->contains(2)) cout << "2 is in H";

		if (!(E->contains(4))) cout << "4 is NOT in E\n";

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

