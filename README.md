# Graph-Updates-Optimization

## The Problem Statement
Given a rooted DAG and several updates, find the total time to execute all the updates. Each update is of the form <node, number of levels, entry time, processing time>.
node is the node in the graph on which some processing is going to be performed.
the number of levels from the node is also going to be involved in the processing. Default value 1 means only the node is involved, value 2 means the node and its neighbors are involved, value 3 means the node, its neighbors and their neighbors are involved, and so on.
entry time is the time after which this processing can be started (it is simply an integer).
processing time is the amount of time required for the processing after all the required nodes are available (integer).

A condition is that no node can be processed by two updates at the same time. Thus, if two updates are <n1, 1, 5, 10> and <n1, 1, 9, 2>, the two updates may conflict.

The task is to:
create random DAGs (or use existing DAGs if available online). Make sure it is well-connected. Number of edges could be four times the number of nodes.
create random updates
think of, implement, and optimize an algorithm to solve the problem.
run it on graphs with a million nodes and a million updates.

