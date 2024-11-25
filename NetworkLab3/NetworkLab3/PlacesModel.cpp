
#include "PlacesAPI.hpp"
#include "ProcessCompletableFutures.h"
#include <vector>
#include <string>
#include "PlacesModel.hpp"

namespace NetworkLab3 {

    std::wstring insertNewLines(const std::wstring& str, size_t maxLength) {
        std::wstring result;
        size_t currentLength = 0;

        for (wchar_t ch : str) {
            result += ch;
            currentLength++;

            if (currentLength >= maxLength && ch == L' ') {
                result += L'\n';
                currentLength = 0;
            }

            if (ch == L'\n') {
                currentLength = 0;
            }
        }

        return result;
    }

    std::vector<std::wstring> Model::GetLocations(const std::wstring& chois)
    {
        return places.getPlaces(chois);
    }

    std::wstring Model::GetWeatherAndDescriptions(int selectedIndex)
    {
        return insertNewLines(CompletableFuturesForOutputWeatherAndDescriptions(places, selectedIndex), 115);
    }
}