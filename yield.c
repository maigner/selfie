//
//  yield.c
//  selfie-fork
//
//  Created by Martin Aigner on 23/10/15.
//  Copyright © 2015 Martin Aigner. All rights reserved.
//


// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~
// -----------------------------------------------------------------
// ---------------------     L I B R A R Y     ---------------------
// -----------------------------------------------------------------
// *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~ *~*~

// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

void initLibrary();

int twoToThePowerOf(int p);
int leftShift(int n, int b);
int rightShift(int n, int b);

int  stringLength(int *s);
void stringReverse(int *s);
int  stringCompare(int *s, int *t);

int  atoi(int *s);
int* itoa(int n, int *s, int b, int a);

void print(int *s);
void println();

void printCharacter(int character);
void printString(int *s);

void exit(int code);
int* malloc(int size);

// ------------------------ GLOBAL CONSTANTS -----------------------

int *power_of_two_table;

int INT_MAX; // maximum numerical value of an integer
int INT_MIN; // minimum numerical value of an integer

int CHAR_EOF;
int CHAR_TAB;
int CHAR_LF;
int CHAR_CR;
int CHAR_SPACE;        //
int CHAR_SEMICOLON;    // ;
int CHAR_PLUS;         // +
int CHAR_DASH;         // -
int CHAR_ASTERISK;     // *
int CHAR_HASH;         // #
int CHAR_SLASH;        // /
int CHAR_UNDERSCORE;   // _
int CHAR_EQUAL;        // =
int CHAR_LPARENTHESIS; // (
int CHAR_RPARENTHESIS; // )
int CHAR_LBRACE;       // {
int CHAR_RBRACE;       // }
int CHAR_COMMA;        // ,
int CHAR_LT;           // <
int CHAR_GT;           // >
int CHAR_EXCLAMATION;  // !
int CHAR_PERCENTAGE;   // %
int CHAR_SINGLEQUOTE;  // '
int CHAR_DOUBLEQUOTE;  // "

int *string_buffer;

// ------------------------- INITIALIZATION ------------------------

void initLibrary() {
    int i;
    
    power_of_two_table = malloc(31*4);
    
    *power_of_two_table = 1; // 2^0
    
    i = 1;
    
    while (i < 31) {
        *(power_of_two_table + i) = *(power_of_two_table + (i - 1)) * 2;
        
        i = i + 1;
    }
    
    // computing INT_MAX and INT_MIN without overflows
    INT_MAX = (twoToThePowerOf(30) - 1) * 2 + 1;
    INT_MIN = -INT_MAX - 1;
    
    CHAR_EOF          = -1; // end of file
    CHAR_TAB          = 9;  // ASCII code 9  = tabulator
    CHAR_LF           = 10; // ASCII code 10 = line feed
    CHAR_CR           = 13; // ASCII code 13 = carriage return
    CHAR_SPACE        = ' ';
    CHAR_SEMICOLON    = ';';
    CHAR_PLUS         = '+';
    CHAR_DASH         = '-';
    CHAR_ASTERISK     = '*';
    CHAR_HASH         = '#';
    CHAR_SLASH        = '/';
    CHAR_UNDERSCORE   = '_';
    CHAR_EQUAL        = '=';
    CHAR_LPARENTHESIS = '(';
    CHAR_RPARENTHESIS = ')';
    CHAR_LBRACE       = '{';
    CHAR_RBRACE       = '}';
    CHAR_COMMA        = ',';
    CHAR_LT           = '<';
    CHAR_GT           = '>';
    CHAR_EXCLAMATION  = '!';
    CHAR_PERCENTAGE   = '%';
    CHAR_SINGLEQUOTE  = 39; // ASCII code 39 = '
    CHAR_DOUBLEQUOTE  = '"';
    
    // accommodate 32-bit numbers for itoa
    string_buffer = malloc(33);
}


// -----------------------------------------------------------------
// ----------------------- LIBRARY FUNCTIONS -----------------------
// -----------------------------------------------------------------

int twoToThePowerOf(int p) {
    // assert: 0 <= p < 31
    return *(power_of_two_table + p);
}

int leftShift(int n, int b) {
    // assert: b >= 0;
    
    if (b > 30)
        return 0;
    else
        return n * twoToThePowerOf(b);
}

int rightShift(int n, int b) {
    // assert: b >= 0
    
    if (b > 30)
        return 0;
    else if (n >= 0)
        return n / twoToThePowerOf(b);
    else
        // works even if n == INT_MIN
        return ((n + 1) + INT_MAX) / twoToThePowerOf(b) +
        (INT_MAX / twoToThePowerOf(b) + 1);
}

