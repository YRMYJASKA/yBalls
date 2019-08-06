#pragma once

namespace yballs {
namespace settings {
    int pixels_to_units = 10; // e.g. w/ ptu = 10, radius of 1 becomes 10 pixels
    double accepted_collision_error = 0.3; 
    bool DEBUG = true;

    // the time interval (ms) on which balls' speed
    // is increased and collision checked
    unsigned int delta_time = 100;
}
}
