#include "scanner.h"

Scanner scanner;

void init_scanner(const char *source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static Token make_token(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token error_token(const char *msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg;
    token.length = (int)strlen(msg);
    token.line = scanner.line;
    return token;
}

static bool at_eof(void) { return *scanner.current == '\0'; }

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static bool is_alphanumeric(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_identifier_char(char c) {
    return is_alphanumeric(c) || is_digit(c) || c == '-';
}

static char peek(void) { return *scanner.current; }

static char peek_next(void) {
    if (at_eof()) return '\0';
    return scanner.current[1];
}

static char consume(void) {
    // TODO can write as one expression?
    // return *(scanner.current++);
    scanner.current++;
    return scanner.current[-1];
}

static bool match_and_consume(char expected) {
    // Consumes only if the character matches
    if (at_eof()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static void consume_whitespace(void) {
    while (true) {
        switch (peek()) {
        case '\n':
            scanner.line++;
            consume();
            break;
        case ' ':
        case '\r':
        case '\t': consume(); break;
        case '/':
            if (peek_next() == '/') {
                while (peek() != '\n' && !at_eof()) consume();
            } else return;
        default: return;
        }
    }
}

static Token consume_string(void) {
    while (peek() != '"' && !at_eof()) {
        // For handling multiline strings
        if (peek() == '\n') scanner.line++;
        consume();
    }

    if (at_eof()) return error_token("Unterminated string.\n");

    // Consume the closing quote
    consume();
    return make_token(TOKEN_STRING);
}

static Token consume_number(void) {
    while (is_digit(peek())) consume();

    if (peek() == '.' && is_digit(peek_next())) {
        consume();
        while (is_digit(peek())) consume();
    }

    return make_token(TOKEN_NUMBER);
}

static TokenType check_keyword(int start, int length, const char *rest,
                               TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(void) {
    switch (scanner.start[0]) {
    case 'a': return check_keyword(1, 2, "nd", TOKEN_AND);
    case 'c': return check_keyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return check_keyword(1, 3, "lse", TOKEN_ELSE);
    case 'f':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'a': return check_keyword(2, 3, "lse", TOKEN_FALSE);
            case 'o': return check_keyword(2, 1, "r", TOKEN_FOR);
            case 'u': return check_keyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    case 'i': return check_keyword(1, 1, "if", TOKEN_IF);
    case 'n': return check_keyword(1, 2, "il", TOKEN_NIL);
    case 'o': return check_keyword(1, 1, "r", TOKEN_OR);
    case 'p': return check_keyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return check_keyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return check_keyword(1, 4, "uper", TOKEN_SUPER);
    case 't':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'h': return check_keyword(2, 2, "is", TOKEN_THIS);
            case 'r': return check_keyword(2, 2, "ue", TOKEN_TRUE);
            }
        }
        break;
    case 'v': return check_keyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return check_keyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_ERROR;
}

static Token consume_identifier(void) {
    while (is_identifier_char(peek())) consume();
    return make_token(identifier_type());
}

Token scan_token(void) {
    consume_whitespace();
    scanner.start = scanner.current;

    if (at_eof()) return make_token(TOKEN_EOF);

    char c = consume();

    if (is_alphanumeric(c)) return consume_identifier();
    if (is_digit(c)) return consume_number();

    switch (c) {
    case '(': return make_token(TOKEN_LEFT_PAREN);
    case ')': return make_token(TOKEN_RIGHT_PAREN);
    case '{': return make_token(TOKEN_LEFT_BRACE);
    case '}': return make_token(TOKEN_RIGHT_BRACE);
    case ';': return make_token(TOKEN_SEMICOLON);
    case ',': return make_token(TOKEN_COMMA);
    case '.': return make_token(TOKEN_DOT);
    case '-': return make_token(TOKEN_MINUS);
    case '+': return make_token(TOKEN_PLUS);
    case '/': return make_token(TOKEN_SLASH);
    case '*': return make_token(TOKEN_STAR);
    case '!':
        return make_token(match_and_consume('=') ? TOKEN_BANG_EQUAL
                                                 : TOKEN_BANG);
    case '=':
        return make_token(match_and_consume('=') ? TOKEN_EQUAL_EQUAL
                                                 : TOKEN_EQUAL);
    case '<':
        return make_token(match_and_consume('=') ? TOKEN_LESS_EQUAL
                                                 : TOKEN_LESS);
    case '>':
        return make_token(match_and_consume('=') ? TOKEN_GREATER_EQUAL
                                                 : TOKEN_GREATER);
    case '"': return consume_string();
    }
    return error_token("Unexpected character.\n");
}
