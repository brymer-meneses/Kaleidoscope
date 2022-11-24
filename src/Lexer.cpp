#include "Lexer.hpp"
#include "Error.hpp"
#include "Token.hpp"

#include <cctype>
#include <charconv>
#include <iostream>
#include <map>
#include <vector>

const static std::map<std::string_view, TokenType> KEYWORDS = {
    {"and", TokenType::And},       {"or", TokenType::Or},
    {"def", TokenType::Def},       {"if", TokenType::If},
    {"elif", TokenType::ElseIf},   {"else", TokenType::Else},
    {"then", TokenType::Then},     {"let", TokenType::Let},
    {"end", TokenType::End},       {"extern", TokenType::Extern},
    {"return", TokenType::Return},
};

std::vector<Token> Lexer::lex() {
    std::vector<Token> tokens = {};

    while (!isAtEnd()) {
        this->start = this->current;
        try {
            std::optional<Token> token = this->lexSingleToken();
            if (token)
                tokens.emplace_back(token.value());
        } catch (Error e) {
            e.report();
        }
    };

    tokens.emplace_back(TokenType::Eof, this->getLineLoc());

    return tokens;
}

std::optional<Token> Lexer::lexSingleToken() {
    char c = this->advance();

    switch (c) {
    case ' ':
    case '\t':
        return std::nullopt;

    case '\n':
    case '\r':
        this->line += 1;
        return std::nullopt;

    case '#':
        while (peek() != '\n' && peek() != '\r' && peek() != EOF)
            advance();
        return std::nullopt;

    case ',':
        return Token(TokenType::Comma, this->getLineLoc());
    case '(':
        return Token(TokenType::LeftParen, this->getLineLoc());
    case ')':
        return Token(TokenType::RightParen, this->getLineLoc());

    case '!':
        if (match('='))
            return Token(TokenType::BangEqual, this->getLineLoc());
        else
            return Token(TokenType::Bang, this->getLineLoc());
    case '>':
        if (match('='))
            return Token(TokenType::GreaterEqual, this->getLineLoc());
        else
            return Token(TokenType::Greater, this->getLineLoc());
    case '<':
        if (match('='))
            return Token(TokenType::LesserEqual, this->getLineLoc());
        else
            return Token(TokenType::Lesser, this->getLineLoc());
    case '=':
        if (match('='))
            return Token(TokenType::EqualEqual, this->getLineLoc());
        else
            return Token(TokenType::Equal, this->getLineLoc());

    case '+':
        return Token(TokenType::Plus, this->getLineLoc());
    case '-':
        return Token(TokenType::Minus, this->getLineLoc());
    case '*':
        return Token(TokenType::Star, this->getLineLoc());
    case '/':
        return Token(TokenType::Slash, this->getLineLoc());
    case '"':
        return lexString();
    default:
        if (std::isalpha(c) || c == '_') {
            return this->lexIdentifier();
        } else if (std::isdigit(c)) {
            return this->lexNumber();
        } else {
            // TODO use libfmt to fix this mess
            throw Error("Unexpected character").setTip("Perhaps you can add ");
        }
    }
    return std::nullopt;
}

Token Lexer::lexString() {
    while (this->peek() != '"' && !this->isAtEnd()) {
        if (this->peek() == '\n')
            this->line += 1;

        advance();
    }

    advance();

    std::string_view substring =
        this->source.substr(this->start + 1, this->current - this->start - 2);

    return Token(TokenType::String, substring,
                 LineLoc(this->start + 1, this->current - 1, this->line));
}

Token Lexer::lexIdentifier() {
    while (std::isalnum(peek()) || peek() == '_')
        advance();

    std::string_view keyword =
        this->source.substr(this->start, this->current - this->start);
    // TODO: support else if, not elif by looking ahead

    TokenType type;
    try {
        type = KEYWORDS.at(keyword);
        return Token(type, LineLoc(this->start, current, line));
    } catch (std::out_of_range e) {
        type = TokenType::Identifier;
        // pass lexeme to the keyword which will be used to reference a variable
        return Token(type, keyword,
                     LineLoc(this->start, this->current, this->line));
    }
}
Token Lexer::lexNumber() {
    while (std::isdigit(peek()))
        advance();

    if (peek() == '.' && std::isdigit(this->peekNext())) {
        advance();

        while (std::isdigit(peek()))
            advance();
    }

    double num;
    std::string_view string_lexeme =
        this->source.substr(this->start, this->start - this->current);
    std::from_chars(string_lexeme.data(),
                    string_lexeme.data() + string_lexeme.size(), num);

    return Token(TokenType::Number, num,
                 LineLoc(this->start, this->current, this->line));
}
