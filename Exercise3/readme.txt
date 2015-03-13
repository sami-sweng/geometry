
Uniform Laplace curvature

We have changed only the calc_uniform_mean_curvature() function in the QualityViewer class. For every vertex of the input mesh, we initialize laplace to zero, visit all neihboring vertices using Vertez_around_vertex_circulator vv_c, and calculate the equation for uniform Laplace approximation given in the exercise sheet. We computed the length of a vector laplace using norm() function from surface_mesh library.


Laplace-Beltrami curvature

We have changed only the calc_mean_curvature() function in the QualityViewer class. For every vertex of the input mesh, we initialize laplace to zero, iterate through all halfedges going out from the current vertex we are processing, use precomputed values eweight_ of cot(alpha_i)+cot(beta_i) and vweight_ for 1/2A to calculate the Laplace-Beltrami operator with equation given in exercise sheet. Finally, we compute the length of a vector laplace using norm() function from surface_mesh library, and store the half of that length in vcurvature_ property for every vertex.    


Triangle shapes

We have changed only the calc_triangle_quality() function in the QualityViewer class. For every face of the input mesh, we take the positions of vertices of the currently processing face in points A, B and C, calculate the denominator denom as a norm of a cross product of two vectors with the same origin in the current face (C-A and C-B). If the denom is smaller than predefined small value FLT_MIN, we set the tshpae_ to be large (FLT_MAX). If denom is not smaller than FLT_MIN, then we compute the length of all three edges of a current face, store them in norm_a, norm_b, and norm_c, find the minimal edge length and store it in min_length_sq, compute the circum_radius_sq as norm(a)* norm(b) * norm(c)/(2*norm(a x b)). Finally, we set the tshape_ to be a ratio of circum_radius_sq and min_length_sq.


Gaussian curvature

We have changed only the calc_gauss_curvature() function in the QualityViewer class. For every vertex of the input mesh, we iterate through neighboring vertices, calculating the inner angles as a acos of dot products of vectors defining the angle. If a dot product is smaller than -0.1 we keep the -0.1 as a cosine of the inner angle, and if it's higher than 1.0, we keep the cos equal to 1.0. We use the precomputed values vweight_ for 1/2A, and calculate the gaussian curvature approximation with equation given in exercise sheet. In variable anles we store the sum of inner angles at the current vertex we are processing.

More comments can be found in the code.

