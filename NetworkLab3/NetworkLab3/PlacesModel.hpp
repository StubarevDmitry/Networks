#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>

class PlacesAPI;  // Forward declaration

namespace NetworkLab3 {

    public ref class Model
    {
    private:
        PlacesAPI& places;

    public:
        Model(PlacesAPI& places) : places(places) {}

        std::vector<std::wstring> GetLocations(const std::wstring& chois);
        std::wstring GetWeatherAndDescriptions(int selectedIndex);
    };

}

#endif