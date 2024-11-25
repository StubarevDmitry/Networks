#ifndef PROCESS_COMPLETABLE_FUTURES_HPP
#define PROCESS_COMPLETABLE_FUTURES_HPP

#include <future>
#include <vector>
#include <string>
#include <windows.h>

std::wstring CompletableFuturesForOutputWeatherAndDescriptions(PlacesAPI& places, int selectedIndex);

void processElement(const json& elementObj, std::vector<std::wstring>& placesAndDescription, PlacesAPI& places);

#endif