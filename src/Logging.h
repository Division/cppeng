//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_LOGGING_H
#define CPPWRAPPER_LOGGING_H

#include <iostream>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
//#define ENGLog(...) (emscripten_log(__VA_ARGS__))
#define ENGLog(...) {}
#else
#define ENGLog(...) printf(__VA_ARGS__);printf("\n");
#endif

#endif //CPPWRAPPER_LOGGING_H
