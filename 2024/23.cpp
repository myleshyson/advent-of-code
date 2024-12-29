#include "helpers.h"

typedef std::unordered_map<std::string, std::vector<std::string>> Graph;

int findTripletsWithT(const Graph& graph) {
    std::set<std::vector<std::string>> triplets;

    // For each vertex
    for (const auto& [vertex, neighbors]: graph) {
        // Only process if vertex starts with 't' or might be part of a triplet
        // with 't' For each pair of its neighbors
        for (auto i = 0; i < neighbors.size(); i++) {
            for (auto j = i + 1; j < neighbors.size(); j++) {
                const auto& n1 = neighbors[i];
                const auto& n2 = neighbors[j];

                // Check if these neighbors are connected
                const auto& n1_neighbors = graph.at(n1);
                if (std::find(n1_neighbors.begin(), n1_neighbors.end(), n2) !=
                    n1_neighbors.end()) {
                    // We found a triplet
                    if (vertex[0] == 't' || n1[0] == 't' || n2[0] == 't') {
                        std::vector<std::string> triplet = {vertex, n1, n2};
                        std::sort(triplet.begin(), triplet.end());
                        triplets.insert(triplet);
                    }
                }
            }
        }
    }

    return triplets.size();
}

// Returns true if all vertices in the potential clique are connected to vertex
// v
bool isConnectedToAll(
        const std::string& v,
        const std::set<std::string>& potentialClique,
        const Graph& graph
) {
    const auto& neighbors = graph.at(v);
    std::set<std::string> neighborSet(neighbors.begin(), neighbors.end());

    for (const auto& u: potentialClique) {
        if (u == v) continue;
        if (neighborSet.find(u) == neighborSet.end()) return false;
    }
    return true;
}

// Helper function to get the neighbors of a vertex as a set
std::set<std::string> getNeighbors(const std::string& vertex, const Graph& graph) {
    const auto& neighbors = graph.at(vertex);
    return std::set<std::string>(neighbors.begin(), neighbors.end());
}

// Helper function to find the intersection of two sets
std::set<std::string> setIntersection(const std::set<std::string>& set1, const std::set<std::string>& set2) {
    std::set<std::string> result;
    std::set_intersection(
        set1.begin(), set1.end(),
        set2.begin(), set2.end(),
        std::inserter(result, result.begin())
    );
    return result;
}

void findMaxClique(
        const Graph& graph,
        std::set<std::string> currentClique, // R
        std::set<std::string> possibleVertices, // P
        std::set<std::string> excludedVertices, // X
        std::set<std::string>& largestClique
) {
    // If both P and X are empty, R is a maximal clique
    if (possibleVertices.empty() && excludedVertices.empty()) {
        if (currentClique.size() > largestClique.size()) {
            largestClique = currentClique;
        }
        return;
    }

    // Choose a pivot vertex from P ∪ X to minimize branching
    std::set<std::string> PossibleUnionExcluded;
    std::set_union(
        possibleVertices.begin(), possibleVertices.end(),
        excludedVertices.begin(), excludedVertices.end(),
        std::inserter(PossibleUnionExcluded, PossibleUnionExcluded.begin())
    );

    // Find pivot vertex with most neighbors in P
    std::string pivot;
    size_t maxCount = 0;
    for (const auto& u : PossibleUnionExcluded) {
        auto neighbors = getNeighbors(u, graph);
        size_t count = setIntersection(neighbors, possibleVertices).size();
        if (count > maxCount) {
            maxCount = count;
            pivot = u;
        }
    }

    // if there is no pivot, since pivot is just a time saving step,
    // we'll pick the first value from possible vertices.
    if (pivot.empty() && !possibleVertices.empty()) {
        pivot = *possibleVertices.begin();
    }

    // if pivot is still empty, we'll just exit here.
    if (pivot.empty()) {
        return;
    }

    // Get vertices in P that aren't neighbors of pivot
    // This is because since the pivot is in our possible, we only
    // need to process the vertices in P that aren't connected to pivot.
    // this lets us see if the these new vertices are in the same clique.
    // we already know that all neighbors of pivot are in the clique.
    auto pivotNeighbors = getNeighbors(pivot, graph);
    std::set<std::string> verticesToTry;
    std::set_difference(
        possibleVertices.begin(), possibleVertices.end(),
        pivotNeighbors.begin(), pivotNeighbors.end(),
        std::inserter(verticesToTry, verticesToTry.begin())
    );

    // For each vertex not connected to pivot
    for (const auto& v : verticesToTry) {
        // Get v's neighbors
        auto neighbors = getNeighbors(v, graph);

        // Add v to R
        currentClique.insert(v);

        // Create new P and X sets from neighbors
        std::set<std::string> newP = setIntersection(possibleVertices, neighbors);
        std::set<std::string> newX = setIntersection(excludedVertices, neighbors);

        // Recursive call
        findMaxClique(graph, currentClique, newP, newX, largestClique);

        // Move v from P to X
        currentClique.erase(v);
        possibleVertices.erase(v);
        excludedVertices.insert(v);
    }
}

int main() {
    Graph graph;
    std::ifstream file = getStream("23.txt");
    std::string line;

    while (getline(file, line)) {
        auto split = line.find('-');
        auto first = line.substr(0, split);
        auto second = line.substr(split + 1);
        graph[first].push_back(second);
        graph[second].push_back(first);
    }

    std::cout << findTripletsWithT(graph) << std::endl;

    // Initialize sets
    std::set<std::string> R; // Current clique (empty initially)
    std::set<std::string> P; // Possible vertices (all vertices initially)
    std::set<std::string> X; // Excluded vertices (empty initially)
    std::set<std::string> largestClique; // Track the largest clique

    // Add all vertices as initial candidates
    for (const auto &vertex: graph | std::views::keys) {
        P.insert(vertex);
    }

    // Find maximum clique
    // Bron–Kerbosch algorithm
    findMaxClique(graph, R, P, X, largestClique);

    std::string result;

    for (const auto& string : largestClique) {
        result += string + ",";
    }
    result.pop_back();
    std::cout << result << std::endl;
}
