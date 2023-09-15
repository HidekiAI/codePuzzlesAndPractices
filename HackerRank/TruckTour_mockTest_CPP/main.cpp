#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'truckTour' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts 2D_INTEGER_ARRAY petrolpumps as parameter.
 */

int truckTour(vector<vector<int>> petrolpumps) {
  // each pump has a petrol amount and distance to the next pump
  auto get_pump_info = [](const vector<int> &pump) {
    return make_pair(pump[0], pump[1]);
  };
  // get the petrol amount of the pump
  auto get_petrol_amount = [](const pair<int, int> &pump_info) {
    return pump_info.first;
  };
  // get the distance to the next pump
  auto get_distance = [](const pair<int, int> &pump_info) {
    return pump_info.second;
  };
  // validate whether it could reach next pump based on current tank level and
  // current pump index and return new tank level after reaching next pump and
  // pumping the petrol amount
  auto travel_and_validate =
      [&get_pump_info, &get_petrol_amount,
       &get_distance](const vector<vector<int>> &petrolpumps,
                      int current_pump_index, int tank_level) -> int {
    const auto pump_count = petrolpumps.size();
    const auto next_pump_index = (current_pump_index + 1) % pump_count;
    const auto pump = get_pump_info(petrolpumps[next_pump_index]);
    const auto petrol_amount = get_petrol_amount(pump);
    const auto distance = get_distance(pump);
    if (tank_level + petrol_amount >= distance) {
      return tank_level + petrol_amount - distance;
    }
    // should be 0 or negative value here
    return tank_level + petrol_amount - distance;
  };

  // walk through each element in the petrolpumps array starting from the passed
  // in index (and wrap back to head of the array if needed) and

  // problem is that there could be 10^5 pumps, hence iterating
  // from i=0 will not be ideal choice.  also, there is no "pump-ID"
  // so identification cannot locate the pump at later lookup.
  // so we should commit to just iterate all pumps once and only once.
  // to do so, we'll keep a record of current pump-index candiate
  // and move on to the next.
  // At the same time, we want to probably (once it's working)
  // break (fork) the array into mulitple sequences and track each
  // partitions' candidate-index and upon joining back from the fork,
  // we'll then evaluate.  Problem with that is complicated by
  // when the next-neighbor is on the next block...

  // truck moves at 1 km/liter
  // Petrol   Distance
  // 1        5
  // 10       3
  // 3        4
  // If I start at index=0, I can only go 1 km since I only have 1 liter of
  // petrol. but the next pump is 5 km away, so I cannot reach it. if I start at
  // index=1, I can go 10 km, but the next pump is 3 km away, so I cannot reach
  // it. Since next pump is 3 km away, I will have 7 liters of petrol left. I
  // then pump 3 litres and now I have 10 liters of petrol.  Next pump is 4 km
  // away. so I now have 6 liters of petrol left when I reach index=0.  And I
  // will add 1 liter of petrol, and have 7 liters of petrol. I can get to the
  // next pump which is 5 km away.  I will have 2 liters of petrol left.  So
  // index=1 is the ideal candidate. If I start from index=2, I can go 3 km, but
  // the next pump is 4 km away, so I cannot reach it. In conclusion, index=1 is
  // the ideal candidate.
  auto tank_capacity = 0;

  auto pump_count = petrolpumps.size();

  // initial search for candiates are first look at the petrol
  // amount and next distance
  // the potential problem with this approach is that we will be
  // penalized with traversing the list more than once
  vector<pair<size_t, pair<int, int>>> potential_start_index;
  for (long unsigned int current_pump = 0; current_pump < pump_count;
       ++current_pump) {
    auto pump = get_pump_info(petrolpumps[current_pump]);
    if (get_petrol_amount(pump) >= get_distance(pump)) {
      potential_start_index.push_back(make_pair(current_pump, pump));
    }
  }
  // sort by petrol amount left after usage (petrol - distance)
  sort(potential_start_index.begin(), potential_start_index.end(),
       [&get_petrol_amount,
        &get_distance](const pair<size_t, pair<int, int>> &a,
                       const pair<size_t, pair<int, int>> &b) {
         return (get_petrol_amount(a.second) - get_distance(a.second)) >
                (get_petrol_amount(b.second) - get_distance(b.second));
       });

  // now we have a list of potential start index, but we need to
  // verify that it is indeed a valid start index by traversing
  // from that index to the end of the list (and wrap around to)
  // back up to current index.
  auto ret = -1;
  for_each(
      potential_start_index.begin(), potential_start_index.end(),
      [&potential_start_index, &pump_count, &petrolpumps, &ret,
       &travel_and_validate](const auto &current) {
        const auto starting_pump_index = current.first;
        auto tank_level = current.second.first - current.second.second; // mutable

        // now begin walking the circular queue to see if we can reach back
        // to starting index
        const auto next_pump_index = (starting_pump_index + 1) % pump_count;
        for (auto current_pump_index = next_pump_index;
             current_pump_index != starting_pump_index; ++current_pump_index) {
          tank_level =
              travel_and_validate(petrolpumps, current_pump_index, tank_level);
          if (tank_level <= 0) {
            // out of petrol, cannot reach next pump
            break;
          }
          if (current_pump_index == starting_pump_index) {
            // we have reached back to the starting index
            // so we have found the answer
            ret = starting_pump_index;
          }
        }
      });
  return ret;
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string n_temp;
  getline(cin, n_temp);

  int n = stoi(ltrim(rtrim(n_temp)));

  vector<vector<int>> petrolpumps(n);

  for (int i = 0; i < n; i++) {
    petrolpumps[i].resize(2);

    string petrolpumps_row_temp_temp;
    getline(cin, petrolpumps_row_temp_temp);

    vector<string> petrolpumps_row_temp =
        split(rtrim(petrolpumps_row_temp_temp));

    for (int j = 0; j < 2; j++) {
      int petrolpumps_row_item = stoi(petrolpumps_row_temp[j]);

      petrolpumps[i][j] = petrolpumps_row_item;
    }
  }

  int result = truckTour(petrolpumps);

  fout << result << "\n";

  fout.close();

  return 0;
}

string ltrim(const string &str) {
  string s(str);

  s.erase(s.begin(),
          find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));

  return s;
}

string rtrim(const string &str) {
  string s(str);

  s.erase(
      find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
      s.end());

  return s;
}

vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));

    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}
