Digital 3D Geometry Processing
Exercise 4 – Surface Smoothing and Enhancement

Group 5
Deleuran, Anders Holden
Konakovic, Mina Aleksandra
Perrin, Sami

3.1 Uniform Laplacian Smoothing

For this exercise the implementation was very straightforward, we used 
the provided calc_uniform_mean_curvature() in QualityViewer so as to get
the uniform Laplace operator. We just removed the unused curvature
variables and added 0.5 of the operator to each non adjacent vertex to
get the desired result.

3.2 Laplace-Beltrami Smoothing

As for the previous exercise, we used the provided function from last
exercise correction. That time the only difference with the previous
hw was the normalization factor. It is given by one over the sum of the
cotan weights (which computation is also very straightforward and
inexpensive as it can be done in the same loop)

3.3 Feature Enhancement

For those two functions uniform_laplacian_enhance_feature() and
beltrami_enhance_feature() the logic is actually pretty simple as it 
just consist of adding the difference between the mesh before and after
the filtering times a factor. To do that we had to copy the mesh
vertices so as to get memory of the mesh before the filtering and then
apply the filtering and finally compute the aforementioned function.

3.3 (tests)

We found three interesting ways of using those enhancement functions.
The first of which was suggested during the course: having some sort of
cartoon caricature of a face. As the artist would try to detect
representative characteristics of a person's face and draw it by
stressing them, it is here replaced by the algorithm detecting
low-frequencies geometric signals (interesting features) and enhancing
them. 1* 
The second of which was maybe not the intended way of using this
algorithm (as it technically consist of features reduction), but could
nevertheless be useful. As we were provided with a very noisy geometric
object of Anastasia (retrieved from a point cloud and itself from
pictures) and we were not fully satisfied with the two smoothing
functions that we have, we hypothesized that using more iterations of
smoothing but averaging the result with the initial mesh might give good
result. To achieve that we assigned a value of .5 to alpha. 2*
The third one is hypothetical as we were not able to reach very good results
with the provided meshes. What could be interesting to do with such
a function would be, in the case of historical objects such as statue or
geological objects such as rivers would be to try to recover from the 
smoothing that the air and water applied to them. In the case of a statue
it is quite often that the nose for instance happen to be more rounded
than it was at its construction and for rivers it might be the case that
it originated as some sort of canyon that got flared over time. We tried
a bit with the provided egea.obj as in this case it seems pretty obvious
that the nose was damaged of some sort. The trouble with that particular
mesh is that the damage added some concave area to the right of the mouth,
damage that clearly become too visible whenever we tried to apply some
feature enhancement. There might still be models for which 
this technique could be adapted.


1* Cartoon caricature
iterations=10, alpha=2.0
10 uniform smoothing iterations: done
Uniform Laplacian feature enhancement: done
Laplace-Beltrami feature enhancement: done
Uniform Laplacian feature enhancement: done
Uniform Laplacian feature enhancement: done
1 iteartions of uniform smoothing: done
image1.jpg

2* Avg smoothing
iterations=30 alpha=0.5
Laplace-Beltrami feature enhancement: done
image2.jpg
