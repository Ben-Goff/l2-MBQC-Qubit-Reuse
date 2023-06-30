# l2-MBQC-Qubit-Reuse

A dense circuit at a specific depth has a larger total causal cone size, while reducing a circuit in an optimal way tends to decrease its causal cone. But also having a large causal cone while reducing a circuit means you may not get the full reduction.
Thus, there can't be an optimal heuristic based only on causal cone size. This is why I added a slack variable.n

decross paper only ckecks how much cc of j adds to i. that is not the biggest effect, much bigger is adding cc of i to everything that depends on j

how does circuit reduction change with graph degree. randomly generate these