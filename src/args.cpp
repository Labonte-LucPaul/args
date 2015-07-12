/*
 * args.cpp
 *
 *  Created on: 2015-07-07
 *      Author: luc
 */

#include "args.h"

Args::Args(const std::string schema, int argc, char* argv[]) :
        marshalers(std::make_unique<std::map<std::string, std::unique_ptr<ArgumentMarshaler>>>()),
        argsFound(std::make_unique<std::set<std::string>>()),
        currentArgument() {
    parseSchema(schema);
    std::vector<std::string> v(argv, argv + argc);
    parseArgumentStrings(v);
}

Args::~Args() {

}

void Args::parseSchema(const std::string schema) {

    auto sp = StringHelper::split(schema, ',');

    for(size_t i = 0; i < sp->size(); ++i) {
        if(sp->at(i).length() > 0)
            parseSchemaElement(sp->at(i));
    }

}

void Args::parseSchemaElement(const std::string element) {
    char elementId = element.at(0);
    std::string elemTail("");

    if(element.length() > 1)
        elemTail = element.substr(1);

    validateSchemaElementId(elementId);
    std::string key(StringHelper::toString(elementId));

    if(elemTail.length() == 0)
        this->marshalers->insert(std::pair<std::string, std::unique_ptr<BoolArgumentMarshaler>>(key, std::make_unique<BoolArgumentMarshaler>()));
    else if(elemTail.compare("*") == 0)
        this->marshalers->insert(std::pair<std::string, std::unique_ptr<StringArgumentMarshaler>>(key, std::make_unique<StringArgumentMarshaler>()));
    else if(elemTail.compare("#") == 0)
        this->marshalers->insert(std::pair<std::string, std::unique_ptr<IntegerArgumentMarshaler>>(key, std::make_unique<IntegerArgumentMarshaler>()));
    else if(elemTail.compare("##") == 0)
            this->marshalers->insert(std::pair<std::string, std::unique_ptr<DoubleArgumentMarshaler>>(key, std::make_unique<DoubleArgumentMarshaler>()));
    else
        throw std::runtime_error("Wrong shema type.");
}

void Args::validateSchemaElementId(const char elementId) {
    if(!std::isalpha(elementId))
        throw std::runtime_error("Wrong character in schema");
}

void Args::parseArgumentStrings(std::vector<std::string>& argsList) {

    this->currentArgument = argsList.begin();

    while(this->currentArgument != argsList.end()) {
        if(this->currentArgument->at(0) == '-')
            parseArgumentCharacters(this->currentArgument->substr(1), argsList.end());
        ++this->currentArgument;
    }
}

void Args::parseArgumentCharacters(const std::string& argChars, const std::vector<std::string>::iterator& end) {
    for(size_t i = 0; i < argChars.length(); ++i)
        parseArgumentCharacter(argChars.at(i), end);
}

void Args::parseArgumentCharacter(const char& argChar, const std::vector<std::string>::iterator& end) {

    this->argsFound->insert(StringHelper::toString(argChar));
    auto str = std::string(StringHelper::toString(argChar));

    if(!this->exists(str)) {
        std::string msg("Argument ");
        msg += str;
        msg += " does not exist.";
        throw std::runtime_error(msg);
    }

    try {
        this->marshalers->at(str)->set(this->currentArgument, end);
    } catch(std::exception& ex) {
        throw std::runtime_error(ex.what());
    }
}

bool Args::getBoolean(std::string key) {
    return BoolArgumentMarshaler::get(*this->marshalers->at(key));
}

std::string Args::getString(std::string key) {
    return StringArgumentMarshaler::get(*this->marshalers->at(key));
}

int Args::getInteger(std::string key) {
    return IntegerArgumentMarshaler::get(*this->marshalers->at(key));
}

double Args::getDouble(std::string key) {
    return DoubleArgumentMarshaler::get(*this->marshalers->at(key));
}

bool Args::exists(std::string key) {
    try {
        return this->marshalers->find(key) != this->marshalers->end();
    } catch(std::exception& ex) {
        throw ex;
    }
}
