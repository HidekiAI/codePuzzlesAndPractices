// Amazon's AWS provides fast and efficient server solutions.
// The developers want to stress-test the quality of the servers'
// channels.  They must ensure the following:

// - Each of the packets must be sent via single channel
// - Each of the channels must transfer at least one packet

// The quality of the transfer for a channle is defined by median of the sizes
// of all the data packets sent through that channel.

// Note: The median of an array is the middle element if the array is sorted in
// non-decreasing order. If the number of elements in the array is even, the
// median is the average of the two middle elements.

// find the maximum possible sum of the qualities of all channels.  If the
// answer is a floating-point value, round it to the next higher integer.

// Example:
// packets = [1, 2, 3, 4, 5]
// channels = 2

// At least one packet has to go through each of the 2 channels.  One maximmal
// solution is to transfer packets {1,2,3,4} thought channel 1 and packet {5}
// through channel 2.

// The quality of transfer for channel 1 is (2 + 3) / 2 = 2.5 and that of
// channel 2 is 5.  Their sum is 2.5 + 5 = 7.5 which rounds up to 8.

// Function Description:
// findMaximumQuality has the following parameter(s):
// - int packets[n]: an array of integers, the sizes of the packets
// - int channels: an integer, the number of channels

// Returns:
// - int: the maximum possible sum of the qualities of all channels

// Sample input:
//    STDIN       Function
//    -----       --------
//    5           packets[] size n = 5
//    2           packets = [2, 2, 1, 5, 3]
//    2
//    1
//    5
//    3
//    2      channels = 2

// Sample output:
//    7

// Explanation:
// One solution is to send packet {5} through one channel and {2,2,1,3} through
// the other. the sum of quality is 5 + (2 + 2) / 2 = 7

// Sample Case 1
//   STDIN       Function
//   -----       --------
//   3          packets[] size n = 3
//   89     packets = [89, 48, 14]
//   48
//   14
//   3     channels = 3

// sample output:
//   151

// explanation
// There are 3 channels and 3 packets.  Each channel carries one, so the overall
// sum of quality is 89 + 48 + 14 = 151

#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);

/*
 * Complete the 'findMaximumQuality' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY packets
 *  2. INTEGER channels
 */

// Assume packets are presorted...
long calc_median(vector<int> packets) {
  auto n = packets.size();
  auto mid_index = n / 2;
  auto prev_index = mid_index - 1;
  // if width of array is even, return average of the two middle elements
  if (n % 2 == 0) {
    return (packets[mid_index] + packets[prev_index]) / 2;
  }
  return packets[mid_index];
}

// NOTE: std::sort is not part of include
long findMaximumQuality(vector<int> packets, int channels) {
  auto fn_max = [](int a, int b) { return a > b ? a : b; };

  // packets = [1, 2, 3, 4, 5], channels = 2; solution:  8 (split 5, and 1234)
  // quality of transfer on ch1 is (2+3)/2 = 2.5
  // ch2 is 5; hence max of 8
  // packets = [2, 2, 1, 5, 3], channels = 2; solution: 7 (split 5, and (2,2,3))
  // packets = [89, 48, 14], channels = 3; solution 151 (packet per channel)

  // I'd want to sort the array, so that I can partition/split the
  // groups to n channels where the last channel are of the
  // biggest packets
  sort(packets.begin(), packets.end()); // sort in place

  // once it's sorted, we'll do the simple/quick edge cases first
  // in general, it seems to be an easy solution to take the top
  // n-1 packets and assign them to the n-1 channels, and the rest
  // to the last channel
  auto num_packets = packets.size();
  if (num_packets == 1) {
    // sum is as-is
    return packets[0];
  }
  if (num_packets == 2) {
    // sum is the average of the two
    return (packets[0] + packets[1]) / 2;
  }
  if (num_packets == channels) {
    // sum is the sum of all packets
    auto sum = 0;
    for (auto i = 0; i < num_packets; ++i) {
      sum += packets[i];
    }
    return sum;
  }
  // now take the top n-1 packets (of the largests) and assign them to the n-1
  // channels and the rest to the last channel
  // next, to maximize, I want to find an optimal median, and it seems
  // best way to do this is assign first to be median and the rest to
  // be the biggests
  // for example, for [2,2,1,5,3] => [1,2,2,3,5] sorted, if there are
  // 3 channels, I'd assign 3 to ch2, 5 to ch3, and (1,2,2) (median=2 )
  // and the sum of the qualities is 5 + (2+2)/2 = 7
  // first, take largest channels-1 packets
  // if there are 10 packets and 4 channels, take the top 3
  // and median the last 7
  // 10 - 3 = 7 => packets[9]...packets[7], median is packets[5..0]

  // i.e. 10 - 3 = 7, so packets[9]...packets[7] for 3 channels out of 4
  auto packets_tail = num_packets - 1;
  //  i.e. 10 - 4 = 6, so packets[6]...packets[0]
  auto packets_head = num_packets - channels;
  auto sum = 0;
  for (auto packets_index = num_packets - 1; num_packets > 1; --packets_index) {
    if (packets_index > packets_head) {
      sum += packets[packets_index]; // just add it
    } else {
      // median constant, should it be all packets or up to packets_head?
      auto packets_sliced =
          vector<int>(packets.begin(), packets.begin() + packets_tail);
      auto median = calc_median(packets_sliced);
      sum += fn_max(packets[packets_index], median);
      break; // we're done
    }
  }
  return sum;
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string packets_count_temp;
  getline(cin, packets_count_temp);

  int packets_count = stoi(ltrim(rtrim(packets_count_temp)));

  vector<int> packets(packets_count);

  for (int i = 0; i < packets_count; i++) {
    string packets_item_temp;
    getline(cin, packets_item_temp);

    int packets_item = stoi(ltrim(rtrim(packets_item_temp)));

    packets[i] = packets_item;
  }

  string channels_temp;
  getline(cin, channels_temp);

  int channels = stoi(ltrim(rtrim(channels_temp)));

  long result = findMaximumQuality(packets, channels);

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
