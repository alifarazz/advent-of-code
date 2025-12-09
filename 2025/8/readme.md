Both gold and silver parts are Kruskals MCST algorithm\
I decided to create all the possible edges and then sort them as part of the initial section of the mcst algorithm. The asymptotic time complexity of it is then O(|V| ^ 2).\
However, finding the closest pair of points in increasing order of their distance can be better done using an all-nearset-neighbors algorithm. Using this [algorithm](https://lic225.github.io/vaidya/ANN_DCG89.pdf), the complexity can be reduced down to O(|V|lg|V|).


Update: Instead of sorting the whole array list, I decided to put all the edges into a heap, and min-heapify everytime I want the next smallest edge to add to MCST. This way, we avoid paying the sort overhead on large edges that never get considered since the MCST is built long before. This reduces the time complexity of preparing the edges from O(|V|^2 * log |V|) to O(|V|log|V|). The bottleneck is still the edge creation step that has the time complexity of O(|V|^2).
