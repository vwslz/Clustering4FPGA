# 4FPGA

## Algorithm

| Step | Time | Space | Parallized |
| --------------- | --------------- | --------------- | --------------- |
| Get Adjancy Matrix | O(n) | O(n^2) | True |
| Get Adjancy List | O(n) | O(n^2) | True |
| Get Cluster | O(\|E\|) | O(n) | False |
| Get Clustered Nodes | O(n) | O(m) | False |
| Get Sorted Clustered Nodes | O(m) | O(m) | False |
| Get Edge Pairs | O(m^2) | O(m^2) | True with larger memory requirement |
| Get Scaled Feature | O(m) | O(m) | True |

### Note
1. n: \# nodes; m: \# clustered nodes
2. Time complexity of *get adjancy matrix* and *get adjancy list* is O(n) due to parallelism. Otherwise O(n^2).
3. The space for adjancy matrix is reused for adjancy list. Hence, O(n) instead of 2O(n).
4. For *get cluster*, it used the node with smallest id in a cluster as the root and use BFS to find the whole cluster sequencially. Although the time is bounded by O(|E|) which has the worst case of O(n^2), since the graph is usually sparse, the complexity is much smaller.
5. The *x*, *y*, *z* of a clustered node is the average of all nodes in this cluster.
6. Creteria of selecting edge pair is to have *phi slope* < 0.100 and *z0* < 200.
7. The scaled feature are saved in *output* with length of *num_clusters*

## Validation
### Data
* D0toPiKInAcceptanceSignal_Iteration8_10000.cfg_HFMLTriggerInterface.json.bz2
* \# Events: 1000
### Result
1. The clusters are the same for the 1000 events.
![Output](img/compareDifference.png)

2. The number of edges varies for different events. Note that the difference is small (within +- 3 edges) which indicates the high similarity between the two generated edge sets.

*Example: Event 14*

| Set | (r phi z) (r phi z) | phi slope | z0 | Included? |
| --------------- | --------------- | --------------- | --------------- | --------------- |
| New | (2.566 2.510 6.377) (3.464 2.599 -2.516) | 0.099952 | 31.784 | YES |
| Old | (2.56688 2.50902 6.37650) (3.46450 2.59933 -2.51550) | 0.10062 | 31.80459 | **NO** |

## Log
* Fix the error in original python code: change *-2* to *+2*.
