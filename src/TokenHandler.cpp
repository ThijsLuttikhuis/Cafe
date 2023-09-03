//
// Created by Thijs on 02/09/2023.
//

#include <iostream>
#include <cwctype>

#include "TokenHandler.h"

namespace Cafe {

BiDirectionalMap<std::string, TokenHandler::TokenType> TokenHandler::stringTokenTypeMap = BiDirectionalMap<std::string, TokenHandler::TokenType>(
      {
            {"\"",            TokenHandler::TokenType::quote_},
            {"'",             TokenHandler::TokenType::single_qoute},
            {";",             TokenHandler::TokenType::semicolon_},
            {":",             TokenHandler::TokenType::colon_},
            {",",             TokenHandler::TokenType::comma_},
            {"=",             TokenHandler::TokenType::equal_sign},
            {"+",             TokenHandler::TokenType::plus_},
            {"-",             TokenHandler::TokenType::minus_},
            {"!",             TokenHandler::TokenType::exclamation_mark},
            {">",             TokenHandler::TokenType::greater_than},
            {"<",             TokenHandler::TokenType::less_than},
            {"*",             TokenHandler::TokenType::star_},
            {"/",             TokenHandler::TokenType::slash_},
            {"(",             TokenHandler::TokenType::open_bracket},
            {")",             TokenHandler::TokenType::close_bracket},
            {"{",             TokenHandler::TokenType::open_curly_bracket},
            {"}",             TokenHandler::TokenType::close_curly_bracket},
            {"[",             TokenHandler::TokenType::open_square_bracket},
            {"]",             TokenHandler::TokenType::close_square_bracket},

            {"import",        TokenHandler::TokenType::import_},
            {"return",        TokenHandler::TokenType::return_},
            {"print",         TokenHandler::TokenType::print_},
            {"auto",          TokenHandler::TokenType::auto_},
            {"int",           TokenHandler::TokenType::int_},
            {"uint",          TokenHandler::TokenType::uint_},
            {"float",         TokenHandler::TokenType::float_},
            {"double",        TokenHandler::TokenType::double_},

            {"variable name", TokenHandler::TokenType::variable_name},
            {"number",        TokenHandler::TokenType::number_},

            {"none",          TokenHandler::TokenType::none_}});

void TokenHandler::tokenize(const std::string &fileContent) {

    std::vector<std::string> lines = separateContentByLine(fileContent);

    tokens = {};

    for (unsigned long i = 0; i < lines.size(); i++) {
        std::cout << i << ": '" << lines[i] << "'" << std::endl;

        std::vector<Token> tokenLine = tokenizeLine(lines[i]);
        tokens.insert(tokens.end(), tokenLine.begin(), tokenLine.end());
        tokens.emplace_back(TokenType::semicolon_);
    }

}

std::vector<std::string> TokenHandler::separateContentByLine(const std::string &str) {
    unsigned long fileLength = str.size();
    std::vector<std::string> lines;

    unsigned long i = 0;
    while (i < fileLength) {
        auto j = str.find(';', i);
        if (j == std::string::npos) {
            j = fileLength;
        }

        std::string line = str.substr(i, j - i);

        removeComments(line);
        removeExcessSpaces(line);
        lines.emplace_back(line);
        i = j + 1;
    }

    return lines;
}

void TokenHandler::removeComments(std::string &line) {

    // remove '/* ... */'
    while (true) {
        auto j = line.find("/*");
        auto k = line.find("*/");
        if (j == std::string::npos) {
            break;
        }
        if (k == std::string::npos) {
            k = line.size() - 1;
        }
        line.erase(j, k - j + 2);
    }

    // remove '// ... \n'
    while (true) {
        auto j = line.find("//");
        auto k = line.find('\n');
        if (j == std::string::npos) {
            break;
        }
        if (k == std::string::npos) {
            k = line.size() - 1;
        }
        line.erase(j, k - j + 1);
    }

}

void TokenHandler::removeExcessSpaces(std::string &line) {
    std::replace_if(line.begin(), line.end(), isspace, ' ');
    std::size_t doubleSpace = line.find("  ");
    while (doubleSpace != std::string::npos) {
        line.erase(doubleSpace, 1);
        doubleSpace = line.find("  ");
    }
}

std::vector<Token> TokenHandler::tokenizeLine(std::string &line) {
    std::vector<Token> tokenLine;

    unsigned long i = 0;
    while (i < line.size()) {
        auto [token, j] = getToken(line, i);
        if (token.tokenType == TokenType::none_) {
            break;
        }
        tokenLine.push_back(token);
        i = j;
    }

    if (i < line.size()) {
        std::cerr << "invalid token: '" << line.substr(0, i) << " ->" << line.substr(i, line.size() - i) << "'"
                  << std::endl;
        throw std::exception();
    }

    return tokenLine;
}

std::pair<Token, unsigned long> TokenHandler::getToken(const std::string &line, unsigned long i) {
    if (isspace(line[i])) {
        i++;
    }
    if (i >= line.size()) {
        return {Token(TokenType::semicolon_), i};
    }

    // check quote
    {
        std::string quote_ = getQuote(line, i);
        if (!quote_.empty()) {
            return {Token(TokenType::quote_, quote_), i + quote_.size() + 2};
        }
    }

    // check keyword / var name
    {
        std::string word_ = getWord(line, i);
        if (!word_.empty()) {
            if (isKeyword(word_)) {
                return {Token(stringTokenTypeMap.at(word_)), i + word_.size()};
            }
            return {Token(TokenType::variable_name), i + word_.size()};
        }
    }

    // check number
    {
        std::string number_ = getNumber(line, i);
        if (!number_.empty()) {
            return {Token(TokenType::number_, number_), i + number_.size()};
        }
    }

    // check operator
    {
        std::string operator_ = getOperator(line, i);
        if (!operator_.empty()) {
            if (isOperator(operator_)) {
                return {Token(stringTokenTypeMap.at(operator_)), i + 1};
            }
            return {};
        }
    }

    return {};
}

std::string TokenHandler::getQuote(const std::string &line, unsigned long i) {
    if (line[i] != '"') {
        return "";
    } else {
        unsigned long long int endPos = i;
        do {
            endPos = line.find('"', endPos + 1);
            if (endPos == std::string::npos) {
                return "";
            }
        } while (line[endPos - 1] == '\\');

        return line.substr(i + 1, endPos - (i + 1));
    }
}

std::string TokenHandler::getWord(const std::string &line, unsigned long i) {
    if (!isalpha(line[i]) && line[i] != '_') {
        return "";
    } else {
        auto j = i + 1;
        while (j < line.size()) {
            if (!isalnum(line[j]) && line[j] != '_') {
                break;
            }
            j++;
        }
        if (isspace(line[j])) {
            j++;
        }
        if (j < line.size() && (!getNumber(line, j).empty() || (!getWord(line, j).empty() && isKeyword(getWord(line, j))))) {
            return "";
        }
        std::string keyword = line.substr(i, j - i);
        return keyword;
    }
}

std::string TokenHandler::getOperator(const std::string &line, unsigned long i) {
    if (!ispunct(line[i])) {
        return "";
    } else {
        return line.substr(i, 1);
    }
}

std::string TokenHandler::getNumber(const std::string &line, unsigned long i) {
    unsigned long nDots = 0;
    if (!isdigit(line[i]) && line[i] != '.') {
        return "";
    } else {
        if (line[i] == '.') {
            nDots++;
        }
        auto j = i + 1;
        while (j < line.size()) {
            if (!isdigit(line[j]) && line[j] != '.') {
                break;
            }
            if (line[j] == '.') {
                nDots++;
            }
            j++;
        }
        if (nDots > 1) {
            return "";
        }
        if (isspace(line[j])) {
            j++;
        }
        if (j < line.size() && isalpha(line[j])) {
            if (nDots == 1) {
                // double / single precision float: d / f
                if (line[j] != 'd' && line[j] != 'f') {
                    return "";
                }
            } else {
                // unsigned / signed int: u / s
                if (line[j] != 'u' && line[j] != 's') {
                    return "";
                }
            }
            j++;
             if (!getWord(line, j).empty() || !getNumber(line, j).empty()){
                return "";
            }
        }
        std::string number = line.substr(i, j - i);
        return number;
    }
}

bool TokenHandler::isKeyword(const std::string &word_) {
    return word_ == "quote" ||
           word_ == "import" ||
           word_ == "return" ||
           word_ == "auto" ||
           word_ == "int" ||
           word_ == "uint" ||
           word_ == "float" ||
           word_ == "double";

}

bool TokenHandler::isOperator(const std::string &operator_) {
    return operator_ == ";" ||
           operator_ == ":" ||
           operator_ == "," ||
           operator_ == "=" ||
           operator_ == "+" ||
           operator_ == "-" ||
           operator_ == "!" ||
           operator_ == ">" ||
           operator_ == "<" ||
           operator_ == "*" ||
           operator_ == "/" ||
           operator_ == "(" ||
           operator_ == ")" ||
           operator_ == "{" ||
           operator_ == "}" ||
           operator_ == "[" ||
           operator_ == "]";
}


}
