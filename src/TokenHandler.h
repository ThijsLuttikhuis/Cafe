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


class Token;

class TokenHandler {
public:
    typedef std::variant<std::string, int, unsigned int, float, double> TokenValue;

    enum class TokenType {
        semicolon_,
        quote_,
        import_,
        return_,

        open_bracket,
        close_bracket,
        open_curly_bracket,
        close_curly_bracket,
        open_square_bracket,
        close_square_bracket,

        auto_,
        int_,
        uint_,
        float_,
        double_,
        function_,
        operator_,
        none_
    };

    TokenHandler() = default;
private:
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

};

class Token {
public:
    Token() = default;

    explicit Token(TokenHandler::TokenType tokenType, TokenHandler::TokenValue tokenValue = 0)
        : tokenType(tokenType), tokenValue(std::move(tokenValue)) {};

    TokenHandler::TokenType tokenType = TokenHandler::TokenType::none_;
    TokenHandler::TokenValue tokenValue = 0;
};


#endif //CAFE_TOKENHANDLER_H
