#include <exception>
class ExpectedError : public std::exception {};
class InsertedError : public std::exception {};