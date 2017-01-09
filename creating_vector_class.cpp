/*
 Lucas O'Rourke
 lor215
 hw04a.cpp
 
 Problem 1 is creating the vector class. Problem 2 and 3 are an introduction to recursion
 */

#include <iostream>
#include <vector>
using namespace std;

template <typename Object>
class Vector
{
public:
    explicit Vector( int initSize = 0 )
    : theSize{ initSize }, theCapacity{ initSize + SPARE_CAPACITY }
    { objects = new Object[ theCapacity ]; }
    
    Vector( const Vector & rhs )
    : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ nullptr }
    {
        objects = new Object[ theCapacity ];
        for( int k = 0; k < theSize; ++k )
            objects[ k ] = rhs.objects[ k ];
    }
    
    Vector & operator= ( const Vector & rhs )
    {
        Vector copy = rhs;
        std::swap( *this, copy );
        return *this;
    }
    
    ~Vector( )
    { delete [ ] objects; }
    
    Vector( Vector && rhs )
    : theSize{ rhs.theSize }, theCapacity{ rhs.theCapacity }, objects{ rhs.objects }
    {
        rhs.objects = nullptr;
        rhs.theSize = 0;
        rhs.theCapacity = 0;
    }
    
    Vector & operator= ( Vector && rhs )
    {
        std::swap( theSize, rhs.theSize );
        std::swap( theCapacity, rhs.theCapacity );
        std::swap( objects, rhs.objects );
        
        return *this;
    }
    
    bool empty( ) const
    { return size( ) == 0; }
    int size( ) const
    { return theSize; }
    int capacity( ) const
    { return theCapacity; }
    
    Object & operator[]( int index )
    {
        return objects[ index ];
    }
    
    const Object & operator[]( int index ) const
    {
        return objects[ index ];
    }
    
    void resize( int newSize )
    {
        if( newSize > theCapacity )
            reserve( newSize * 2 );
        theSize = newSize;
    }
    
    void reserve( int newCapacity )
    {
        if( newCapacity < theSize )
            return;
        
        Object *newArray = new Object[ newCapacity ];
        for( int k = 0; k < theSize; ++k )
            newArray[ k ] = std::move( objects[ k ] );
        
        theCapacity = newCapacity;
        std::swap( objects, newArray );
        delete [ ] newArray;
    }
    
    // Stacky stuff
    void push_back( const Object & x )
    {
        if( theSize == theCapacity )
            reserve( 2 * theCapacity + 1 );
        objects[ theSize++ ] = x;
    }
    // Stacky stuff
    void push_back( Object && x )
    {
        if( theSize == theCapacity )
            reserve( 2 * theCapacity + 1 );
        objects[ theSize++ ] = std::move( x );
    }
    
    
    
    
    // Iterator stuff: not bounds checked
    typedef Object * iterator;
    typedef const Object * const_iterator;
    iterator begin( )
    { return &objects[ 0 ]; }
    const_iterator begin( ) const
    { return &objects[ 0 ]; }
    iterator end( )
    { return &objects[ size( ) ]; }
    const_iterator end( ) const
    { return &objects[ size( ) ]; }
    
    
    //Programming number 1
    iterator erase( iterator vItr) {
        for (iterator itr = vItr; itr != end(); itr++) {
            *itr = *(itr+1);
        }
        resize(size()-1);
        return vItr;
    }
    
    static const int SPARE_CAPACITY = 2;
    
private:
    int theSize;
    int theCapacity;
    Object * objects;
};



//Programming number 2
int sumDigits(int x) {
    if (x == 0) {
        return 0;
    }
    else {
        return x%10 + sumDigits(x/10);
    }
}


//Programming number 3
template <class Iterator>
int recursiveFunc(Iterator start, Iterator end) {
    if (start == end - 1) {
        return *start;
    }
    Iterator mid = start + ((end-start)/2);
    return recursiveFunc(start, mid) + recursiveFunc(mid, end);
}

int sumVector(const vector<int> & a) {
    return recursiveFunc(a.begin(), a.end());
}



int main() {
    vector<int> v = {1, 2, 3, 4, 5};
    vector<int>::iterator itr = v.begin() + 1;
    v.erase(itr);
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
    }
    cout << "\nsum of the digits of " << 519 << " is " << sumDigits(519) << endl;
    vector<int> q = {-21, 31, 14, 1, -20};
    cout << "The sum of {−21, 31, 14, 1, −20} is " << sumVector(q) << endl;
}
