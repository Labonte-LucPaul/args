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
        virtual ~ArgumentMarshaler() {}
};

class BoolArgumentMarshaler : public ArgumentMarshaler {
    public:
        BoolArgumentMarshaler();
        void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end);
        static bool get(ArgumentMarshaler& am);

    private:
        bool valueBool = false;
};

class StringArgumentMarshaler : public ArgumentMarshaler {
    public:
        StringArgumentMarshaler();
        void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end);
        static std::string get(ArgumentMarshaler& am);

    private:
        std::string valueString;
};

class IntegerArgumentMarshaler : public ArgumentMarshaler {
    public:
        IntegerArgumentMarshaler();
        void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end);
        static int get(ArgumentMarshaler& am);

    private:
        int valueInteger;
};

class DoubleArgumentMarshaler : public ArgumentMarshaler {
    public:
        DoubleArgumentMarshaler();
        void set(std::vector<std::string>::iterator& it, const std::vector<std::string>::iterator& end);
        static double get(ArgumentMarshaler& am);

    private:
        double valueDouble;
};

#endif /* ARGS_H_ */
