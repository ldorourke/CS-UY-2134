/*
 Lucas O'Rourke
 lor215
 hw07_3_xc.cpp
 
 Creating a doubly linked list
 */

#include <iostream>
using namespace std;

template <typename Object>
class DList
{
private:
    // The basic doubly linked list node.
    // Nested inside of List, can be public
    // because the Node is itself private
    struct Node
    {
        Object  data;
        Node   *prev;
        Node   *next;
        
        Node( const Object & d = Object{ }, Node * p = nullptr, Node * n = nullptr )
        : data{ d }, prev{ p }, next{ n } { }
        
        Node( Object && d, Node * p = nullptr, Node * n = nullptr )
        : data{ std::move( d ) }, prev{ p }, next{ n } { }
    };
    
    class iterator
    {
    public:
        
        iterator( ):current(nullptr)
        { }
        
        Object & operator* ( )
        { return current->data; }
        
        iterator & operator++ ( )
        {
            this->current = this->current->next;
            return *this;
        }
        
        iterator operator++ ( int )
        {
            iterator old = *this;
            ++( *this );
            return old;
        }
        
        iterator & operator-- ( )
        {
            this->current = this->current->prev;
            return *this;
        }
        
        iterator operator-- ( int )
        {
            iterator old = *this;
            --( *this );
            return old;
        }
        
    private:
        Node *current;
        // Protected constructor for iterator.
        // Expects the current position.
        iterator( Node *p ) : current{ p }
        { }
        
        friend class DList<Object>;
    };
    
public:
    DList( )
    { init( ); }
    
    ~DList( )
    {
        clear( );
        delete head;
        delete tail;
    }
    
    // Return iterator representing beginning of list.
    iterator begin( )
    { return iterator( head->next ); }
    
    
    // Return iterator representing endmarker of list.
    iterator end( )
    { return iterator( tail ); }
    
    // Return number of elements currently in the list.
    int size( ) const
    { return theSize; }
    
    // Return true if the list is empty, false otherwise.
    bool empty( ) const
    { return size( ) == 0; }
    
    void clear( )
    {
        while( !empty( ) )
        {
            Node* deleteNode = head->next;
            head->next = deleteNode->next;
            deleteNode->next->prev = head;
            --theSize;
        }
    }
    
    // front, back, push_front, push_back, pop_front, and pop_back
    // are the basic double-ended queue operations.
    Object & front( )
    { return *begin( ); }
    
    const Object & front( ) const
    { return *begin( ); }
    
    Object & back( )
    { return *--end( ); }
    
    const Object & back( ) const
    { return *--end( ); }
    
    void push_front( const Object & x )
    { insert( begin( ), x ); }
    
    void push_back( const Object & x )
    { insert( end( ), x ); }
    
    void push_front( Object && x )
    { insert( begin( ), std::move( x ) ); }
    
    void push_back( Object && x )
    { insert( end( ), std::move( x ) ); }
    
    // Insert x before itr.
    iterator insert( iterator itr, const Object & x )
    {
        Node *p = itr.current;
        ++theSize;
        return iterator( p->prev = p->prev->next = new Node{ x, p->prev, p } );
    }
    
    // Insert x before itr.
    iterator insert( iterator itr, Object && x )
    {
        Node *p = itr.current;
        ++theSize;
        return iterator( p->prev = p->prev->next = new Node{ std::move( x ), p->prev, p } );
    }
    
    void print() {
        Node* temp = head->next;
        while (temp->next != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
    
    //Extra Credit - Remove
    void remove(const Object& x) {
        Node* tmp = head;
        while (tmp != tail) {
            if (tmp->next->data == x) {
                Node* tmp_nxt = tmp->next->next;
                tmp_nxt->prev = tmp;
                Node* del = tmp->next;
                tmp->next = tmp_nxt;
                delete del;
                --theSize;
            }
            else {
                tmp = tmp->next;
            }
        }
    }
    
private:
    int   theSize;
    Node *head;
    Node *tail;
    
    void init( )
    {
        theSize = 0;
        head = new Node;
        tail = new Node;
        head->next = tail;
        tail->prev = head;
    }
};


int main() {
    DList<int> dl;
    dl.push_front(5);
    dl.push_front(4);
    dl.push_front(3);
    dl.push_front(2);
    dl.push_front(1);
    cout << "The list before removing a 3 is: ";
    dl.print();
    dl.remove(3);
    cout << "The list after removing a 3 is: ";
    dl.print();
    
}
