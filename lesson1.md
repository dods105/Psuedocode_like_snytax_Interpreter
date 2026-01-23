# Lesson 2

## The Lexer (Tokenizer)

### Covered in this lesson:

-   What tokens are\
-   Token types\
-   Building a simple lexer in C++

------------------------------------------------------------------------

## What is a TOKEN?

Before the interpreter understands the code, it breaks text into pieces
called tokens.\
This process is called **lexical analysis** or **tokenizing**.

**Input:**

    LET x = 5 + 3

**Your interpreter sees:**

    [LET] [x] [=] [5] [+] [3]

These things are called **TOKENS**.

------------------------------------------------------------------------

## What is a Token Type?

  Token   Type
  ------- ------------
  LET     KEYWORD
  X       IDENTIFIER
  =       EQUAL
  5       NUMBER
  \+      PLUS
  3       NUMBER

------------------------------------------------------------------------

## Building a Lexer

### Libraries

``` cpp
#include <string>
#include <iostream>
#include <vector>
#include <cctype>
#include <unordered_set>
```

------------------------------------------------------------------------

## Step 1 -- Token Types

``` cpp
enum class TokenType{
    NUMBER,
    IDENTIFIER,
    KEYWORD,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUAL,
    LPAREN,
    RPAREN,
    SEMICOLON,
    END
};
```

------------------------------------------------------------------------

## Step 2 -- Token Structure

``` cpp
struct Token{
    TokenType type;
    std::string value;
};
```

------------------------------------------------------------------------

## Step 3 -- Lexer Class

``` cpp
class Lexer{
    std::string text;
    int pos = 0;
    char current;

public:
    Lexer(std::string src): text(src){
        current = text[pos];
    }

    void advance();
    void skipWhiteSpace();
    Token number();
    Token identifier();
    Token getNextToken();
    char peek();
};
```

------------------------------------------------------------------------

## Step 4 -- Running the Lexer

``` cpp
int main(){
    Lexer lexer("PRINT x x = 20.5 + 25");

    Token t;
    do{
        t = lexer.getNextToken();
        std::cout << "Token " << (int)t.type << " : "
                  << tokenNames(t.type) << " -> "
                  << t.value << "\n";
    }while(t.type != TokenType::END);
}
```
