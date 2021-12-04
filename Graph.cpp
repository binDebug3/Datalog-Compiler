#include "Graph.h"

Graph::Graph() = default;
Graph::~Graph() = default;

void Graph::insertNode(int node, const std::set<int>& edges) {
    graphEdges.insert(graphEdges.begin(), std::pair<int, std::set<int>>(node, edges));
    nodesVisited.insert(nodesVisited.begin(), std::pair<int, bool>(node, false));
}
void Graph::insertEdge(int node, int edge) {
    auto iterGraph = graphEdges.find(node);
    if (iterGraph != graphEdges.end())
        iterGraph->second.insert(edge);
}
bool Graph::checkVisited(int node) {
    auto iterBool = nodesVisited.find(node);
    if (iterBool != nodesVisited.end())
        return iterBool->second;
    else
        return false;
}

void Graph::markNode(int node, bool marked) {
    auto iterBool = nodesVisited.find(node);
    if (iterBool != nodesVisited.end())
        iterBool->second = marked;
}
int Graph::getLength() const {
    return graphEdges.size();
}
int Graph::getNode(int index) {
    auto iterGraph = graphEdges.begin();
    for (int i = 0; i < index; i++) {
        iterGraph++;
    }
    return iterGraph->first;
}
std::set<int> Graph::getEdgesOfNode(int node) {
    auto iterGraph = graphEdges.begin();
    for (int i = 0; i < node; i++) {
        iterGraph++;
    }
    return iterGraph->second;
}
int Graph::getLengthOfEdges(int node) {
    auto iterEdge = graphEdges.find(node);
    if (iterEdge != graphEdges.end())
        return iterEdge->second.size();
    else
        return 0;
}
void Graph::pushBack(int order) {
    postOrder.push_back(order);
}
void Graph::pushBack(const std::vector<int>& smolPostOrder) {
    for (int i : smolPostOrder) {
        postOrder.push_back(i);
    }
}
void Graph::resetVisits() {
    std::map<int, bool>::iterator iterMark;
    for (iterMark = nodesVisited.begin(); iterMark != nodesVisited.end(); iterMark++)
        iterMark->second = false;
}
void Graph::resetPostOrder() {
    postOrder.clear();
}
std::vector<int> Graph::getPostOrder() {
    return postOrder;
}
std::string Graph::printPostOrder() const {
    std::string output;
    for (unsigned int i = 0; i < postOrder.size(); i++) {
        output += std::to_string(postOrder.at(i)) + ",";
    }
    output = output.substr(0,output.size()-1);
    return output + "\n";
}
void Graph::insertSCC(std::set<int> newSCC) {
    SCC.push_back(newSCC);
}
void Graph::deleteSCC() {
    SCC.erase(SCC.begin() + SCC.size()-1);
}
void Graph::insertElement(int index, int elem) {
    SCC.at(index).insert(elem);
}
std::set<int> Graph::getSCC(int index) {
    return SCC.at(index);
}
std::vector<std::set<int>> Graph::getAllSCC() {
    return SCC;
}
std::string Graph::printSCC() const {
    std::string output;
    for (unsigned int i = 0; i < SCC.size(); i++) {
        output += "SCC" + std::to_string(i) + ":";
        auto iterElem = SCC.at(i).begin();
        for (unsigned int j = 0; j < SCC.at(i).size(); j++) {
            output += std::to_string(*iterElem) + ",";
            iterElem++;
        }
        if (output.substr(output.size()-1,1) == ",")
            output = output.substr(0, output.size()-1);
        output += "\n";
    }
    return output;
}


std::string Graph::toString() const {
    auto iterGraph = graphEdges.begin();
    std::string output = "Dependency Graph\n";
    for (unsigned int i = 0; i < graphEdges.size(); i++) {
        output += "R" + std::to_string(iterGraph->first) + ":";
        auto iterEdges = iterGraph->second.begin();
        for (unsigned int k = 0; k < iterGraph->second.size(); k++) {
            output += "R" + std::to_string(*iterEdges) + ",";
            iterEdges++;
        }
        if (output.substr(output.size()-1,1) == ",")
            output = output.substr(0, output.size()-1);
        output += "\n";
        iterGraph++;
    }
    return output + "\n";
}