int getCharacter(int *s, int i) {
    // assert: i >= 0
    int a;
    
    a = i / 4;
    
    return rightShift(leftShift(*(s + a), 24 - (i % 4) * 8), 24);
}

int* putCharacter(int *s, int i, int c) {
    // assert: i >= 0, all characters are 7-bit
    int a;
    
    a = i / 4;
    
    *(s + a) = (*(s + a) - leftShift(getCharacter(s, i), (i % 4) * 8)) + leftShift(c, (i % 4) * 8);
    
    return s;
}

int stringLength(int *s) {
    int i;
    
    i = 0;
    
    while (getCharacter(s, i) != 0)
        i = i + 1;
    
    return i;
}

void stringReverse(int *s) {
    int i;
    int j;
    int tmp;
    
    i = 0;
    j = stringLength(s) - 1;
    
    while (i < j) {
        tmp = getCharacter(s, i);
        
        putCharacter(s, i, getCharacter(s, j));
        putCharacter(s, j, tmp);
        
        i = i + 1;
        j = j - 1;
    }
}

int stringCompare(int *s, int *t) {
    int i;
    
    i = 0;
    
    while (1)
        if (getCharacter(s, i) == 0)
            if (getCharacter(t, i) == 0)
                return 1;
            else
                return 0;
            else if (getCharacter(s, i) == getCharacter(t, i))
                i = i + 1;
            else
                return 0;
}

int atoi(int *s) {
    int i;
    int n;
    
    i = 0;
    
    n = 0;
    
    while (getCharacter(s, i) != 0) {
        n = n * 10 + getCharacter(s, i) - '0';
        
        i = i + 1;
    }
    
    return n;
}

int* itoa(int n, int *s, int b, int a) {
    // assert: b in {2,4,8,10,16}
    
    int i;
    int sign;
    
    i = 0;
    
    sign = 0;
    
    if (n == 0) {
        putCharacter(s, 0, '0');
        
        i = 1;
    } else if (n < 0) {
        sign = 1;
        
        if (b == 10) {
            if (n == INT_MIN) {
                // rightmost decimal digit of 32-bit INT_MIN
                putCharacter(s, 0, '8');
                
                n = -(n / 10);
                i = i + 1;
            } else
                n = -n;
        } else {
            if (n == INT_MIN) {
                // rightmost non-decimal digit of INT_MIN
                putCharacter(s, 0, '0');
                
                n = (rightShift(INT_MIN, 1) / b) * 2;
                i = i + 1;
            } else
                n = rightShift(leftShift(n, 1), 1);
        }
    }
    
    while (n != 0) {
        if (n % b > 9)
            putCharacter(s, i, n % b - 10 + 'A');
        else
            putCharacter(s, i, n % b + '0');
        
        n = n / b;
        i = i + 1;
        
        if (i == 1) {
            if (sign) {
                if (b != 10)
                    n = n + (rightShift(INT_MIN, 1) / b) * 2;
            }
        }
    }
    
    if (b != 10) {
        while (i < a) {
            putCharacter(s, i, '0'); // align with zeros
            
            i = i + 1;
        }
        
        if (b == 16) {
            putCharacter(s, i, 'x');
            putCharacter(s, i + 1, '0');
            
            i = i + 2;
        }
    } else if (sign) {
        putCharacter(s, i, '-');
        
        i = i + 1;
    }
    
    putCharacter(s, i, 0); // null terminated string
    
    stringReverse(s);
    
    return s;
}

void print(int *s) {
    int i;
    
    i = 0;
    
    while (getCharacter(s, i) != 0) {
        putchar(getCharacter(s, i));
        
        i = i + 1;
    }
}

void println() {
    putchar(CHAR_LF);
}

void printCharacter(int character) {
    putchar(CHAR_SINGLEQUOTE);
    
    if (character == CHAR_EOF)
        print((int*) "end of file");
    else if (character == CHAR_TAB)
        print((int*) "tabulator");
    else if (character == CHAR_LF)
        print((int*) "line feed");
    else if (character == CHAR_CR)
        print((int*) "carriage return");
    else
        putchar(character);
    
    putchar(CHAR_SINGLEQUOTE);
}

void printString(int *s) {
    putchar(CHAR_DOUBLEQUOTE);
    
    print(s);
    
    putchar(CHAR_DOUBLEQUOTE);
}





int main(int argc, int *argv) {
    
    print((int*) "Hello");
    println();

    

    return 0;
}
