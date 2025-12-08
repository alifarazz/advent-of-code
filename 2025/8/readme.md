Both gold and silver parts are Kruskals MCST algorithm\
I decided to create all the possible edges and then sort them as part of the initial section of the mcst algorithm. The asymptotic ime complexity of it is then O(|V| ^ 2).\
However, finding the closest pair of points in increasing order of their distance can be better done using an all-nearset-neighbors algorithm. Using this [algorithm](https://lic225.github.io/vaidya/ANN_DCG89.pdf), the complexity can be reduced down to O(|V|lg|V|).
