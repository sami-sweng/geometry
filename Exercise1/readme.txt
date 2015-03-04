Digital 3D Geometry Processing
Exercise 1 – Getting Started with Surface_mesh

Group 5
Deleuran, Anders Holden
Konakovic, Mina Aleksandra
Perrin, Sami

1.2
Implementing calc_valence() required no further change to the file.
We used a Vertex_Iterator so as to iterate over every vertices in the
mesh. (Exactly as mentioned in the course pdf).
Then for each vertex we instantiated a Vertex_around_vertex_circulator,
so as to iterate over every incident vertices once (to count the number of neighbouring vertices for each vertex).
For each of those incident vertices we incremented a counter and pushed 
the result inside the property vvalence_ (previously initialized) of
the corresponding vertex.

1.3
We worked with HSL color system and divided the hue interval [0.0-0.7] by number of maximal valence to set number_of_maximal_valence different colors. To optimise the time and memory, we didn’t calculated the exact number of different valence values, but made number_of_maximal_valence distinct colors. In our code, we used a global variable unsigned int max_valence for storing the detected maximal valence value, and an array listOfColors to store different colors. Exact one color is assigned to every valence value.
    
