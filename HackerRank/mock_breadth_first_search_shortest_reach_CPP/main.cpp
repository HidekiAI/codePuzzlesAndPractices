#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'bfs' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. INTEGER m
 *  3. 2D_INTEGER_ARRAY edges
 *  4. INTEGER s
 */

vector<int> bfs(int n, int m, vector<vector<int>> edges, int s) {
  // e.g. 1
  // n=5 nodes,  m=2 edges
  // edges = [1,2], [1,3],  s (start) = 1
  // e.g. 2
  // n=3, m=1
  // edges = [2,3], s=2
  auto get_nodes = [edges]() {
    // locate distinct nodes for all edges
    std::map<int, int> nodes;
    for (auto i = 0; i < edges.size(); ++i) {
      for (auto j = 0; j < edges[i].size(); ++j) {
        // if not in map, add it
        if (nodes.find(edges[i][j]) != nodes.end()) {
          nodes[edges[i][j]] = 1;
        } else {
          // increment count
          nodes[edges[i][j]] += 1;
        }
      }
    }
    // return just the keys
    vector<int> keys;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      keys.push_back(it->first);
    }
    return keys;
  };

  map<int, pair<pair<int, int>, int>>
      distances; // key: Node, value: pair((i,j) position, distance from s)
  auto make_distance_element = [](int i, int j, int distance) {
    return make_pair(make_pair(i, j), distance);
  };

  auto calc_distance_from_node = [s, &edges, get_nodes, &distances]() {
    // calculate distance from s per node - do NOT include start-node s in
    // distance list if node is unreachable, set distance to -1
    auto nodes = get_nodes();
    for (auto i = 0; i < edges.size(); ++i) {
      for (auto j = 0; j < edges[i].size(); ++j) {
        // if not in map, add it
        if (distances.find(edges[i][j]) != distances.end()) {
          nodes[edges[i][j]] = 1;
        } else {
          // increment count
          nodes[edges[i][j]] += 1;
        }
      }
    }
  };
}

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string q_temp;
  getline(cin, q_temp);

  int q = stoi(ltrim(rtrim(q_temp)));

  for (int q_itr = 0; q_itr < q; q_itr++) {
    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input =
        split(rtrim(first_multiple_input_temp));

    int n = stoi(first_multiple_input[0]);

    int m = stoi(first_multiple_input[1]);

    vector<vector<int>> edges(m);

    for (int i = 0; i < m; i++) {
      edges[i].resize(2);

      string edges_row_temp_temp;
      getline(cin, edges_row_temp_temp);

      vector<string> edges_row_temp = split(rtrim(edges_row_temp_temp));

      for (int j = 0; j < 2; j++) {
        int edges_row_item = stoi(edges_row_temp[j]);

        edges[i][j] = edges_row_item;
      }
    }

    string s_temp;
    getline(cin, s_temp);

    int s = stoi(ltrim(rtrim(s_temp)));

    vector<int> result = bfs(n, m, edges, s);

    for (size_t i = 0; i < result.size(); i++) {
      fout << result[i];

      if (i != result.size() - 1) {
        fout << " ";
      }
    }

    fout << "\n";
  }

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
