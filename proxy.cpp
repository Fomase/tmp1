#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

QueryType StringIntoQueryType(const string& text) {
    if (text == "NEW_BUS"s) {
        return QueryType::NewBus;
    } else if (text == "BUSES_FOR_STOP"s) {
        return QueryType::BusesForStop;
    } else if (text == "STOPS_FOR_BUS"s) {
        return QueryType::StopsForBus;
    } else if (text == "ALL_BUSES"s) {
        return QueryType::AllBuses;
    }
    return QueryType::AllBuses;
}

istream& operator>>(istream& is, Query& q) {
    string raw_type;
    cin >> raw_type;
    q.type = StringIntoQueryType(raw_type);
    switch (q.type) {
        case QueryType::NewBus :
            cin >> q.bus;
            int stop_count;
            cin >> stop_count;
            q.stops.resize(stop_count);
            for (auto& stop : q.stops) {
                cin >> stop;
            }
            break;
            
        case QueryType::BusesForStop :
            cin >> q.stop;
            break;
            
        case QueryType::StopsForBus :
            cin >> q.bus;
            break;
            
        case QueryType::AllBuses :
            break;
    }
    return is;
}

struct BusesForStopResponse {
    string stop;
    vector <string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty()) {
        os << "No stop"s;
        return os;
    }

    bool first = true;
    for (const auto& bus : r.buses) {
        if (first == true) {
            os << bus;
            first = false;
            continue;
        }
        os << " "s << bus;
    }
    return os;
}

struct StopsForBusResponse {
    string bus;
    vector <BusesForStopResponse> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    if (r.stops.empty()) {
        os << "No bus"s;
        return os;
    }
    bool first = true;
    for (const auto& stop : r.stops) {
        if (!first) {
            os << endl;
        } else {
            first = false;
        }
        if (stop.buses.size() == 1) {
            os << "Stop " << stop.stop << ": no interchange"s;
        } else {
            os << "Stop "s << stop.stop << ":"s;
            for (const auto& bus : stop.buses) {
                if (bus == r.bus) {
                    continue;
                }
                os << " "s << bus;
            }
        }
    }
    return os;
}

struct AllBusesResponse {
    vector <pair<string, vector<string>>> buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    if (r.buses.empty()) {
        os << "No buses";
        return os;
    }
    bool first = true;
    for (const auto& bus : r.buses) {
        if (!first) {
            os << endl;
        } else {
            first = false;
        }
        os << "Bus "s << bus.first << ":"s;
        for (const auto& stop : bus.second) {
            os << " "s << stop;
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        all_buses[bus] = stops;
        
        for (const auto& stop : stops) {
            all_stops[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse bfsr;
        bfsr.stop = stop;

        if (all_stops.count(stop) == 0) {
            return bfsr;
        }
        
        for (const auto& bus : all_stops.at(stop)) {
            bfsr.buses.push_back(bus);
        }
        
        return bfsr;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse sfbr;
        sfbr.bus = bus;

        if (all_buses.count(bus) == 0) {
            return sfbr;
        }

        for (const auto& stop : all_buses.at(bus)) {
            sfbr.stops.push_back(GetBusesForStop(stop));
        }
        return sfbr;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse abr;
        for (const auto& bus : all_buses) {
            abr.buses.push_back({bus.first, bus.second});
        }
        return abr;
    }

    void Test() {
        Query q;
        BusManager bm;
        ostringstream output;
        istringstream input;
        input.str("ALL_BUSES"s);
        input >> q;
        assert(q.type == QueryType::AllBuses);
        output << bm.GetAllBuses();
        assert(output.str() == "No buses"s);
        input.str("BUSES_FOR_STOP Marushkino"s);
        input >> q;
        assert(q.type == QueryType::BusesForStop);
        assert(q.stop == "Marushkino"s);
        output << bm.GetBusesForStop(q.stop);
        assert(output.str() == "No stop"s);

    }

private:
   map <string, vector<string>> all_buses;
   map <string, vector<string>> all_stops;
};

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;

    bm.Test();

    
}