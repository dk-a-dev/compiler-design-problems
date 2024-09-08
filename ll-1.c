// expr   -> term expr'
// expr'  -> '+' term expr' | '-' term expr' | ε
// term   -> factor term'
// term'  -> '*' factor term' | '/' factor term' | ε
// factor -> NUMBER | '(' expr ')
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

typedef struct {
    const char *text;
    size_t pos;
    Token current_token;
} Lexer;

Token get_next_token(Lexer *lexer) {
    while (lexer->text[lexer->pos] != '\0') {
        if (isspace(lexer->text[lexer->pos])) {
            lexer->pos++;
            continue;
        }

        if (isdigit(lexer->text[lexer->pos])) {
            int value = 0;
            while (isdigit(lexer->text[lexer->pos])) {
                value = value * 10 + (lexer->text[lexer->pos] - '0');
                lexer->pos++;
            }
            return (Token){TOKEN_NUMBER, value};
        }

        if (lexer->text[lexer->pos] == '+') {
            lexer->pos++;
            return (Token){TOKEN_PLUS, 0};
        }

        if (lexer->text[lexer->pos] == '-') {
            lexer->pos++;
            return (Token){TOKEN_MINUS, 0};
        }

        if (lexer->text[lexer->pos] == '*') {
            lexer->pos++;
            return (Token){TOKEN_MULTIPLY, 0};
        }

        if (lexer->text[lexer->pos] == '/') {
            lexer->pos++;
            return (Token){TOKEN_DIVIDE, 0};
        }

        if (lexer->text[lexer->pos] == '(') {
            lexer->pos++;
            return (Token){TOKEN_LPAREN, 0};
        }

        if (lexer->text[lexer->pos] == ')') {
            lexer->pos++;
            return (Token){TOKEN_RPAREN, 0};
        }

        fprintf(stderr, "Error: Unknown character '%c'\n", lexer->text[lexer->pos]);
        exit(EXIT_FAILURE);
    }

    return (Token){TOKEN_EOF, 0};
}

typedef struct {
    Lexer lexer;
} Parser;

void error() {
    fprintf(stderr, "Error parsing input\n");
    exit(EXIT_FAILURE);
}

void eat(Parser *parser, TokenType token_type) {
    if (parser->lexer.current_token.type == token_type) {
        parser->lexer.current_token = get_next_token(&parser->lexer);
    } else {
        error();
    }
}

int expr(Parser *parser);
int expr_prime(Parser *parser, int inherited_value);
int term(Parser *parser);
int term_prime(Parser *parser, int inherited_value);
int factor(Parser *parser);

int expr(Parser *parser) {
    int result = term(parser);
    return expr_prime(parser, result);
}

int expr_prime(Parser *parser, int inherited_value) {
    if (parser->lexer.current_token.type == TOKEN_PLUS) {
        eat(parser, TOKEN_PLUS);
        int term_value = term(parser);
        return expr_prime(parser, inherited_value + term_value);
    } else if (parser->lexer.current_token.type == TOKEN_MINUS) {
        eat(parser, TOKEN_MINUS);
        int term_value = term(parser);
        return expr_prime(parser, inherited_value - term_value);
    } else {
        return inherited_value;
    }
}

int term(Parser *parser) {
    int result = factor(parser);
    return term_prime(parser, result);
}

int term_prime(Parser *parser, int inherited_value) {
    if (parser->lexer.current_token.type == TOKEN_MULTIPLY) {
        eat(parser, TOKEN_MULTIPLY);
        int factor_value = factor(parser);
        return term_prime(parser, inherited_value * factor_value);
    } else if (parser->lexer.current_token.type == TOKEN_DIVIDE) {
        eat(parser, TOKEN_DIVIDE);
        int factor_value = factor(parser);
        return term_prime(parser, inherited_value / factor_value);
    } else {
        return inherited_value;
    }
}

int factor(Parser *parser) {
    Token token = parser->lexer.current_token;
    if (token.type == TOKEN_NUMBER) {
        eat(parser, TOKEN_NUMBER);
        return token.value;
    } else if (token.type == TOKEN_LPAREN) {
        eat(parser, TOKEN_LPAREN);
        int result = expr(parser);
        eat(parser, TOKEN_RPAREN);
        return result;
    } else {
        error();
        return 0;
    }
}

void run_test(const char *text) {
    Lexer lexer = {text, 0, get_next_token(&lexer)};
    Parser parser = {lexer};

    parser.lexer.current_token = get_next_token(&parser.lexer);
    int result = expr(&parser);

    printf("Expression: %s\nResult: %d\n\n", text, result);
}

int main() {
    const char *test_cases[] = {
        "3 + 5 * (10 - 4)",
        "10 + 2 * 6",
        "100 * 2 + 12",
        "100 * ( 2 + 12 )",
        "100 * ( 2 + 12 ) / 14",
        "7 - 3 + 2",
        "7 * (3 + 2) - 5",
        "42",
        "(1 + 2) * (3 + 4)",
        "10 / 2 + 3 * 4"
    };

    for (int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        run_test(test_cases[i]);
    }

    return 0;
}