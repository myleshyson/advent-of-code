#include <complex>
#include <map>
#include <iostream>
#include <set>
#include "helpers.h"

int findScore(std::pair<int, int> node, std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > &graph,
        std::vector<std::vector<int> > &matrix, std::set<std::pair<int, int> > &seen, std::vector<std::pair<int, int>> &destinations) {
   if (matrix[node.first][node.second] == 9) {
      destinations.push_back(node);
      return 1;
   }

   int result = 0;

   for (auto neighbor: graph[node]) {
      if (matrix[neighbor.first][neighbor.second] == matrix[node.first][node.second] + 1 && !seen.contains(neighbor)) {
         seen.insert(neighbor);
         result += findScore(neighbor, graph, matrix, seen, destinations);
      }
   }

   return result;
}

int findRating(std::pair<int, int> node, std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > &graph,
        std::vector<std::vector<int> > &matrix, std::pair<int, int> const &target) {
   if (node == target) {
      return 1;
   }

   int result = 0;

   for (auto neighbor: graph[node]) {
      if (matrix[neighbor.first][neighbor.second] == matrix[node.first][node.second] + 1) {
         result += findRating(neighbor, graph, matrix, target);
      }
   }

   return result;
}

int main() {
   std::ifstream file = getStream("10.txt");
   std::string line;
   std::vector<std::vector<int> > matrix;
   std::pair<int, int> directions[] = {
      std::make_pair(0, 1),
      std::make_pair(0, -1),
      std::make_pair(1, 0),
      std::make_pair(-1, 0)
   };

   std::map<std::pair<int, int>, std::vector<std::pair<int, int> > > graph;

   while (getline(file, line)) {
      std::vector<int> row;
      row.reserve(line.size());

      for (int i = 0; i < line.size(); i++) {
         row.push_back(line[i] - '0');
      }

      matrix.push_back(row);
   }

   for (int i = 0; i < matrix.size(); i++) {
      for (int j = 0; j < matrix[0].size(); j++) {
         for (auto direction: directions) {
            std::pair<int, int> newPos = std::make_pair(direction.first + i, direction.second + j);

            if (newPos.first < 0 || newPos.first >= matrix.size() || newPos.second < 0 || newPos.second >= matrix[0].
                size()) {
               continue;
            }
            if (matrix[newPos.first][newPos.second] != matrix[i][j] + 1) continue;

            graph[std::make_pair(i, j)].push_back(newPos);
         }
      }
   }

   int answer[] = {0, 0};

   for (auto &pair: graph) {
      if (matrix[pair.first.first][pair.first.second] == 0) {
         std::set<std::pair<int, int> > seen;
         std::vector<std::pair<int, int>> destinations;
         answer[0] += findScore(pair.first, graph, matrix, seen, destinations);

         int rating = 0;

         for (auto &destination : destinations) {
            rating += findRating(pair.first, graph, matrix, destination);
         }

         answer[1] += rating;
      }
   }

   std::println("{}", answer);
}
