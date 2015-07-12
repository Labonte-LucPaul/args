/*
 * args.h
 *
 *  Created on: 2015-07-07
 *      Author: luc
 *
 *  Original source: The clean code
 *  ISBN: 978-0-13-235088-2
 */

#ifndef ARGS_H_
#define ARGS_H_

#include <cassert>
#include <cstring>
#include <exception>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "strhelper.h"

class ArgumentMarshaler;
class BoolArgumentMarshaler;
class StringArgumentMarshaler;
class IntegerArgumentMarshaler;

class Args {
    public:
        Args() = delete;
        Args(const std::string schema, int argc, char* argv[]);
        ~Args();

    public:
        bool getBoolean(std::string key);
        std::string getString(std::string key);
        int getInteger(std::string key);
        double getDouble(std::string key);
        bool exists(std::string key);

    private:
        void parseSchema(const std::string schema);
        void parseSchemaElement(const std::string element);
        void validateSchemaElementId(const char elementId);
        void parseArgumentStrings(std::vector<std::string>& argsList);
        void parseArgumentCharacters(const std::string& argChars, const std::vector<std::string>::iterator& end);
        void parseArgumentCharacter(const char& argChar, const std::vector<std::string>::iterator& end);

    private:

        std::unique_ptr<std::map<std::string, std::unique_ptr<ArgumentMarshaler>>> marshalers;
        std::unique_ptr<std::set<std::string>> argsFound;
        std::vector<std::string>::iterator currentArgument;
};

class ArgumentMarshaler {
    public:
        virtual void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end) = 0;
        virtual inline ~ArgumentMarshaler() {}
};

class BoolArgumentMarshaler : public ArgumentMarshaler {
    public:
        explicit inline BoolArgumentMarshaler() : valueBool(false) {}
        inline void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end) { this->valueBool = true; }
        inline static bool get(ArgumentMarshaler& am) {
            assert(&am != nullptr && "BoolArgumentMarshaler->get->Argument is NULL.");

            return ((BoolArgumentMarshaler&)am).valueBool;
        }
        inline ~BoolArgumentMarshaler() {}

    private:
        bool valueBool = false;
};

class StringArgumentMarshaler : public ArgumentMarshaler {
    public:
        explicit inline StringArgumentMarshaler() : valueString("") {}
        inline void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end) {

            {
                auto tmp = it + 1;
                if(tmp == end) {
                    std::string msg("Wrong parameter option for argument: ");
                    msg += *it;
                    throw std::runtime_error(msg);
                }
                if(tmp->at(0) == '-') {
                    std::string msg("Invalid start of option for argument: ");
                    msg += *it;
                    msg += " ; cannot start with '-' char. Try: \"\\\"-your string\\\"\"";
                    throw std::runtime_error(msg);
                }
            }

            ++it;
            std::string str(*it);
            this->valueString = str;
        }
        inline static std::string get(ArgumentMarshaler& am) {
            assert(&am != nullptr && "StringArgumentMarshaler->get->Argument is NULL.");

            return ((StringArgumentMarshaler&)am).valueString;
        }
        inline ~StringArgumentMarshaler() {}

    private:
        std::string valueString;
};

class IntegerArgumentMarshaler : public ArgumentMarshaler {
    public:
        explicit inline IntegerArgumentMarshaler() : valueInteger(0)  {}
        inline void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end) {

            {
                auto tmp = it + 1;
                if(tmp == end) {
                    std::string msg("Wrong parameter option for argument '");
                    msg += *it;
                    msg += "'.";
                    throw std::runtime_error(msg);
                }
                if (tmp->length() > 1) {
                    if(tmp->at(0) == '-' && !std::isdigit(tmp->at(1))) {
                        std::string msg("Invalid option for argument '");
                        msg += *it;
                        msg += "' ; '-' cannot be followed by characters. An Integer value is required";
                        throw std::runtime_error(msg);
                    }
                }
                if(!StringHelper::isNumber(*tmp)) {
                    std::string msg("Argument ");
                    msg += *it;
                    msg += " require an Integer value.";
                    throw std::runtime_error(msg);
                }
            }

            ++it;
            std::string str(*it);
            int val = std::stoi(str);

            this->valueInteger = val;
        }
        inline static int get(ArgumentMarshaler& am) {
            assert(&am != nullptr && "IntegerArgumentMarshaler->get->Argument is NULL.");

            return ((IntegerArgumentMarshaler&)am).valueInteger;
        }
        inline ~IntegerArgumentMarshaler() {}

    private:
        int valueInteger;
};

class DoubleArgumentMarshaler : public ArgumentMarshaler {
    public:
        explicit inline DoubleArgumentMarshaler() : valueDouble(0)  {}
        inline void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end) {

            {
                auto tmp = it + 1;
                if(tmp == end) {
                    std::string msg("Wrong parameter option for argument '");
                    msg += *it;
                    msg += "'.";
                    throw std::runtime_error(msg);
                }
                if (tmp->length() > 1) {
                    if(tmp->at(0) == '-' && !std::isdigit(tmp->at(1))) {
                        std::string msg("Invalid option for argument '");
                        msg += *it;
                        msg += "' ; '-' cannot be followed by characters. A double value is required.";
                        throw std::runtime_error(msg);
                    }
                }
                if(!StringHelper::isDouble(*tmp)) {
                    std::string msg("Argument ");
                    msg += *it;
                    msg += " require a Double value.";
                    throw std::runtime_error(msg);
                }
            }

            ++it;
            std::string str(*it);
            double val = std::stod(str);
            this->valueDouble = val;
        }
        inline static double get(ArgumentMarshaler& am) {
            assert(&am != nullptr && "DoubleArgumentMarshaler->get->Argument is NULL.");

            return ((DoubleArgumentMarshaler&)am).valueDouble;
        }
        inline ~DoubleArgumentMarshaler() {}

    private:
        double valueDouble;
};

#endif /* ARGS_H_ */
