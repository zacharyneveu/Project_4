# results4.txt

# Usage
To run Knapsack for all input files, use `python run_knap.py 8 1025`. The final two parameters are the start and end range of sizes for which to run. Each instance only uses one processor core, so it is possible to run multiple ranges in parallel on multicore systems.

# Knapsack
The knapsack algorithm implemented successfully implements a branch and bound algorithm to solve the given knapsack instances. The algorithm goes through each item in the knapsack instance, branching on whether to include the item or leave it out. An initial greedy-style solution is found by going through the items in order of descending value/cost ratio and including all items that fit, while skipping those that don't. From this solution, the algorithm backtracks up the solution binary tree, exploring branches whose bound is greater than the best found solution using DFS. The algorithm successfuly solves many of the knapsack instances, but could be improved for speed. This algorithm is able to complete knapsack instances from 8-32 in under 10 minutes per problem, but times out when solving problems 48-1024. One large benefit when compared to exhaustive algorithms, is that the partial solutions found when this algorithm times out are already very good, and frequently turn out to actually be optimal even when a time limit is hit. Further optimizations to this approach are possible. One possible improvement would be to explore branches using a different algorithm than DFS to explore the solution tree. This would remove the assumption coded into this implementation that good solutions should be very similar. Removing this assumption could help explore more regions of the search tree faster, potentially providing the ability to prune more branches.
