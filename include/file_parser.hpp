// File parser for yball configurations
#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <ball.hpp>
#include <settings.hpp>

namespace yballs {
std::vector<yballs::Ball> configuration(std::string filename)
{
	// TODO: quite hack-ish as it stands. There might be neater ways to implement
	// file reading
    std::vector<yballs::Ball> ball_config;
    std::ifstream ifile;

    if (yballs::settings::FLAGS["-v"]) {
        std::cout << "Reading yballs config file from '" << filename << "'...";
    }

    ifile.open(filename);
    if (!ifile) {
        std::cout << "Error opening file located in '" << filename << "'!\n";
        return ball_config;
    }
    std::string line;
    while (std::getline(ifile, line)) {
        // If line is  comment or an empty one, skip
        if (line.length() < 1) {
            continue;
        }
        std::string tk;
        while (line.length() > 0) {
            tk = line.substr(0, line.find(" "));
            if (tk.compare(" ") != 0) {
                if (tk.compare("WIDTH") == 0) {
                    line.erase(0, line.find(" ") + 1);
                    tk = line.substr(0, line.find(" "));
                    while (tk.compare("") == 0) {
                        line.erase(0, line.find(" ") + 1);
                        tk = line.substr(0, line.find(" "));
                    }
                    yballs::settings::PARAMS["--width"] = std::stoi(tk);
                    break;
                } else if (tk.compare("HEIGHT") == 0) {
                    line.erase(0, line.find(" ") + 1);
                    tk = line.substr(0, line.find(" "));
                    while (tk.compare("") == 0) {
                        line.erase(0, line.find(" ") + 1);
                        tk = line.substr(0, line.find(" "));
                    }
                    yballs::settings::PARAMS["--height"] = std::stoi(tk);
                    break;
                } else {
                    yballs::Ball _b;
                    std::istringstream is(line);
                    is >> _b;
                    ball_config.push_back(_b);
                    break;
                }
            }
            line.erase(0, line.find(" ") + 1);
        }
    }
    for (const auto& e : ball_config)
        std::cout << e;
    ifile.close();
    return ball_config;
}
}
