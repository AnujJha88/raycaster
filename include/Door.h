#pragma once

enum class DoorState{
    OPENING,
    CLOSING,
    CLOSED,
    OPEN
};

struct Door{
public:
    double openAmount;
    int locX;
    int locY;
    DoorState state;
    double timer;
};
