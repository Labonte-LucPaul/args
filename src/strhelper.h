/*
 * strhelper.h
 *
 *  Created on: 2015-07-10
 *      Author: luc
 */

#ifndef STRHELPER_H_
#define STRHELPER_H_

#if __cplusplus < 201402L
#error "Must define variable __cplusplus=201402L and build with flag -std=c++14"
#endif

#include <sstream>
#include <string>
#include <algorithm>

class StringHelper {

    public:
        static inline std::string toString(const char& c) {
            std::string s("");
            std::stringstream ss;
            ss << c;
            ss >> s;
            return s;
        }

        static inline std::string toString(const int& i) {
            std::string s("");
            std::stringstream ss;
            ss << i;
            ss >> s;
            return s;
        }

        static inline std::unique_ptr<std::vector<std::string>> split(const std::string str, const char sChar) {

            auto splitted = std::make_unique<std::vector<std::string>>();

            std::string tmp("");

            for(size_t i = 0; i < str.length(); ++i) {
                char c = str.at(i);
                if(c == sChar) {
                    splitted->push_back(std::string(tmp));
                    tmp = std::string("");
                } else {
                    tmp.append(str.substr(i, 1));
                }
            }
            splitted->push_back(std::string(tmp));

            return std::move(splitted);
        }

        static inline bool isNumber(std::string str) {
            size_t neg = std::count(str.begin(), str.end(), '-');
            if(neg > 1 || neg == str.length())
                return false;

            return !str.empty() && std::find_if(str.begin(),
                   str.end(), [](char c) { return !std::isdigit(c) && c != '-';}) == str.end();
        }

        static inline bool isDouble(std::string str) {
            size_t count = std::count(str.begin(), str.end(), '.');
            if(count > 1)
                return false;

            size_t neg = std::count(str.begin(), str.end(), '-');
            if(neg > 1 || neg == str.length())
                return false;

            return !str.empty() &&
                   std::find_if(str.begin(), str.end(), [](char c) { return (!std::isdigit(c)) && (c != '.' && c != '-'); }) == str.end();
        }
};

#endif /* STRHELPER_H_ */
