#pragma once

#include <string>
#include <exception>

class NoSuchAlgorithmException : public std::exception {
private:
    std::string msg;

public:
    NoSuchAlgorithmException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};

class InvalidKeySpecException : public std::exception {
private:
    std::string msg;

public:
    InvalidKeySpecException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};

class InvalidKeyException : public std::exception {
private:
    std::string msg;

public:
    InvalidKeyException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};

class SignatureException : public std::exception {
private:
    std::string msg;

public:
    SignatureException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};

class IOException : public std::exception {
private:
    std::string msg;

public:
    IOException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};

class InvalidAlgorithmParameterException : public std::exception {
private:
    std::string msg;

public:
    InvalidAlgorithmParameterException(const std::string &message = "") : msg(message) {
    }

    const char *what() const throw() {
        return msg.c_str();
    }
};
