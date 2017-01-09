/*
 Lucas O'Rourke
 lor215
 hw08_1.cpp
 */


#include <iostream>
using namespace std;

// This is code from Weiss
#define CATCH_CONFIG_MAIN
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include "catch.hpp"
#include <sstream>
using namespace std;

class Tokenizer
{
public:
    Tokenizer( istream & input )
    : currentLine( 1 ), errors( 0 ), inputStream( input ) { }
    
    // The public routines
    char getNextOpenClose( );
    string getNextID( );
    int getLineNumber( ) const;
    int getErrorCount( ) const;
    
private:
    enum CommentType { SLASH_SLASH, SLASH_STAR };
    
    istream & inputStream;     // Reference to the input stream
    char ch;                   // Current character
    int currentLine;           // Current line
    int errors;                // Number of errors detected
    
    // A host of internal routines
    bool nextChar( );
    void putBackChar( );
    void skipComment( CommentType start );
    void skipQuote( char quoteType );
    string getRemainingString( );
};


// nextChar sets ch based on the next character in
// inputStream and adjusts currentLine if necessary.
// It returns the result of get.
// putBackChar puts the character back onto inputStream.
// Both routines adjust currentLine if necessary.
bool Tokenizer::nextChar( )
{
    if( !inputStream.get( ch ) )
        return false;
    if( ch == '\n' )
        currentLine++;
    return true;
}

void Tokenizer::putBackChar( )
{
    inputStream.putback( ch );
    if( ch == '\n' )
        currentLine--;
}

// Precondition: We are about to process a comment;
//                 have already seen comment start token.
// Postcondition: Stream will be set immediately after
//                 comment ending token.
void Tokenizer::skipComment( CommentType start )
{
    if( start == SLASH_SLASH )
    {
        while( nextChar( ) && ( ch != '\n' ) )
            ;
        return;
    }
    
    // Look for */
    bool state = false;    // Seen first char in comment ender.
    
    while( nextChar( ) )
    {
        if( state && ch == '/' )
            return;
        state = ( ch == '*' );
    }
    cout << "Unterminated comment at line " << getLineNumber( ) << endl;
    errors++;
}

// Precondition: We are about to process a quote;
//                   have already seen beginning quote.
// Postcondition: Stream will be set immediately after
//                   matching quote.
void Tokenizer::skipQuote( char quoteType )
{
    while( nextChar( ) )
    {
        if( ch == quoteType )
            return;
        if( ch == '\n' )
        {
            cout << "Missing closed quote at line " << ( getLineNumber( ) - 1 ) << endl;
            errors++;
            return;
        }
        // If a backslash, skip next character.
        else if( ch == '\\' )
            nextChar( );
    }
}

// Return the next opening or closing symbol or '\0' (if EOF).
// Skip past comments and character and string constants.
char Tokenizer::getNextOpenClose( )
{
    while( nextChar( ) )
    {
        if( ch == '/' )
        {
            if( nextChar( ) )
            {
                if( ch == '*' )
                    skipComment( SLASH_STAR );
                else if( ch == '/' )
                    skipComment( SLASH_SLASH );
                else if( ch != '\n' )
                    putBackChar( );
            }
        }
        else if( ch == '\'' || ch == '"' )
            skipQuote( ch );
        else if( ch == '\\' )  // Extra case, not in text
            nextChar( );
        else if( ch == '(' || ch == '[' || ch == '{' ||
                ch == ')' || ch == ']' || ch == '}' )
            return ch;
    }
    return '\0';       // End of file
}

// Return current line number.
int Tokenizer::getLineNumber( ) const
{
    return currentLine;
}

// Return current line number.
int Tokenizer::getErrorCount( ) const
{
    return errors;
}

// Return indicates if ch can be part of a C++ identifier.
bool isIdChar( char ch )
{
    return ch == '_' || isalnum( ch );
}

// Return an identifier read from input stream.
// First character is already read into ch.
string Tokenizer::getRemainingString( )
{
    string result;
    
    for( result = ch; nextChar( ); result += ch )
        if( !isIdChar( ch ) )
        {
            putBackChar( );
            break;
        }
    
    return result;
}

