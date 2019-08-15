#pragma once

#include <string>
#include <unordered_map>

namespace yballs {
namespace settings {
    std::unordered_map<std::string, bool> FLAGS;
    std::unordered_map<std::string, int> PARAMS;

    void init_defaults()
    {
        // Set flags
        FLAGS["-v"] = false;
        FLAGS["-r"] = true;
        FLAGS["--conf"] = false;
        FLAGS["--mass-independent"] = false;
        FLAGS["-g"] = false;

        // Set params
        PARAMS["--width"] = 640;
        PARAMS["--height"] = 480;
        PARAMS["-n"] = 1;

        // Random generation configurations
        PARAMS["--min-rad"] = 10;
        PARAMS["--max-rad"] = 60;
        PARAMS["--min-mass"] = 1;
        PARAMS["--max-mass"] = 6;
        PARAMS["--min-vel"] = -5;
        PARAMS["--max-vel"] = 5;
        PARAMS["--rad-to-mass"] = 10;
    }
}
}
