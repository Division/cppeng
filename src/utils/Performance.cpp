//
// Created by Sidorenko Nikita on 11/25/18.
//

#include "Performance.h"

double engine::Performance::_fps = 0;
double engine::Performance::_lastUpdateTime = 0;
bool engine::Performance::_averagePrinted = true;
std::map<int, engine::Performance::EntryData> engine::Performance::_entries;