# Graphviz Utility for C++
Easily export a graph to .dot format

### Sample Usage:
```c++
#include "graphviz_utility.h"

int main()
{
    GraphvizUtility util;
    util.AddNode(1,"A", "fillcolor=blue, style=\"filled\",");
    util.AddNode(2,"B");
    util.AddNode(3,"C");
    util.AddEdge(1,2,"A-B");
    util.AddEdge(2,3,"B-C");
    util.AddEdge(3,1,"C-A");
    util.AddEdge(1,1,"A-A");
    util.Export("example.dot");
    return 0;
}
```
