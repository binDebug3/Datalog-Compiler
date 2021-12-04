#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <vector>

class Graph {
private:
    std::map<int, std::set<int>> graphEdges;
    std::map<int, bool> nodesVisited;
    std::vector<int> postOrder;
    std::vector<std::set<int>> SCC;
public:
    Graph();
    ~Graph();
    void insertNode(int node, const std::set<int>& edges);
    void insertEdge(int node, int edge);
    int getNode(int index);
    std::set<int> getEdgesOfNode(int node);
    int getLengthOfEdges(int node);

    bool checkVisited(int node);
    void markNode(int node, bool marked);
    void pushBack(int order);
    void resetVisits();

    int getLength() const;
    void pushBack(const std::vector <int>& smolPostOrder);
    std::vector<int> getPostOrder();
    void resetPostOrder();
    std::string printPostOrder() const;

    void insertSCC(std::set<int> newSCC);
    void deleteSCC();
    void insertElement(int index, int elem);
    std::set<int> getSCC(int index);
    std::vector<std::set<int>> getAllSCC();
    std::string printSCC() const;

    std::string toString() const;
};
#endif //GRAPH_H
