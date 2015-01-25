#ifndef TEST_FAILURE_H
#define TEST_FAILURE_H

#include <exception>
#include <sstream>

class TestFailure : public std::exception {
    private:
    const char * expr;
    const char * funcName;
    const char * file;
    unsigned int line;
    public:
    TestFailure(const char * _expr, const char * _funcName, const char * _file, unsigned int _line) :
        expr(_expr), funcName(_funcName), file(_file), line(_line) {};
    TestFailure
        (const char * _lft, const char * _rht, std::string _operator, const char * _funcName, const char * _file, unsigned int _line) :
        funcName(_funcName), file(_file), line(_line)
     {
        std::stringstream ss;
        ss << _lft << _operator << _rht;
        expr = ss.str().c_str();
    }
    virtual const char * what() const throw() {
        std::stringstream ss;
        ss << "ASSERTION FAILURE for " << funcName << " : " << std::endl << expr;
        ss << std::endl << file << ": " << line;
        return ss.str().c_str();
    };
};

#endif // TEST_FAILURE_H
