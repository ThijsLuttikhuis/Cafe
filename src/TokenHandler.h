//
// Created by Thijs on 02/09/2023.
//

#ifndef CAFE_TOKENHANDLER_H
#define CAFE_TOKENHANDLER_H

#include <utility>
#include <vector>
#include <variant>
#include <cstdint>
#include <string>
#include <memory>

#include "BiDirectionalMap.h"

namespace Cafe {

class Token;

class TokenHandler {
public:
    typedef std::variant<std::string, int, unsigned int, float, double> TokenValue;

    enum class TokenType {
        // quote
        quote_,
        single_qoute,

        // operators
        semicolon_,
        colon_,
        comma_,
        equal_sign,
        plus_,
        minus_,
        exclamation_mark,
        greater_than,
        less_than,
        star_,
        slash_,
        open_bracket,
        close_bracket,
        open_curly_bracket,
        close_curly_bracket,
        open_square_bracket,
        close_square_bracket,

        // keywords
        import_,
        return_,
        print_,
        auto_,
        int_,
        uint_,
        float_,
        double_,

        variable_name,
        number_,

        none_
    };

    TokenHandler() = default;

private:
    static BiDirectionalMap<std::string, TokenType> stringTokenTypeMap;

    std::vector<Token> tokens;

    static std::vector<std::string> separateContentByLine(const std::string &str);

    static void removeExcessSpaces(std::string &basicString);

    static void removeComments(std::string &basicString);

    static std::vector<Token> tokenizeLine(std::string &line);

    static std::pair<Token, unsigned long> getToken(const std::string &line, unsigned long i);

    static std::string getWord(const std::string &line, unsigned long i);

    static std::string getQuote(const std::string &line, unsigned long i);

    static std::string getOperator(const std::string &line, unsigned long i);

public:
    void tokenize(const std::string &fileContent);

    static bool isKeyword(const std::string &word_);

    static bool isOperator(const std::string &basicString);

    static std::string getNumber(const std::string &line, unsigned long i);
};

class Token {
public:
    Token() = default;

    explicit Token(TokenHandler::TokenType tokenType, std::string tokenValue = "")
          : tokenType(tokenType), tokenValue(std::move(tokenValue)) {};

    TokenHandler::TokenType tokenType = TokenHandler::TokenType::none_;
    std::string tokenValue = "";
};

}

#endif //CAFE_TOKENHANDLER_H
