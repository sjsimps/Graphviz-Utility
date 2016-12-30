
#ifndef GRAPHVIZ_UTILITY_H
#define GRAPHVIZ_UTILITY_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

class GraphvizUtility
{
public:
    GraphvizUtility(bool directed = true);
    ~GraphvizUtility();

    // Adds a node to the visualizer. If a node with the same ID is
    // present, it and its edges are removed, and the new node takes its place.
    void AddNode(int id, std::string label, std::string options = "");

    // Updates the node for the given id without removing its edges.
    void EditNode(int id, std::string label, std::string options = "");

    // Remove the node and all of its edges
    void RemoveNode(int id);

    // Add an edge connecting two nodes by their IDs
    void AddEdge(int from_id, int to_id, std::string label);

    // Remove all edges between two nodes
    void RemoveEdges(int id_1, int id_2);

    // Remove edges between two nodes by direction
    void RemoveEdgesDirected(int from_id, int to_id);

    // Export the graph to Graphviz .dot format
    void Export(std::string filename);

    // Clear the nodes in the graph
    void ClearGraph();

private:
    struct GraphvizEdge
    {
        int to_id;
        std::string label;
    };

    struct GraphvizNode
    {
        int id;
        std::vector<struct GraphvizEdge> edges;
        std::vector<int> incoming_edges;
        std::string label;
        std::string options;

        GraphvizNode(const int& s_id = 0) : id(s_id)
        {
        }
        bool operator==(const GraphvizNode& a) const
        {
            return (id == a.id);
        }
        bool operator<(const GraphvizNode& a) const
        {
            return (id < a.id);
        }
    };

    std::map<int,struct GraphvizNode> m_nodes;
    bool m_directed;
};

GraphvizUtility::GraphvizUtility(bool directed)
{
    m_directed = directed;
}

GraphvizUtility::~GraphvizUtility()
{
}

void GraphvizUtility::AddNode(int id, std::string label, std::string options)
{
    RemoveNode(id);
    struct GraphvizNode node;
    node.id = id;
    node.label = label;
    node.options = options;
    
    std::pair<int,struct GraphvizNode> node_pair(id, node);
    m_nodes.insert(node_pair);
}

void GraphvizUtility::EditNode(int id, std::string label, std::string options)
{
    std::map<int,struct GraphvizNode>::iterator it;
    it = m_nodes.find(id);
    
    if (it != m_nodes.end())
    {
        m_nodes[id].label = label;
        m_nodes[id].options = options;
    }
}

void GraphvizUtility::RemoveNode(int id)
{
    std::map<int,struct GraphvizNode>::iterator it;
    it = m_nodes.find(id);

    if (it != m_nodes.end())
    {
        for (unsigned int w = 0; w < m_nodes[id].incoming_edges.size(); w++)
        {
            int from_id = m_nodes[id].incoming_edges[w];
            RemoveEdgesDirected(from_id, id);
        }
        for (unsigned int w = 0; w < m_nodes[id].incoming_edges.size(); w++)
        {
            int to_id = m_nodes[id].edges[w].to_id;
            RemoveEdgesDirected(id, to_id);
        }
        m_nodes.erase(id);
    }
}

void GraphvizUtility::AddEdge(int from_id, int to_id, std::string label)
{
    std::map<int,struct GraphvizNode>::iterator from_it, to_it;
    from_it = m_nodes.find(from_id);
    to_it = m_nodes.find(to_id);

    if (from_it != m_nodes.end() && to_it != m_nodes.end())
    {
        struct GraphvizEdge edge;
        edge.to_id = to_id;
        edge.label = label;
        m_nodes[from_id].edges.push_back(edge);
        m_nodes[to_id].incoming_edges.push_back(from_id);
    }
}

void GraphvizUtility::RemoveEdges(int id_1, int id_2)
{
    RemoveEdgesDirected(id_1, id_2);
    RemoveEdgesDirected(id_2, id_1);
}

void GraphvizUtility::RemoveEdgesDirected(int from_id, int to_id)
{
    unsigned int x = 0;
    while (x < m_nodes[from_id].edges.size())
    {
        if (m_nodes[from_id].edges[x].to_id == to_id)
        {
            m_nodes[from_id].edges.erase(m_nodes[from_id].edges.begin()+x);
        }
        else
        {
            x++;
        }
    }
    x = 0;
    while (x < m_nodes[to_id].incoming_edges.size())
    {
        if (m_nodes[to_id].incoming_edges[x] == from_id)
        {
            m_nodes[to_id].incoming_edges.erase(m_nodes[to_id].incoming_edges.begin()+x);
        }
        else
        {
            x++;
        }
    }
}

void GraphvizUtility::Export(std::string filename)
{
    std::ofstream out(filename);

    std::string edge_repr;
    if (m_directed)
    {
        out << "\ndigraph{";
        edge_repr = "->";
    }
    else 
    {
        out << "\ngraph{";
        edge_repr = "--";
    }

    for(auto node : m_nodes)
    {
        out << "\n_" <<  node.second.id
            << " [ "<< node.second.options << " label=\"" << node.second.label << "\"];";
    }
    for(auto node : m_nodes)
    {
        for(auto edge : node.second.edges)
        {
            out << "\n_" << node.second.id 
                << " " << edge_repr << " _" << edge.to_id
                << " [label=\"" << edge.label << "\"];";
        }
    }
    out << "\n}";
    out.close();
}

void GraphvizUtility::ClearGraph()
{
    m_nodes.clear();
}

#endif