// Return next identifier, skipping comments
// string constants, and character constants.
// Return "" if end of stream is reached.
string Tokenizer::getNextID( )
{
    while( nextChar( ) )
    {
        if( ch == '/' )
        {
            if( nextChar( ) )
            {
                if( ch == '*' )
                    skipComment( SLASH_STAR );
                else if( ch == '/' )
                    skipComment( SLASH_SLASH );
                else
                    putBackChar( );
            }
        }
        else if( ch == '\\' )
            nextChar( );
        else if( ch == '\'' || ch == '"' )
            skipQuote( ch );
        else if( !isdigit( ch ) && isIdChar( ch ) )
            return getRemainingString( );
    }
    return "";       // End of file
}



// Code from Weiss
// Symbol is the class that will be placed on the Stack.
struct Symbol
{
    char token;
    int  theLine;
};

class Balance
{
public:
    Balance( istream & is ):tok( is ),errors( 0 ){}
    
    
    int checkBalance() { // returns the number of mismatched parenthesis
        stack<Symbol> s;
        int lineNum;
        char check = tok.getNextOpenClose();
        Symbol sym;
        while (check != '\0') {
            if (check == '(' || check == '{' || check == '[') {
                lineNum = tok.getLineNumber();
                sym.token = check;
                sym.theLine = lineNum;
                s.push(sym);
                check = tok.getNextOpenClose();
            }
            else if (check == ')' || check == '}' || check == ']') {
                lineNum = tok.getLineNumber();
                sym.token = check;
                sym.theLine = lineNum;
                if (s.size() == 0) {
                    cout << "Error: Expected open " << check << " in line " << lineNum << endl;
                    errors += 1;
                }
                while (s.size() != 0) {
                    int pre_err = errors;
                    checkMatch(s.top(), sym);
                    s.pop();
                    if (pre_err == errors) { //only pops one item if there is no error in the checkMatch
                        break;
                    }
                }
                check = tok.getNextOpenClose();
            }
        }
        
        if (s.size() != 0) { //If there are still brackets on the stack
            while (s.size() != 0) {
                Symbol err_sym;
                err_sym.token = ' ';
                err_sym.theLine = 0;
                checkMatch(s.top(), err_sym);
                s.pop();
            }
        }
        return errors;
    }
private:
    Tokenizer tok;
    int errors;
    
    //Check Match function updates errors
    void checkMatch( const Symbol & oParen, const Symbol & cParen ) {
        if (oParen.token == '(' && cParen.token != ')') {
            cout << "Error: Expected closed ) from line " << oParen.theLine << endl;
            errors += 1;
        }
        else if (oParen.token == '{' && cParen.token != '}') {
            cout << "Error: Expected closed } from line " << oParen.theLine << endl;
            errors += 1;
        }
        else if (oParen.token == '[' && cParen.token != ']') {
            cout << "Error: Expected closed ] from line " << oParen.theLine << endl;
            errors += 1;
        }
    }
};



SCENARIO("Checking your Balancer"){
    GIVEN("a test file test.cc with 1 known issues taken in as a ifstream"){
        ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw08/hw08/test.cc");
        if (!ifs) cerr << "Failure to open file" << endl;
        cerr << "test.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("CheckBalance should return 1"){
                REQUIRE(b.checkBalance() == 1);
            }
        }
        ifs.close();
    }
    GIVEN("a test file test2.cc with 2 known issues taken in as a ifstream"){
        ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw08/hw08/test2.cc");
        if(!ifs) cerr << "Failure to open file" << endl;
        cerr << "test2.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("Check balance should return 2"){
                REQUIRE(b.checkBalance() == 2);
            }
        }
    }
    GIVEN("a test file test3.cc with 0 known issues taken in as a ifstream"){
        ifstream ifs("/Users/lucasorourke/Desktop/Data Structures/hw08/hw08/test3.cc");
        if(!ifs) cerr << "Failure to open file" << endl;
        cerr << "test3.cc" << endl;
        WHEN("Given a balancer"){
            Balance b(ifs);
            THEN("Check balance should return 0"){
                REQUIRE(b.checkBalance() == 0);
            }
        }
    }
}


