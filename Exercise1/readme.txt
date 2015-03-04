Digital 3D Geometry Processing
Exercise 1 – Getting Started with Surface_mesh

Group 5
Deleuran, Anders Holden
Konakovic, Mina Aleksandra
Perrin, Sami

1.2
Implementing calc_valence() required no further change to the file.
We used a Vertex_Iterator so as to iterate over every vertices in the
mesh. (Exactly as mentionned in the course pdf).
Then for each vertex we instantiated a Vertex_around_vertex_circulator,
so as to iterate over every incident vertices once. (In the course pdf)
For each of those incident vertices we incremented a counter and pushed 
the result inside the properity v:valence (previously initalized) of
the corresponding vertex.

1.3
We defined a color gradient in the HSL color representation,
from [0, 1, 0.5] to [0.7, 1, 0.5] which correspond to red to blue and 
converted them to rgb (we wrote a python script for that, that you can
find attached) to produce the actual colors. The red being assigned to
vertices of valence 3 (min) and blue to those of valence 11 (max).
The algorithm is straightforward, as it simply iterates over all the
vertices retrieves it's valence and as recommended use the set_color
function to set it's color.