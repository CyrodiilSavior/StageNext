#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <Arduino.h>
#include "input_data.h"

class InputReader {
    private:

    public:
        InputReader();
        ~InputReader();
        InputData read();
};

#endif