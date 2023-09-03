//
// Created by Thijs on 02/09/2023.
//

#include <iostream>
#include <cwctype>

#include "TokenHandler.h"

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

    // check quote_
    std::string quote_ = getQuote(line, i);
    if (!quote_.empty()) {
        return {Token(TokenType::quote_, quote_), i + quote_.size() + 1};
    }

    // check bracket
    std::string operator_ = getOperator(line, i);
    if (!operator_.empty()) {
        if (operator_ == "(") {
            return {Token(TokenType::open_bracket), i + 1};
        }
        if (operator_ == ")") {
            return {Token(TokenType::close_bracket), i + 1};
        }
    }

    // check keyword / var name
    std::string word_ = getWord(line, i);
    if (!word_.empty()) {
        if (word_ == "import") {
            return {Token(TokenType::import_), i + word_.size()};
        }
    }

    return {};
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
        std::string keyword = line.substr(i, j);
        return keyword;
    }
}

std::string TokenHandler::getQuote(const std::string &line, unsigned long i) {
    if (line[i] != '"') {
        return "";
    } else {
        unsigned long long int endPos;
        do {
            endPos = line.find('"', i + 1);
            if (endPos == std::string::npos) {
                return "";
            }
        } while (line[endPos-1] == '\\');

        return line.substr(i + 1, endPos - i);
    }
}

std::string TokenHandler::getOperator(const std::string &line, unsigned long i) {
    if (!ispunct(line[i])) {
        return "";
    } else {
        return line.substr(i, 1);
    }
}
